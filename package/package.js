const fs = require("fs")
const pkg = require("pkg")
const { log } = require("../utils")
const { version } = require("../version")
const { execSync } = require("child_process")

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


const c = {
    forWin7: false,
    sevenZipPath: "\"C:/Program Files/7-Zip/7z.exe\""
};

(async () => {
    const fn = c.forWin7 ? `${name}-Win7` : name
    const nv = c.forWin7 ? "14.19.1" : "16.14.2"
    log("Generate and compile version info")
    generateAndCompileVersionInfo()
    log("Modify executable file resources")
    const path = `C:/Users/holog/.pkg-cache/v3.3/built-v${nv}-win-x64`
    generateScript(path)
    execSync(`rh.exe -script tmp.script`)
    cleanUp()
    log("Build and compress package")
    await pkg.exec(`../app.js -t node${nv.split(".")[0]}-win-x64 -C Brotli --build -o ${fn}.exe`.split(" "))
    execSync(`${c.sevenZipPath} a ${fn}.7z ${fn}.exe -mx=9 -myx=9 -mmt=4 -sdel -stl`)
    log("Done")
})()
