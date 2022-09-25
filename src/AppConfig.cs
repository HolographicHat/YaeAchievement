using System.Text.RegularExpressions;
using YaeAchievement.res;

namespace YaeAchievement; 

public static class AppConfig {
    
    public static string GamePath { get; private set; } = null!;

    internal static void Load() {
        var appDataPath = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
        var cnLogPath = Path.Combine(appDataPath, @"..\LocalLow\miHoYo\原神\output_log.txt");
        var osLogPath = Path.Combine(appDataPath, @"..\LocalLow\miHoYo\Genshin Impact\output_log.txt");
        if (!File.Exists(cnLogPath) && !File.Exists(osLogPath)) {
            throw new ApplicationException(App.ConfigNeedStartGenshin);
        }
        string latestLogPath;
        if (!File.Exists(osLogPath)) {
            latestLogPath = cnLogPath;
        } else if (!File.Exists(cnLogPath)) {
            latestLogPath = osLogPath;
        } else {
            var cnLastWriteTime = File.GetLastWriteTime(cnLogPath);
            var osLastWriteTime = File.GetLastWriteTime(cnLogPath);
            latestLogPath = cnLastWriteTime > osLastWriteTime ? cnLogPath : osLogPath;
        }
        var content = File.ReadAllText(latestLogPath);
        var matchResult = Regex.Match(content, @"(?m).:/.+(GenshinImpact_Data|YuanShen_Data)");
        GamePath = matchResult.Value;
    }
}
