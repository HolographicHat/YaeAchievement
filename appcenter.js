const axios = require("axios")
const crypto = require("crypto")
const { version } = require("./version")
const { getDeviceID, getDeviceInfo } = require("./native")

const getTimestamp = (d = new Date()) => {
    const p = i => i.toString().padStart(2, "0")
    return `${d.getUTCFullYear()}-${p(d.getUTCMonth() + 1)}-${p(d.getUTCDate())}T${p(d.getUTCHours())}:${p(d.getUTCMinutes())}:${p(d.getUTCSeconds())}.${p(d.getUTCMilliseconds())}Z`
}

const queue = []
const session = crypto.randomUUID()
const key = "648b83bf-d439-49bd-97f4-e1e506bdfe39"

const install = (() => {
    const id = getDeviceID()
    return id === undefined ? crypto.randomUUID() : id
})()

const device = (() => {
    const info = getDeviceInfo()
    info.appBuild     = version.code
    info.appVersion   = version.name
    info.sdkName      = "appcenter.wpf.netcore"
    info.sdkVersion   = "4.5.0"
    info.osName       = "WINDOWS"
    info.appNamespace = "default"
    return info
})()

const upload = () => {
    if (queue.length > 0) {
        const logs = []
        for (let i = 0; i <= queue.length; i++) {
            logs.push(queue.pop())
        }
        const data = JSON.stringify({"logs": logs})
        axios.post("https://in.appcenter.ms/logs?api-version=1.0.0", data,{
            headers: {
                "App-Secret": key,
                "Install-ID": install
            }
        }).catch(_ => {}).then()
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

const uploadEvent = (name, prop) => {
    const content = {
        type: "event",
        id: crypto.randomUUID(),
        sid: session,
        name: name,
        properties: prop,
        timestamp: getTimestamp(),
        device: device
    }
    queue.push(content)
}

const startup = () => {
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
    setInterval(() => upload(), 5000)
}

module.exports = {
    startup, upload, uploadError, uploadEvent
}
