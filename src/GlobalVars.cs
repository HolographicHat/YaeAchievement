using System.Reflection;

namespace YaeAchievement; 

// ReSharper disable InconsistentNaming
// ReSharper disable ConvertToConstant.Global
// ReSharper disable FieldCanBeMadeReadOnly.Global
#pragma warning disable CA2211

public static class GlobalVars {
    
    public static bool DebugProxy = false;
    public static bool CheckGamePath = true;
    public static bool UnexpectedExit = true;
    public static Version AppVersion = Assembly.GetEntryAssembly()!.GetName().Version!;
    public static readonly string AppPath = AppDomain.CurrentDomain.BaseDirectory;
    
    public const uint   AppVersionCode = 29;
    public const string AppVersionName = "2.1";
    public const string LibName = "YaeLib.dll";
    public const string PipeName = "YaeAchievementPipe";
    public const string BucketHost = "https://cn-cd-1259389942.file.myqcloud.com";
    
}
