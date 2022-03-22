const fs = require("fs")
const dns = require("dns")
const ini = require("ini")
const util = require("util")
const zlib = require("zlib")
const cloud = require("./secret")
const readline = require("readline")
const protobuf = require("protobufjs")
const { version } = require("./version")
const { createHash } = require("crypto")
const path = require("path")
const messages = path.join(__dirname, "./proto/Messages.proto")
let axios = require("axios")

const sleep = ms => new Promise(resolve => {
    setTimeout(resolve, ms)
})

const encodeProto = (object, name) => protobuf.load(messages).then(r => {
    const msgType = r.lookupType(name)
    const msgInst = msgType.create(object)
    return msgType.encode(msgInst).finish()
})

const decodeProto = (buf, name) => protobuf.load(messages).then(r => {
    return r.lookupType(name).decode(buf)
})

const checkPath = (path, cb) => {
    if (!fs.existsSync(`${path}/UnityPlayer.dll`) && !fs.existsSync(`${path}/pkg_version`)) {
        throw Error(`路径有误: ${path}`)
    } else {
        cb(path)
    }
}

let conf

const initConfig = async () => {
    const configFileName = "./config.json"
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    })
    const question = (query) => new Promise(resolve => {
        rl.question(query, resolve)
    })
    const lookup = util.promisify(dns.lookup).bind(dns)
    if (fs.existsSync(configFileName)) {
        conf = JSON.parse(fs.readFileSync(configFileName, "utf-8"))
    } else {
        const p = await question("原神主程序(YuanShen.exe或GenshinImpact.exe)所在路径: (支持多个路径, 使用符号'*'分隔)\n")
        conf = {
            path: [],
            offlineResource: false,
            customCDN: ""
        }
        p.split("*").forEach(s => {
            checkPath(s, () => {
                if (!conf.path.includes(s)) {
                    conf.path.push(s)
                }
            })
        })
        fs.writeFileSync(configFileName, JSON.stringify(conf, null, 2))
        rl.close()
    }
    if (conf.proxy !== undefined) {
        axios = axios.create({
            proxy: conf.proxy
        })
    }
    if (conf.path.length === 1) {
        checkPath(conf.path[0], p => {
            conf.path = p
        })
    } else {
        const idx = await question(`选择客户端: \n${conf.path.map((s, i) => {
            const fp = fs.existsSync(`${s}/GenshinImpact.exe`) ? `${s}\\GenshinImpact.exe` : `${s}\\YuanShen.exe`
            return `[${i}] ${fp}`
        }).join("\n")}\n> `)
        checkPath(conf.path[parseInt(idx)], p => {
            conf.path = p
        })
    }
    rl.close()
    conf.isOversea = fs.existsSync(conf.path + "/GenshinImpact.exe")
    conf.dataDir = conf.isOversea ? conf.path + "/GenshinImpact_Data" : conf.path + "/YuanShen_Data"
    const readGameRes = (path) => fs.readFileSync(conf.dataDir + path)
    // noinspection JSUnresolvedVariable
    const genshinConf = ini.parse(fs.readFileSync(conf.path + "/config.ini", "utf-8")).General
    conf.channel = genshinConf.channel
    // noinspection JSUnresolvedVariable
    conf.subChannel = genshinConf.sub_channel
    conf.version = readGameRes("/Persistent/ChannelName").toString() + readGameRes("/Persistent/ScriptVersion").toString()
    conf.executable = conf.isOversea ? conf.path + "/GenshinImpact.exe" : conf.path + "/YuanShen.exe"
    conf.dispatchUrl = `dispatch${conf.isOversea ? "os" : "cn"}global.yuanshen.com`
    conf.dispatchIP = (await lookup(conf.dispatchUrl, 4)).address
    return conf
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

let cdnUrlFormat = null

String.prototype.format = function() {
    const args = arguments;
    return this.replace(/{(\d+)}/g, (match, number) => typeof args[number] != "undefined" ? args[number] : match)
}

const checkCDN = async () => {
    try {
        cdnUrlFormat = "https://cdn.jsdelivr.net/gh/{0}@master/{1}"
        await axios.head(cdnUrlFormat.format("github/fetch", ".gitignore"))
        return
    } catch (e) {}
    try {
        cdnUrlFormat = "https://raw.githubusercontent.com/{0}/master/{1}"
        await axios.head(cdnUrlFormat.format("github/fetch", ".gitignore"))
        return
    } catch (e) {}
    try {
        const s = conf === undefined ? "" : conf.customCDN.trim()
        if (s.length > 0) {
            cdnUrlFormat = s
            await axios.head(cdnUrlFormat.format("github/fetch", ".gitignore"))
            return
        }
    } catch (e) {}
    try {
        cdnUrlFormat = "https://raw.fastgit.org/{0}/master/{1}"
        await axios.head(cdnUrlFormat.format("github/fetch", ".gitignore"))
        return
    } catch (e) {}
    try {
        cdnUrlFormat = "https://ghproxy.net/https://raw.githubusercontent.com/{0}/master/{1}"
        await axios.head(cdnUrlFormat.format("github/fetch", ".gitignore"))
        return
    } catch (e) {}
    throw "没有可用的CDN"
}

const loadCache = async (fp, repo = "Dimbreath/GenshinData") => {
    console.log(cdnUrlFormat.format(repo, fp))
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
        console.log("文件 %s 命中缓存", fp)
        const etagLength = fd.readUInt8()
        return JSON.parse(fd.subarray(1 + etagLength).toString())
    } else {
        console.log("正在下载资源, 请稍后...")
        const response = await axios.get(cdnUrlFormat.format(repo, fp))
        const etag = response.headers.etag
        const str = JSON.stringify(response.data)
        const comp = brotliCompressSync(Buffer.concat([Buffer.of(etag.length), Buffer.from(etag), Buffer.from(str)]))
        fs.writeFileSync(localPath, comp)
        console.log("完成.")
        return response.data
    }
}

const isDebug = false

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
        console.log(`有可用更新: ${version.name} => ${data["vn"]}`)
        console.log(`更新内容: \n${data["ds"]}`)
        console.log("下载地址: https://github.com/HolographicHat/genshin-achievement-export/releases\n")
    }
}

const brotliCompressSync = data => zlib.brotliCompressSync(data,{
    params: {
        [zlib.constants.BROTLI_PARAM_QUALITY]: zlib.constants.BROTLI_MAX_QUALITY,
        [zlib.constants.BROTLI_PARAM_SIZE_HINT]: data.length
    }
})

const brotliDecompressSync = data => zlib.brotliDecompressSync(data)

let hostsContent = ""

const setupHost = (restore = false) => {
    const path = "C:\\Windows\\System32\\drivers\\etc\\hosts"
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
    log, sleep, encodeProto, decodeProto, initConfig, splitPacket, upload, brotliCompressSync, brotliDecompressSync,
    setupHost, loadCache, debug, checkCDN, checkUpdate, KPacket, cdnUrlFormat
}
