using System.Net;
using System.Net.Http.Headers;
using YaeAchievement;
using static YaeAchievement.Utils;

/*InstallExitHook();
InstallExceptionHook();
CheckGenshinIsRunning();
LoadConfig();*/
var ch = GetBucketFileAsString("latest.json");
//Console.WriteLine(c.Send(msg).StatusCode);
return;
StartAndWaitResult(@"D:\Genshin Impact Dev\2.8\YuanShen.exe", str => {
    GlobalVars.UnexpectedExit = false;
    //AchievementAllDataNotify.Parser.ParseFrom(Convert.FromBase64String(str)).List;
    //var notify = Serializer.Deserialize<AchievementAllDataNotify>(stream)!;
    //notify.list;
    return false;
});
