const fs = require("fs")
const util = require("util")
const readline = require("readline")
const { spawnSync } = require("child_process")
const { loadCache } = require("./utils")

const exportToSeelie = proto => {
    const out = { achievements: {} }
    proto.list.filter(achievement => achievement.status === 3).forEach(({id}) => {
        out.achievements[id] = { done: true }
    })
    const fp = `./export-${Date.now()}-seelie.json`
    fs.writeFileSync(fp, JSON.stringify(out))
    console.log(`导出为文件: ${fp}`)
}

const exportToPaimon = async proto => {
    const out = { achievements: {} }
    const achTable = new Map()
    const excel = await loadCache("ExcelBinOutput/AchievementExcelConfigData.json")
    excel.forEach(({GoalId, Id}) => {
        achTable.set(Id, GoalId === undefined ? 0 : GoalId)
    })
    proto.list.filter(achievement => achievement.status === 3).forEach(({id}) => {
        const gid = achTable.get(id)
        if (out.achievements[gid] === undefined) {
            out.achievements[gid] = {}
        }
        out.achievements[gid][id] = true
    })
    const fp = `./export-${Date.now()}-paimon.json`
    fs.writeFileSync(fp, JSON.stringify(out))
    console.log(`导出为文件: ${fp}`)
}

const exportToCocogoat = async proto => {
    const out = {
        value: {
            achievements: []
        }
    }
    const achTable = new Map()
    const preStageAchievementIdList = []
    const excel = await loadCache("ExcelBinOutput/AchievementExcelConfigData.json")
    excel.forEach(({GoalId, Id, PreStageAchievementId}) => {
        if (PreStageAchievementId !== undefined) {
            preStageAchievementIdList.push(PreStageAchievementId)
        }
        achTable.set(Id, GoalId === undefined ? 0 : GoalId)
    })
    const p = i => i.toString().padStart(2, "0")
    const getDate = ts => {
        const d = new Date(parseInt(`${ts}000`))
        return `${d.getFullYear()}/${p(d.getMonth()+1)}/${p(d.getDate())}`
    }
    proto.list.filter(achievement => achievement.status === 3).forEach(({current, finishTimestamp, id, require}) => {
        out.value.achievements.push({
            id: id,
            status: current === undefined || current === 0 || preStageAchievementIdList.includes(id) ? `${require}/${require}` : `${current}/${require}`,
            categoryId: achTable.get(id),
            date: getDate(finishTimestamp)
        })
    })
    spawnSync("clip", { input: JSON.stringify(out,null,2) })
    console.log("导出内容已复制到剪贴板.")
}

const exportToCsv = async proto => {
    const excel = await loadCache("achievement-data.json", "HolographicHat/genshin-achievement-export")
    const achievementMap = new Map()
    excel["achievement"].forEach(obj => {
        achievementMap.set(parseInt(obj.id), obj)
    })
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
    proto.list.forEach(({current, finishTimestamp, id, status, require}) => {
        const desc = achievementMap.get(id) === undefined ? (() => {
            console.log(`Error get id ${id} in excel`)
            return {
                goal: "未知",
                name: "未知",
                desc: "未知"
            }
        })() : achievementMap.get(id)
        outputLines.push(`${id},${getStatusText(status)},${excel.goal[desc.goal]},${desc.name},${desc.desc},${status !== 1 ? current === 0 ? require : current : current},${require},${status === 1 ? "" : getTime(finishTimestamp)}`)
    })
    const fp = `./export-${Date.now()}.csv`
    fs.writeFileSync(fp, `\uFEFF${outputLines.join("\n")}`)
    console.log(`导出为文件: ${fp}`)
}

const exportData = async proto => {
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    })
    const question = util.promisify(rl.question).bind(rl)
    const chosen = await question("导出至: \n[0] 椰羊 (https://cocogoat.work/achievement)\n[1] Paimon.moe\n[2] Seelie.me\n[3] 表格文件 (默认)\n> ")
    rl.close()
    switch (chosen) {
        case "0":
            await exportToCocogoat(proto)
            break
        case "1":
            await exportToPaimon(proto)
            break
        case "2":
            await exportToSeelie(proto)
            break
        default:
            await exportToCsv(proto)
    }
}

module.exports = {
    exportData
}
