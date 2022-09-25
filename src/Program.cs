using YaeAchievement;
using YaeAchievement.AppCenterSDK;
using YaeAchievement.AppCenterSDK.Models;
using YaeAchievement.res;
using static YaeAchievement.Utils;

InstallExitHook();
InstallExceptionHook();

await CheckVcRuntime();
CheckSelfIsRunning();
TryDisableQuickEdit();
CheckGenshinIsRunning();

Console.WriteLine(@"----------------------------------------------------");
Console.WriteLine(App.AppBanner, GlobalVars.AppVersionName);
Console.WriteLine(@"https://github.com/HolographicHat/YaeAchievement");
Console.WriteLine(@"----------------------------------------------------");

AppConfig.Load();
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
    Console.WriteLine(App.UsePreviousData);
    Console.WriteLine(App.RefreshData);
    Export.Choose(AchievementAllDataNotify.Parser.ParseFrom(historyCache.Read().Content));
} else {
    StartAndWaitResult(AppConfig.GamePath, str => {
        GlobalVars.UnexpectedExit = false;
        var data = Convert.FromBase64String(str);
        var list = AchievementAllDataNotify.Parser.ParseFrom(data);
        historyCache.Write(data);
        Export.Choose(list);
        return true;
    });
}