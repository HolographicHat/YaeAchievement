const fs = require("fs")
const dns = require("dns")
const ini = require("ini")
const zlib = require("zlib")
const cloud = require("./secret")
const native = require("./native")
const readline = require("readline")
const protobuf = require("protobufjs")
const { version } = require("./version")
const { promisify } = require("util")
const { createHash } = require("crypto")
const path = require("path")
const messages = path.join(__dirname, "./proto/Messages.proto")
let axios = require("axios")

const encodeProto = (object, name) => protobuf.load(messages).then(r => {
    const msgType = r.lookupType(name)
    const msgInst = msgType.create(object)
    return msgType.encode(msgInst).finish()
})

const decodeProto = (buf, name) => protobuf.load(messages).then(r => {
    return r.lookupType(name).decode(buf)
})

const checkPath = path => new Promise((resolve, reject) => {
    if (!fs.existsSync(`${path}/UnityPlayer.dll`) || !fs.existsSync(`${path}/pkg_version`)) {
        reject(`路径有误 ${path}`)
    } else {
        resolve(path)
    }
})

let conf

const initConfig = async () => {
    const configFileName = "./config.json"
    if (fs.existsSync(configFileName)) {
        conf = JSON.parse(fs.readFileSync(configFileName, "utf-8"))
    } else {
        conf = {
            path: [],
            offlineResource: false,
            customCDN: ""
        }
        const p = path.dirname(native.selectGameExecutable())
        await checkPath(p).catch(reason => {
            console.log(reason)
            process.exit(1)
        })
        conf.path.push(p)
        fs.writeFileSync(configFileName, JSON.stringify(conf, null, 2))
    }
    if (conf.proxy !== undefined) {
        axios = axios.create({
            proxy: conf.proxy
        })
    }
    if (conf.path.length === 1) {
        await checkPath(conf.path[0]).catch(reason => {
            console.log(reason)
            process.exit(1)
        }).then(p => {
            conf.path = p
        })
    } else {
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        })
        const question = (query) => new Promise(resolve => {
            rl.question(query, resolve)
        })
        const idx = await question(`选择客户端: \n${conf.path.map((s, i) => {
            const fp = fs.existsSync(`${s}/GenshinImpact.exe`) ? `${s}\\GenshinImpact.exe` : `${s}\\YuanShen.exe`
            return `[${i}] ${fp}`
        }).join("\n")}\n> `)
        await checkPath(conf.path[parseInt(idx)]).catch(reason => {
            console.log(reason)
            process.exit(1)
        }).then(p => {
            conf.path = p
        })
        rl.close()
    }
    conf.isOversea = fs.existsSync(conf.path + "/GenshinImpact.exe")
    conf.dataDir = conf.isOversea ? conf.path + "/GenshinImpact_Data" : conf.path + "/YuanShen_Data"
    const readGameRes = (path) => fs.readFileSync(conf.dataDir + path, "utf-8")
    const genshinConf = ini.parse(fs.readFileSync(`${conf.path}/config.ini`, "utf-8"))["General"]
    conf.channel = genshinConf["channel"]
    conf.subChannel = genshinConf["sub_channel"]
    conf.version = readGameRes("/Persistent/ChannelName") + readGameRes("/Persistent/ScriptVersion")
    conf.executable = conf.isOversea ? `${conf.path}/GenshinImpact.exe` : `${conf.path}/YuanShen.exe`
    conf.dispatchUrl = `dispatch${conf.isOversea ? "os" : "cn"}global.yuanshen.com`
    conf.dispatchIP = (await promisify(dns.lookup).bind(dns)(conf.dispatchUrl, 4)).address
    return conf
}

const checkGameIsRunning = () => {
    const name = path.basename(conf.executable)
    if (native.checkGameIsRunning(name)) {
        console.log("原神正在运行，请关闭后重试")
        process.exit(301)
    }
}

