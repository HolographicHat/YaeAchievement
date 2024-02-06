using Proto;
using YaeAchievement;
using YaeAchievement.AppCenterSDK;
using YaeAchievement.AppCenterSDK.Models;
using YaeAchievement.res;
using static YaeAchievement.Utils;

TryDisableQuickEdit();
InstallExitHook();
InstallExceptionHook();

CheckSelfIsRunning();
await CheckVcRuntime();
CheckGenshinIsRunning();

Console.WriteLine(@"----------------------------------------------------");
Console.WriteLine(App.AppBanner, GlobalVars.AppVersionName);
Console.WriteLine(@"https://github.com/HolographicHat/YaeAchievement");
Console.WriteLine(@"----------------------------------------------------");

AppConfig.Load(args.GetOrNull(0) ?? "auto");
Export.ExportTo = ToUIntOrNull(args.GetOrNull(1)) ?? uint.MaxValue;

CheckUpdate(ToBooleanOrFalse(args.GetOrNull(2)));
AppCenter.Init();
new EventLog("AppInit") {
    Properties = {
        { "AppVersion", GlobalVars.AppVersionName },
        { "SystemVersion", DeviceHelper.GetSystemVersion() }
    }
}.Enqueue();

var historyCache = new CacheFile("ExportData");

AchievementAllDataNotify? data = null;
try {
    data = AchievementAllDataNotify.Parser.ParseFrom(historyCache.Read().Content);
} catch (Exception) { /* ignored */ }

if (historyCache.LastWriteTime.AddMinutes(10) > DateTime.UtcNow && data != null) {
    Console.WriteLine(App.UsePreviousData);
    if (Console.ReadLine() == "yes") {
        Export.Choose(data);
        return;
    }
}

StartAndWaitResult(AppConfig.GamePath, str => {
    GlobalVars.UnexpectedExit = false;
    var bytes = Convert.FromBase64String(str);
    var list = AchievementAllDataNotify.Parser.ParseFrom(bytes);
    historyCache.Write(bytes);
    Export.Choose(list);
    return true;
});
