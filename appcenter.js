const cp = require("child_process")
const axios = require("axios")
const crypto = require("crypto")
const { version } = require("./version")

const getTimestamp = (d = new Date()) => {
    const p = i => i.toString().padStart(2, "0")
    return `${d.getUTCFullYear()}-${p(d.getUTCMonth() + 1)}-${p(d.getUTCDate())}T${p(d.getUTCHours())}:${p(d.getUTCMinutes())}:${p(d.getUTCSeconds())}.${p(d.getUTCMilliseconds())}Z`
}

const readRegistry = (path, key) => {
    const i = cp.execSync(`reg query "${path}" /v ${key}`, {
        encoding: "utf-8"
    }).split("\n")[2].split(" ").filter(s => s.length > 0).map(s => s.trim())
    switch (i[1]) {
        case "REG_SZ":
            return i[2]
        case "REG_DWORD":
            return parseInt(i[2])
        default:
            throw "Unsupported"
    }
}

const queue = []
const session = crypto.randomUUID()
const key = "648b83bf-d439-49bd-97f4-e1e506bdfe39"

const install = (() => {
    const s = readRegistry("HKCU\\SOFTWARE\\miHoYoSDK", "MIHOYOSDK_DEVICE_ID")
    return `${s.substring(0, 8)}-${s.substring(8, 12)}-${s.substring(12, 16)}-${s.substring(16, 20)}-${s.substring(20, 32)}`
})()

const device = (() => {
    const csi = cp.execSync("wmic computersystem get manufacturer,model /format:csv", {
        encoding: "utf-8"
    }).split("\n")[2].split(",").map(s => s.trim())
    const osi = cp.execSync("wmic os get currentTimeZone, version /format:csv", {
        encoding: "utf-8"
    }).split("\n")[2].split(",").map(s => s.trim())
    return {
        model: csi[2],
        oemName: csi[1],
        timeZoneOffset: parseInt(osi[1]),
        osBuild: `${osi[2]}.${readRegistry("HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "UBR")}`,
        osVersion: osi[2],
        locale: readRegistry("HKCU\\Control Panel\\International", "LocaleName"),
        carrierCountry: readRegistry("HKCU\\Control Panel\\International\\Geo", "Name"),
        sdkName: "appcenter.wpf.netcore",
        sdkVersion: "4.5.0",
        osName: "WINDOWS",
        appVersion: version.name,
        appBuild: version.code,
        appNamespace: "default"
    }
})()

const upload = () => {
    if (queue.length > 0) {
        try {
            const data = JSON.stringify({ "logs": queue })
            axios.post("https://in.appcenter.ms/logs?api-version=1.0.0", data,{
                headers: {
                    "App-Secret": key,
                    "Install-ID": install
                }
            }).then(_ => {
                queue.length = 0
            })
        } catch (e) {}
    }
}

const uploadError = (err, fatal) => {
    const eid = crypto.randomUUID()
    const reportJson = process.report.getReport(err)
    const reportAttachment = {
        type: "errorAttachment",
        device: device,
        timestamp: getTimestamp(),
        id: crypto.randomUUID(),
        sid: session,
        errorId: eid,
        contentType: "application/json",
        fileName: "report.json",
        data: Buffer.from(JSON.stringify(reportJson, null, 2), "utf-8").toString("base64")
    }
    // noinspection JSUnresolvedVariable
    const errorContent = {
        type: "managedError",
        id: eid,
        sid: session,
        architecture: "AMD64",
        userId: install,
        fatal: fatal,
        processId: process.pid,
        processName: process.argv0.replaceAll("\\", "/").split("/").pop(),
        timestamp: getTimestamp(),
        appLaunchTimestamp: getTimestamp(new Date(Date.now() - process.uptime())),
        exception: {
            "type": err.name,
            "message": err.message,
            "stackTrace": err.stack
        },
        device: device
    }
    queue.push(errorContent, reportAttachment)
    upload()
}

const init = () => {
    queue.push({
        type: "startService",
        services: [ "Analytics","Crashes" ],
        timestamp: getTimestamp(),
        device: device
    })
    queue.push({
        type: "startSession",
        sid: session,
        timestamp: getTimestamp(),
        device: device
    })
    upload()
    setInterval(() => upload(), 10000)
}

module.exports = {
    init, upload, uploadError
}