const checkPortIsUsing = () => {
    const portUsage = native.whoUseThePort(443)
    if (portUsage !== undefined) {
        console.log(`443端口被 ${path.basename(portUsage["path"])}(${portUsage["pid"]}) 占用，结束该进程后重试`)
        process.exit(14)
    }
}

const splitPacket = buf => {
    let offset = 0
    let arr = []
    while (offset < buf.length) {
        let dataLength = buf.readUInt32LE(offset + 24)
        arr.push(buf.subarray(offset, offset + 28 + dataLength))
        offset += dataLength + 28
    }
    return arr
}

const md5 = str => {
    const h = createHash("md5")
    h.update(str)
    return h.digest("hex")
}

let cdnUrlFormat = undefined

String.prototype.format = function() {
    const args = arguments;
    return this.replace(/{(\d+)}/g, (match, number) => typeof args[number] != "undefined" ? args[number] : match)
}

const checkCDN = async () => {
    const controller = new AbortController()
    const a = new Promise(resolve => axios.get("https://cdn.jsdelivr.net/gh/HolographicHat/genshin-achievement-export@master/2MB.bin", {
        signal: controller.signal
    }).then(_ => resolve(["jsdelivr", "https://cdn.jsdelivr.net/gh/{0}@master/{1}"])))
    const b = new Promise(resolve => axios.get("https://raw.githubusercontent.com/HolographicHat/genshin-achievement-export/master/2MB.bin", {
        signal: controller.signal
    }).then(_ => resolve(["githubusercontent", "https://raw.githubusercontent.com/{0}/master/{1}"])))
    const c = new Promise(resolve => axios.get("https://ghproxy.net/https://raw.githubusercontent.com/HolographicHat/genshin-achievement-export/master/2MB.bin", {
        signal: controller.signal
    }).then(_ => resolve(["ghproxy", "https://ghproxy.net/https://raw.githubusercontent.com/{0}/master/{1}"])))
    const d = new Promise(resolve => {
        const s = conf === undefined ? "" : conf.customCDN.trim() // 🤔
        if (s.length === 0) throw "empty"
        axios.get(s.format("HolographicHat/genshin-achievement-export", "2MB.bin"), {
            signal: controller.signal
        }).then(_ => resolve(["customCDN", s]))
    })
    await Promise.any([a, b, c, d]).then(([name, url]) => {
        debug(`Fastest: ${name}`)
        controller.abort()
        cdnUrlFormat = url
    }).catch()
    if (cdnUrlFormat === undefined) {
        throw "网络错误，请检查配置文件/网络后重试 (11-3)"
    }
}

const loadCache = async (fp, repo = "Dimbreath/GenshinData") => {
    log(`预检资源: ${cdnUrlFormat.format(repo, fp)}`)
    fs.mkdirSync("./cache", { recursive: true })
    const localPath = `./cache/${md5(fp)}`
    if (conf.offlineResource) {
        const fd = brotliDecompressSync(fs.readFileSync(localPath))
        return JSON.parse(fd.subarray(1 + fd.readUInt8()).toString())
    }
    const header = {}
    let fd = Buffer.alloc(0)
    if (fs.existsSync(localPath)) {
        fd = brotliDecompressSync(fs.readFileSync(localPath))
        const etagLength = fd.readUInt8()
        header["If-None-Match"] = fd.subarray(1, 1 + etagLength).toString()
    }
    const headResponse = await axios.head(cdnUrlFormat.format(repo, fp), {
        headers: header,
        validateStatus: _ => true
    })
    if (headResponse.status === 304) {
        log("%s 命中缓存", fp)
        const etagLength = fd.readUInt8()
        return JSON.parse(fd.subarray(1 + etagLength).toString())
    } else {
        log("下载所需资源, 请稍后...")
        const response = await axios.get(cdnUrlFormat.format(repo, fp))
        const etag = response.headers.etag
        const str = JSON.stringify(response.data)
        const comp = brotliCompressSync(Buffer.concat([Buffer.of(etag.length), Buffer.from(etag), Buffer.from(str)]))
        fs.writeFileSync(localPath, comp)
        log("下载完成")
        return response.data
    }
}

