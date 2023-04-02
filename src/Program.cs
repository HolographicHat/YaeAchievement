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
var usePreviousData = false;
var historyCache = new CacheFile("ExportData");
if (historyCache.LastWriteTime.AddMinutes(10) > DateTime.UtcNow) {
    Console.WriteLine(App.UsePreviousData);
    usePreviousData = Console.ReadLine() == "yes";
}
Export:
if(usePreviousData) {
    AchievementAllDataNotify data;
    try {
        data = AchievementAllDataNotify.Parser.ParseFrom(historyCache.Read().Content);
    } catch (Exception) {
        usePreviousData = false;
        goto Export;
    }
    Export.Choose(data);
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