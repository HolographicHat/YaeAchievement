using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace YaeAchievement; 

[SuppressMessage("Usage", "CA2211:非常量字段应当不可见")]
[SuppressMessage("ReSharper", "ConvertToConstant.Global")]
[SuppressMessage("ReSharper", "FieldCanBeMadeReadOnly.Global")]
public static class GlobalVars {
    
    public static bool Verbose = false;
    public static bool UnexpectedExit = true;
    public static string GamePath = "";
    public static Version AppVersion = Assembly.GetEntryAssembly()!.GetName().Version!;
    
    public const uint   AppVersionCode = 77;
    public const string AppVersionName = "2.?";
    public const string LibName = "YaeLib.dll";
    public const string PipeName = "YaeAchievementPipe";
    public const string BucketHost = "https://cn-cd-1259389942.file.myqcloud.com";
    public const string ConfigFileName = "YaeAchievement.runtimeconfig.json";
    
}
