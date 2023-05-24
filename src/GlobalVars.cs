using System.Reflection;

namespace YaeAchievement; 

// ReSharper disable InconsistentNaming
// ReSharper disable ConvertToConstant.Global
// ReSharper disable FieldCanBeMadeReadOnly.Global
// ReSharper disable once MemberCanBePrivate.Global

public static class GlobalVars {
    
    public static bool DebugProxy => false;
    public static bool UnexpectedExit { get; set; } = true;
    public static bool PauseOnExit { get; set; } = true;
    public static Version AppVersion { get; } = Assembly.GetEntryAssembly()!.GetName().Version!;
    
    public static readonly string AppPath = AppDomain.CurrentDomain.BaseDirectory;
    private static readonly string CommonData = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData);
    public static readonly string DataPath = Path.Combine(CommonData, "Yae");
    public static readonly string CachePath = Path.Combine(DataPath, "cache");
    public static readonly string LibFilePath = Path.Combine(DataPath, "YaeAchievement.dll");
    
    public const uint   AppVersionCode = 38;
    public const string AppVersionName = "2.8";
    
    public const string PipeName = "YaeAchievementPipe";
    public const string BucketHost = "https://cn-cd-1259389942.file.myqcloud.com";

    static GlobalVars() {
        Directory.CreateDirectory(DataPath);
        Directory.CreateDirectory(CachePath);
    }
}
