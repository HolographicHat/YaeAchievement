const proxy = require("udp-proxy")
const cp = require("child_process")
const rs = require("./regionServer")
const appcenter = require("./appcenter")
const {
    initConfig, splitPacket, upload, decodeProto, log, setupHost, KPacket, debug, checkCDN, checkUpdate,
    brotliCompressSync, brotliDecompressSync, checkGameIsRunning, checkPortIsUsing
} = require("./utils")
const { exportData } = require("./export")
const { enablePrivilege, pause } = require("./native")

const onExit = () => {
    setupHost(true)
    console.log("按任意键退出")
    pause()
};

(async () => {
    try {
        process.once("SIGHUP", () => setupHost(true))
        process.on("unhandledRejection", (reason, promise) => {
            console.log("Unhandled Rejection at: ", promise, "\n0Reason:", reason)
        })
        process.once("uncaughtException", (err, origin) => {
            appcenter.uploadError(err, true)
            console.log(err)
            console.log(`Origin: ${origin}`)
            process.exit(1)
        })
        process.once("exit", onExit)
        process.once("SIGINT", onExit)
        try {
            enablePrivilege()
        } catch (e) {
            console.log("请使用管理员身份运行此程序")
            process.exit(-1)
        }
        appcenter.startup()
        let conf = await initConfig()
        checkPortIsUsing()
        checkGameIsRunning()
        log("检查更新")
        await checkUpdate()
        checkCDN().then(_ => debug("CDN check success.")).catch(reason => {
            console.log(reason)
            process.exit(113)
        })
        let gameProcess
        let unexpectedExit = true
        rs.create(conf,() => {
            setupHost()
            gameProcess = cp.execFile(conf.executable, { cwd: conf.path },err => {
                if (err !== null && !err.killed) {
                    throw err
                }
            })
            log("启动原神")
            gameProcess.on("exit", () => {
                if (unexpectedExit) {
                    console.log("游戏进程异常退出")
                    process.exit(0)
                }
            })
        },(ip, port, hServer) => {
            let login = false
            let cache = new Map()
            let lastRecvTimestamp = 0
            // noinspection JSUnusedGlobalSymbols
            const options = {
                address: ip,
                port: port,
                localaddress: "127.0.0.1",
                localport: 45678,
                middleware: {
                    message: (msg, sender, next) => {
                        const buf = Buffer.from(msg)
                        if (!(login && buf.readUInt8(8) === 0x51)) {
                            next(msg, sender)
                        }
                    },
                    proxyMsg: (msg, sender, peer, next) => {
                        try { next(msg, sender, peer) } catch (e) {}
                    }
                }
            }
            let monitor;
            let stopped = false
            const createMonitor = () => {
                monitor = setInterval(async () => {
                    if (login && lastRecvTimestamp + 2 < parseInt(Date.now() / 1000) && !stopped) {
                        stopped = true
                        unexpectedExit = false
                        server.close(() => {})
                        hServer.close()
                        gameProcess.kill()
                        clearInterval(monitor)
                        setupHost(true)
                        log("正在处理数据，请稍后...")
                        let packets = Array.from(cache.values())
                        cache.clear()
                        packets.sort((a, b) => a.frg - b.frg)
                            .sort((a, b) => a.sn - b.sn)
                            .filter(i => i.data.byteLength !== 0)
                            .forEach(i => {
                                const psn = i.sn + i.frg
                                cache.has(psn) ? (() => {
                                    const arr = cache.get(psn)
                                    arr.push(i.data)
                                    cache.set(psn, arr)
                                })() : cache.set(psn, [i.data])
                            })
                        packets = Array.from(cache.values())
                            .map(arr => {
                                const data = Buffer.concat(arr)
                                const len = Buffer.alloc(4)
                                len.writeUInt32LE(data.length)
                                return Buffer.concat([len, data])
                            })
                        const merged = Buffer.concat(packets)
                        const compressed = brotliCompressSync(merged)
                        const response = await upload(compressed)
                        const data = brotliDecompressSync(response.data)
                        if (response.status !== 200) {
                            log(`发生错误: ${data}`)
                            log(`请求ID: ${response.headers["x-api-requestid"]}`)
                            log("请联系开发者以获取帮助")
                        } else {
                            const proto = await decodeProto(data,"AllAchievement")
                            await exportData(proto)
                        }
                        process.exit(0)
                    }
                },1000)
            }
            const server = proxy.createServer(options)
            server.on("message", (msg, _) => {
                if (msg.byteLength > 500) {
                    login = true
                }
            })
            server.on("error", err => console.log(`Proxy error: ${err.message}` + err.message))
            server.on("proxyError", err => console.log(`Proxy error: ${err.message}` + err.message))
            server.on("proxyMsg", (msg, _) => {
                lastRecvTimestamp = parseInt(Date.now() / 1000)
                let buf = Buffer.from(msg)
                if (buf.byteLength <= 20) {
                    switch(buf.readUInt32BE(0)) {
                        case 325:
                            createMonitor()
                            debug("服务端握手应答")
                            break
                        case 404:
                            lastRecvTimestamp = parseInt(Date.now() / 1000) - 2333
                            break
                        default:
                            console.log(`Unhandled: ${buf.toString("hex")}`)
                            process.exit(2)
                            break
                    }
                    return
                }
                splitPacket(buf).forEach(sb => {
                    if (sb.readUInt8(8) === 0x51) {
                        const p = new KPacket(sb)
                        if (!cache.has(p.hash)) cache.set(p.hash, p)
                    }
                })
            })
            return server
        }).then(() => log("加载完毕"))
    } catch (e) {
        console.log(e)
        if (e instanceof Error) {
            appcenter.uploadError(e, true)
        } else {
            appcenter.uploadError(Error(e), true)
        }
        process.exit(1)
    }
})()
