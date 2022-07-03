using YaeAchievement;
using YaeAchievement.AppCenterSDK;
using YaeAchievement.AppCenterSDK.Models;
using static YaeAchievement.Utils;

InstallExitHook();
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
StartAndWaitResult(GlobalVars.GamePath, str => {
    GlobalVars.UnexpectedExit = false;
    var list = AchievementAllDataNotify.Parser.ParseFrom(Convert.FromBase64String(str));
    Export.Choose(list);
    return true;
});
