const fs = require("fs")
const https = require("https")
const axios = require("axios")
const { decodeProto, encodeProto, debug } = require("./utils")
const path = require("path")
const cert = path.join(__dirname, "./cert/root.p12")

const preparedRegions = {}
let currentProxy = undefined

const getModifiedRegionList = async (conf) => {
    const d = await axios.get(`https://${conf.dispatchUrl}/query_region_list`, {
        responseType: "text",
        params: {
            version: conf.version,
            channel_id: conf.channel,
            sub_channel_id: conf.subChannel
        }
    }).catch(_ => {
        console.log("网络错误，请检查网络后重试 (22-1)")
        process.exit(221)
    })
    const regions = await decodeProto(Buffer.from(d.data,"base64"),"QueryRegionList")
    if (regions["retcode"] !== 0) {
        console.log(`系统错误，请稍后重试 (${regions["retcode"]}-23)`)
        process.exit(23)
    }
    regions.list = regions.list.map(item => {
        const host = new URL(item.url).host
        if (regions.list.length === 1) {
            preparedRegions[host] = true
        }
        item.url = `https://localdispatch.yuanshen.com/query_cur_region/${host}`
        return item
    })
    return (await encodeProto(regions,"QueryRegionList")).toString("base64")
}

const getModifiedRegionInfo = async (url, uc, hs) => {
    const splitUrl = url.split("?")
    const host = splitUrl[0].split("/")[2]
    const noQueryRequest = splitUrl[1] === undefined
    const query = noQueryRequest ? "" : `?${splitUrl[1]}`
    const d = await axios.get(`https://${host}/query_cur_region${query}`, {
        responseType: "text"
    }).catch(_ => {
        console.log("网络错误，请检查网络后重试 (22-2)")
        process.exit(222)
    })
    if (noQueryRequest) {
        preparedRegions[host] = true
        return d.data
    } else {
        const region = await decodeProto(Buffer.from(d.data,"base64"),"QueryCurRegion")
        if (region["retcode"] !== 0) {
            console.log(`${region["message"]} (${region["retcode"]}-24)`)
            process.exit(24)
        }
        const info = region.info
        if (preparedRegions[host]) {
            if (currentProxy !== undefined) {
                currentProxy.close()
            }
            debug("Create udp proxy: %s:%d", info.ip, info.port)
            currentProxy = uc(info.ip, info.port, hs)
        } else {
            preparedRegions[host] = true
        }
        info.ip = "127.0.0.1"
        info.port = 45678
        return (await encodeProto(region,"QueryCurRegion")).toString("base64")
    }
}

const agent = new https.Agent({
    rejectUnauthorized: false
})

const create = async (conf, regionListLoadedCallback, regionSelectCallback) => {
    const regions = await getModifiedRegionList(conf)
    const hServer = https.createServer({
        pfx: fs.readFileSync(cert),
        passphrase: ""
    }, async (request, response) => {
        const url = request.url
        debug("HTTP: %s", url)
        response.writeHead(200, { "Content-Type": "text/html" })
        if (url.startsWith("/query_region_list")) {
            response.end(regions)
        } else if (url.startsWith("/query_cur_region")) {
            const regionInfo = await getModifiedRegionInfo(url, regionSelectCallback, hServer)
            response.end(regionInfo)
        } else {
            const frontResponse = await axios.get(`https://${conf.dispatchIP}${url}`, {
                responseType: "arraybuffer",
                httpsAgent: agent
            }).catch(err => {
                console.log("网络错误，请检查网络后重试 (22-3)")
                console.log(err.message)
                process.exit(223)
            })
            response.end(frontResponse.data)
        }
    }).listen(443, "127.0.0.1", () => {
        regionListLoadedCallback()
    })
}

module.exports = {
    create
}
