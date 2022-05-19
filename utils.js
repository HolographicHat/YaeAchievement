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
const { uploadEvent } = require("./appcenter")
const messages = path.join(__dirname, "./proto/Messages.proto")

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
        if (conf.offlineResource !== undefined || conf.customCDN !== undefined || conf.proxy !== undefined) {
            conf.proxy = undefined
            conf.customCDN = undefined
            conf.offlineResource = undefined
            fs.writeFileSync(configFileName, JSON.stringify(conf, null, 2))
        }
    } else {
        conf = {
            path: [],
            oversea_api: false
        }
        let p = ""
        try {
            p = path.dirname(native.selectGameExecutable())
        } catch (e) {
            process.exit(1)
        }
        await checkPath(p).catch(reason => {
            console.log(reason)
            process.exit(1)
        })
        conf.path.push(p)
        fs.writeFileSync(configFileName, JSON.stringify(conf, null, 2))
    }
    if (conf.oversea_api === undefined) {
        conf.oversea_api = false
        fs.writeFileSync(configFileName, JSON.stringify(conf, null, 2))
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
    uploadEvent("AppInitialize", { ClientVersion: version.name, GameVersion: conf.version })
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

const openUrl = url => native.openUrl(encodeURI(url))

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

/*
 * Structure:
 * UInt16 = 0x4321: magic num
 * UInt8  = 0x2   : version
 * Char[32]       : data md5
 * ...            : compressed data
 */

const loadCache = async (fp = "latest-data") => {
    log(`正在加载资源: ${fp}`)
    const startAt = Date.now()
    fs.mkdirSync("./cache", { recursive: true })
    // remove old cache file
    fs.readdir("./cache/", (err, files) => {
        if (err === null) files.forEach(s => {
            const fn = `./cache/${s}`
            if (!fn.endsWith(".ch")) fs.rm(fn,_ => {})
        })
    })
    const localPath = `./cache/${md5(fp)}.ch`
    const headers = {
        "x-content-hash": "0".repeat(32)
    }
    let fd = Buffer.alloc(0)
    if (fs.existsSync(localPath)) {
        fd = fs.readFileSync(localPath)
        if (fd.readUInt16BE(0) === 0x4321 && fd.readUInt8(2) === 2) {
            headers["x-content-hash"] = fd.subarray(3, 35).toString("utf-8")
        }
    }
    const resp = await cloud.get(`/${fp}`, "application/json", headers, "arraybuffer")
    if (resp.status === 304) {
        log(`完成 (Cache, ${Date.now() - startAt}ms)`)
        return JSON.parse(brotliDecompressSync(fd.subarray(35)).toString())
    } else {
        const compressedData = resp.data
        const decompressedData = brotliDecompressSync(compressedData)
        const buf = Buffer.allocUnsafe(compressedData.length + 35)
        buf.writeUInt16BE(0x4321, 0)
        buf.writeUInt8(0x2, 2)
        buf.fill(md5(decompressedData), 3)
        buf.fill(compressedData, 35)
        fs.writeFileSync(localPath, buf)
        log(`完成 (Network, ${Date.now() - startAt}ms)`)
        return JSON.parse(decompressedData)
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
    const data = (await cloud.fetchBucket("/latest.json")).data
    if (data["vc"] !== version.code) {
        log(`有可用更新: ${version.name} => ${data["vn"]}`)
        log(`更新内容: \n${data["ds"]}`)
        log("下载地址: https://github.com/HolographicHat/genshin-achievement-export/releases\n")
    }
    if (data["fc"] === true) {
        process.exit(410)
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
    setupHost, loadCache, debug, checkUpdate, KPacket, checkGameIsRunning, checkPortIsUsing, openUrl
}
