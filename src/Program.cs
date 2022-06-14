using System.Net;
using System.Net.Http.Headers;
using Google.Protobuf;
using YaeAchievement;
using static YaeAchievement.Utils;

Console.WriteLine("----------------------------------------------------");
Console.WriteLine("YaeAchievement - 原神成就导出工具");
Console.WriteLine("https://github.com/HolographicHat/YaeAchievement");
Console.WriteLine("----------------------------------------------------");
/*InstallExitHook();
InstallExceptionHook();
CheckGenshinIsRunning();
LoadConfig();
CheckUpdate();*/
TryDisableQuickEdit();
//Console.WriteLine(c.Send(msg).StatusCode);
//using var o = File.OpenWrite("ai");
//AchievementInfo.Parser.ParseJson(File.ReadAllText(@"C:\Users\holog\Desktop\cc.json")).WriteTo(o);
StartAndWaitResult(@"D:\Genshin Impact Dev\2.8\YuanShen.exe", str => {
    GlobalVars.UnexpectedExit = false;
    var list = AchievementAllDataNotify.Parser.ParseFrom(Convert.FromBase64String(str));
    Export.Choose(list);
    return true;
});
