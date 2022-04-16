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
    finalName: `${name}-Win7`,
    nodeVersion: "14.19.1",
    sevenZipPath: "C:/Program Files/7-Zip/7z.exe"
};

(async () => {
    console.log(c)
    log("Generate and compile version info")
    generateAndCompileVersionInfo()
    log("Modify executable file resources")
    const path = `C:/Users/holog/.pkg-cache/v3.3/built-v${c.nodeVersion}-win-x64`
    generateScript(path)
    execSync(`rh.exe -script tmp.script`)
    cleanUp()
    log("Build and compress package")
    await pkg.exec(`../app.js -t node${c.nodeVersion.split(".")[0]}-win-x64 -C Brotli --build -o ${c.finalName}.exe`.split(" "))
    execSync(`${c.sevenZipPath} a ${c.finalName}.7z ${c.finalName}.exe -mx=9 -myx=9 -mmt=4 -sdel -stl`)
    log("Done")
})()
