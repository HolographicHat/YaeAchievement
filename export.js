const fs = require("fs")
const axios = require("axios")
const readline = require("readline")
const { randomUUID } = require("crypto")
const { loadCache, log } = require("./utils")
const { openUrl, checkSnapFastcall, copyToClipboard } = require("./native")

const exportToSeelie = proto => {
    const out = { achievements: {} }
    proto.list.filter(a => a.status === 3 || a.status === 2).forEach(({id}) => {
        out.achievements[id] = { done: true }
    })
    const fp = `./export-${Date.now()}-seelie.json`
    fs.writeFileSync(fp, JSON.stringify(out))
    log(`导出为文件: ${fp}`)
}

const exportToPaimon = async proto => {
    const out = { achievement: {} }
    const data = await loadCache()
    proto.list.filter(a => a.status === 3 || a.status === 2).forEach(({id}) => {
        const gid = data["a"][id]
        if (out.achievement[gid] === undefined) {
            out.achievement[gid] = {}
        }
        out.achievement[gid][id] = true
    })
    const fp = `./export-${Date.now()}-paimon.json`
    fs.writeFileSync(fp, JSON.stringify(out))
    log(`导出为文件: ${fp}`)
}

const exportToSnapGenshin = async proto => {
    const out = []
    proto.list.filter(a => a.status === 3 || a.status === 2).forEach(({id, finishTimestamp}) => {
        out.push({
            id: id,
            timestamp: finishTimestamp
        })
    })
    if (checkSnapFastcall()) {
        const json = JSON.stringify(out)
        const path = `${process.env.TMP}/YaeAchievement-export-${randomUUID()}`
        fs.writeFileSync(path, json)
        openUrl(`snapgenshin://achievement/import/file?path=\"${path}\"`)
        log("在 SnapGenshin 进行下一步操作")
    } else {
        const json = JSON.stringify(out, null, 2)
        copyToClipboard(json)
        log("导出内容已复制到剪贴板")
    }
}

const exportToCocogoat = async proto => {
    const out = {
        achievements: []
    }
    const data = await loadCache()
    const p = i => i.toString().padStart(2, "0")
    const getDate = ts => {
        const d = new Date(parseInt(`${ts}000`))
        return `${d.getFullYear()}/${p(d.getMonth()+1)}/${p(d.getDate())}`
    }
    proto.list.filter(a => a.status === 3 || a.status === 2).forEach(({current, finishTimestamp, id, require}) => {
        const curAch = data["a"][id]
        out.achievements.push({
            id: id,
            status: current === undefined || current === 0 || curAch["p"] === undefined ? `${require}/${require}` : `${current}/${require}`,
            categoryId: curAch["g"],
            date: getDate(finishTimestamp)
        })
    })
    const response = await axios.post(`https://77.cocogoat.work/v1/memo?source=${encodeURI("全部成就")}`, out).catch(_ => {
        console.log("网络错误，请检查网络后重试 (26-1)")
        process.exit(261)
    })
    if (response.status !== 201) {
        console.log(`API StatusCode 错误，请联系开发者以获取帮助 (26-2-${response.status})`)
        process.exit(262)
    }
    const retcode = openUrl(`https://cocogoat.work/achievement?memo=${response.data.key}`)
    if (retcode > 32) {
        log("在浏览器内进行下一步操作")
    } else {
        log(`导出失败，请联系开发者以获取帮助 (26-3-${retcode})`)
    }
}

const exportToCsv = async proto => {
    const data = await loadCache()
    const outputLines = ["ID,状态,特辑,名称,描述,当前进度,目标进度,完成时间"]
    const getStatusText = i => {
        switch (i) {
            case  1: return "未完成"
            case  2: return "已完成，未领取奖励"
            case  3: return "已完成"
            default: return "未知"
        }
    }
    const getTime = ts => {
        const d = new Date(parseInt(`${ts}000`))
        const p = i => i.toString().padStart(2, "0")
        return `${d.getFullYear()}/${p(d.getMonth() + 1)}/${p(d.getDate())} ${p(d.getHours())}:${p(d.getMinutes())}:${p(d.getSeconds())}`
    }
    const bl = [84517]
    proto.list.forEach(({current, finishTimestamp, id, status, require}) => {
        if (!bl.includes(id)) {
            const curAch = data["a"][id] === undefined ? (() => {
                console.log(`Error get id ${id} in excel`)
                return {
                    g: "未知",
                    n: "未知",
                    d: "未知"
                }
            })() : data["a"][id]
            outputLines.push(`${id},${getStatusText(status)},${data["g"][curAch.g]},${curAch.n},${curAch.d},${status !== 1 ? current === 0 ? require : current : current},${require},${status === 1 ? "" : getTime(finishTimestamp)}`)
        }
    })
    const fp = `./export-${Date.now()}.csv`
    fs.writeFileSync(fp, `\uFEFF${outputLines.join("\n")}`)
    log(`导出为文件: ${fp}`)
}

const exportData = async proto => {
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    })
    const question = (query) => new Promise(resolve => {
        rl.question(query, resolve)
    })
    const chosen = await question("导出至: \n[0] 椰羊 (https://cocogoat.work/achievement)\n[1] SnapGenshin\n[2] Paimon.moe\n[3] Seelie.me\n[4] 表格文件 (默认)\n输入一个数字(0-4): ")
    rl.close()
    switch (chosen.trim()) {
        case "0":
            await exportToCocogoat(proto)
            break
        case "1":
            await exportToSnapGenshin(proto)
            break
        case "2":
            await exportToPaimon(proto)
            break
        case "3":
            await exportToSeelie(proto)
            break
        case "raw":
            fs.writeFileSync(`./export-${Date.now()}-raw.json`, JSON.stringify(proto,null,2))
            log("OK")
            break
        default:
            await exportToCsv(proto)
    }
}

module.exports = {
    exportData
}
