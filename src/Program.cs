using YaeAchievement;
using static YaeAchievement.Utils;

InstallExitHook();
TryDisableQuickEdit();
InstallExceptionHook();
CheckGenshinIsRunning();

Console.WriteLine("----------------------------------------------------");
Console.WriteLine($"YaeAchievement - 原神成就导出工具 ({GlobalVars.AppVersionName})");
Console.WriteLine("https://github.com/HolographicHat/YaeAchievement");
Console.WriteLine("----------------------------------------------------");

LoadConfig();
CheckUpdate();
StartAndWaitResult(GlobalVars.GamePath, str => {
    GlobalVars.UnexpectedExit = false;
    var list = AchievementAllDataNotify.Parser.ParseFrom(Convert.FromBase64String(str));
    Export.Choose(list);
    return true;
});
