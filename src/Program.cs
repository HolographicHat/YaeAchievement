using YaeAchievement;
using YaeAchievement.AppCenterSDK;
using YaeAchievement.AppCenterSDK.Models;
using static YaeAchievement.Utils;

InstallExitHook();

CheckVcRuntime();
CheckSelfIsRunning();
TryDisableQuickEdit();
InstallExceptionHook();
CheckGenshinIsRunning();

Console.WriteLine("----------------------------------------------------");
Console.WriteLine($"YaeAchievement - 原神成就导出工具 ({GlobalVars.AppVersionName})");
Console.WriteLine("https://github.com/HolographicHat/YaeAchievement");
Console.WriteLine("----------------------------------------------------");

LoadConfig();
CheckUpdate();
AppCenter.Init();
new EventLog("AppInit") {
    Properties = {
        { "AppVersion", GlobalVars.AppVersionName },
        { "SystemVersion", DeviceHelper.GetSystemVersion() }
    }
}.Enqueue();
var historyCache = new CacheFile("ExportData");
if (historyCache.LastWriteTime.AddMinutes(10) > DateTime.UtcNow) {
    Console.WriteLine("使用上一次获取到的成就数据");
    Console.WriteLine("要重新获取数据，手动删除 cache\\d1a8ef40a67a5929.miko 后重新启动 YaeAchievement");
    Export.Choose(AchievementAllDataNotify.Parser.ParseFrom(historyCache.Read().Content));
} else {
    StartAndWaitResult(GlobalVars.GamePath, str => {
        GlobalVars.UnexpectedExit = false;
        var data = Convert.FromBase64String(str);
        var list = AchievementAllDataNotify.Parser.ParseFrom(data);
        historyCache.Write(data);
        Export.Choose(list);
        return true;
    });
}