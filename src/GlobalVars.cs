using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace YaeAchievement; 

[SuppressMessage("ReSharper", "ConvertToConstant.Global")]
[SuppressMessage("ReSharper", "FieldCanBeMadeReadOnly.Global")]

#pragma warning disable CA2211
public static class GlobalVars {
    
    public static bool DebugProxy = false;
    public static bool CheckGamePath = true;
    public static bool UnexpectedExit = true;
    public static string GamePath = null!;
    public static Logger.Level LogLevel = Logger.Level.Info;
    public static Version AppVersion = Assembly.GetEntryAssembly()!.GetName().Version!;
    
    public const uint   AppVersionCode = 28;
    public const string AppVersionName = "2.0";
    public const string LibName = "YaeLib.dll";
    public const string PipeName = "YaeAchievementPipe";
    public const string BucketHost = "https://cn-cd-1259389942.file.myqcloud.com";
    public const string ConfigFileName = "YaeAchievement.runtimeconfig.json";
    
}
#pragma warning restore CA2211
