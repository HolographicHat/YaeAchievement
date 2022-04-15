const fs = require("fs")
const pkg = require("pkg")
const { version } = require("../version")
const { execSync } = require("child_process")

// noinspection DuplicatedCode
const log = (msg, ...params) => {
    const time = new Date()
    const timeStr = time.getHours().toString().padStart(2, "0") + ":" + time.getMinutes().toString().padStart(2, "0") + ":" + time.getSeconds().toString().padStart(2, "0")
    console.log(`${timeStr} ${msg}`, ...params)
}

const name = `YaeAchievement-${ version.isDev ? "DevBuild" : "Release" }-${((t = new Date()) => {
    const p = i => i.toString().padStart(2, "0")
    return `${p(t.getMonth()+1)}${p(t.getDate())}${p(t.getHours())}${p(t.getMinutes())}`
})()}`

const generateAndCompileVersionInfo = () => {
    const vci = version.name.split(".")
    let content = fs.readFileSync("./version.rc", "utf-8")
    content = content.replaceAll("${BID}"          , vci[2] === undefined ? "0" : vci[2])
    content = content.replaceAll("${YEAR}"         , (new Date()).getFullYear())
    content = content.replaceAll("${MAJOR_VERSION}", vci[0])
    content = content.replaceAll("${MINOR_VERSION}", vci[1])
    content = content.replaceAll("${ORIGINAL_NAME}", `${name}.exe`)
    fs.writeFileSync("./tmp.rc", content, "utf-8")
    execSync(`rh.exe -open tmp.rc -save tmp.res -action compile -log NUL`)
    fs.rm("./tmp.rc", () => {})
}

const generateScript = (path) => {
    let content = fs.readFileSync("./rh.script", "utf-8")
    content = content.replaceAll("${PATH}", path)
    fs.writeFileSync("./tmp.script", content, "utf-8")
}

const cleanUp = () => {
    fs.rm("./rh.ini", () => {})
    fs.rm("./tmp.res", () => {})
    fs.rm("./tmp.script", () => {})
}

const prepare = () => {
    log("Generate and compile version info")
    generateAndCompileVersionInfo()
    log("Modify executable file resources")
    const node = ["14.19.1", "16.14.2"]
    node.forEach(v => {
        const path = `C:/Users/holog/.pkg-cache/v3.3/built-v${v}-win-x64`
        generateScript(path)
        execSync(`rh.exe -script tmp.script`)
    })
    cleanUp()
    log("Done")
}

prepare()
return

/*await Promise.all([
    pkg.exec(`app.js -t node16-win-x64 -C Brotli --build -o ${name}.exe`.split(" ")),
    pkg.exec(`app.js -t node14-win-x64 -C Brotli --build -o ${name}-Win7.exe`.split(" "))
])*/