const isDebug = true

const debug = (msg, ...params) => {
    if (isDebug) log(msg, ...params)
}

const log = (msg, ...params) => {
    const time = new Date()
    const timeStr = time.getHours().toString().padStart(2, "0") + ":" + time.getMinutes().toString().padStart(2, "0") + ":" + time.getSeconds().toString().padStart(2, "0")
    console.log(`${timeStr} ${msg}`, ...params)
}

const upload = async data => {
    return await cloud.post("/achievement-export", data)
}

const checkUpdate = async () => {
    const data = (await cloud.get("/latest-version")).data
    if (data["vc"] !== version.code) {
        log(`有可用更新: ${version.name} => ${data["vn"]}`)
        log(`更新内容: \n${data["ds"]}`)
        log("下载地址: https://github.com/HolographicHat/genshin-achievement-export/releases\n")
    }
}

const brotliCompressSync = data => zlib.brotliCompressSync(data,{
    params: {
        [zlib.constants.BROTLI_PARAM_QUALITY]: zlib.constants.BROTLI_MAX_QUALITY,
        [zlib.constants.BROTLI_PARAM_SIZE_HINT]: data.length
    }
})

const brotliDecompressSync = data => zlib.brotliDecompressSync(data)

let hostsContent = undefined

const setupHost = (restore = false) => {
    if (restore && hostsContent === undefined) return
    const path = `${process.env.windir}\\System32\\drivers\\etc\\hosts`
    if (!fs.existsSync(path)) {
        fs.writeFileSync(path, "")
    }
    fs.chmodSync(path, 0o777)
    if (restore) {
        fs.writeFileSync(path, hostsContent)
    } else {
        hostsContent = fs.readFileSync(path, "utf-8")
        const requireHosts = new Map()
        requireHosts.set(conf.dispatchUrl, "127.0.0.1")
        requireHosts.set("localdispatch.yuanshen.com", "127.0.0.1")
        const currentHosts = new Map()
        hostsContent.split("\n").map(l => l.trim()).filter(l => !l.startsWith("#") && l.length > 0).forEach(value => {
            const pair = value.trim().split(" ").filter(v => v.trim().length !== 0)
            currentHosts.set(pair[1], pair[0])
        })
        requireHosts.forEach((value, key) => {
            if (currentHosts.has(key)) {
                if (currentHosts.get(key) === value) {
                    requireHosts.delete(key)
                } else {
                    currentHosts.delete(key)
                }
            }
        })
        requireHosts.forEach((ip, host) => {
            currentHosts.set(host, ip)
        })
        const newContent = Array.from(currentHosts.entries()).map(pair => {
            return `${pair[1]} ${pair[0]}`
        }).join("\n")
        fs.writeFileSync(path, newContent)
    }
    debug("修改SystemHosts")
    process.on("exit", () => {
        fs.writeFileSync(path, hostsContent)
    })
}

// noinspection JSUnusedGlobalSymbols
class KPacket {

    constructor(data) {
        this.origin = data
        this.conv   = data.readUInt32BE(0)
        this.token  = data.readUInt32BE(4)
        this.cmd    = data.readUInt8(8)
        this.frg    = data.readUInt8(9)
        this.wnd    = data.readUInt16LE(10)
        this.ts     = data.readUInt32LE(12)
        this.sn     = data.readUInt32LE(16)
        this.una    = data.readUInt32LE(20)
        this.length = data.readUInt32LE(24)
        this.data   = data.subarray(28)
        this.hash   = (() => {
            const h = createHash("sha256")
            h.update(Buffer.concat([Buffer.of(this.sn, this.frg), this.data]))
            return h.digest("hex")
        })()
    }
}

module.exports = {
    log, encodeProto, decodeProto, initConfig, splitPacket, upload, brotliCompressSync, brotliDecompressSync,
    setupHost, loadCache, debug, checkCDN, checkUpdate, KPacket, cdnUrlFormat, checkGameIsRunning, checkPortIsUsing
}
