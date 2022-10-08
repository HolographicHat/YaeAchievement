using System.Text.RegularExpressions;
using YaeAchievement.res;

namespace YaeAchievement; 

public static class AppConfig {
    
    public static string GamePath { get; private set; } = null!;

    internal static void Load(string argumentPath) {
        if (argumentPath != "auto" && File.Exists(argumentPath)) {
            GamePath = argumentPath;
            return;
        }
        var pathCacheFile = new CacheFile("genshin_impact_game_path");
        if (pathCacheFile.Exists()) {
            var path = pathCacheFile.Read().Content.ToStringUtf8();
            if (path != null && File.Exists(path)) {
                GamePath = path;
                return;
            }
        }
        var appDataPath = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
        var cnLogPath = Path.Combine(appDataPath, @"..\LocalLow\miHoYo\原神\output_log.txt");
        var osLogPath = Path.Combine(appDataPath, @"..\LocalLow\miHoYo\Genshin Impact\output_log.txt");
        if (!File.Exists(cnLogPath) && !File.Exists(osLogPath)) {
            throw new ApplicationException(App.ConfigNeedStartGenshin);
        }
        string finalLogPath;
        if (!File.Exists(osLogPath)) {
            finalLogPath = cnLogPath;
        } else if (!File.Exists(cnLogPath)) {
            finalLogPath = osLogPath;
        } else {
            var cnLastWriteTime = File.GetLastWriteTime(cnLogPath);
            var osLastWriteTime = File.GetLastWriteTime(osLogPath);
            finalLogPath = cnLastWriteTime > osLastWriteTime ? cnLogPath : osLogPath;
        }
        GamePath = GetGamePathFromLogFile(finalLogPath) 
                   ?? GetGamePathFromLogFile($"{finalLogPath}.last") 
                   ?? throw new ApplicationException(App.ConfigNeedStartGenshin);
        pathCacheFile.Write(GamePath);
    }

    private static string? GetGamePathFromLogFile(string path) {
        if (!File.Exists(path)) {
            return null;
        }
        var copiedLogFilePath = Path.GetTempFileName();
        File.Copy(path, copiedLogFilePath, true);
        var content = File.ReadAllText(copiedLogFilePath);
        try {
            File.Delete(copiedLogFilePath);
        } catch (Exception) { /* ignore */}
        var matchResult = Regex.Match(content, @"(?m).:/.+(GenshinImpact_Data|YuanShen_Data)");
        if (!matchResult.Success) {
            return null;
        }
        var entryName = matchResult.Groups["1"].Value.Replace("_Data", ".exe");
        return Path.GetFullPath(Path.Combine(matchResult.Value, "..", entryName));
    }
}
