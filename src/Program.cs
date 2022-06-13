using YaeAchievement;
using static YaeAchievement.Utils;

InstallExitHook();
InstallExceptionHook();
CheckGenshinIsRunning();
LoadConfig();
return;
StartAndWaitResult(@"D:\Genshin Impact Dev\2.8\YuanShen.exe", str => {
    GlobalVars.UnexpectedExit = false;
    //AchievementAllDataNotify.Parser.ParseFrom(Convert.FromBase64String(str)).List;
    //var notify = Serializer.Deserialize<AchievementAllDataNotify>(stream)!;
    //notify.list;
    return false;
});
