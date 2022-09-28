using System.Reflection;

namespace YaeAchievement; 

// ReSharper disable InconsistentNaming
// ReSharper disable ConvertToConstant.Global
// ReSharper disable FieldCanBeMadeReadOnly.Global

public static class GlobalVars {
    
    public static bool DebugProxy => false;
    public static bool UnexpectedExit { get; set; }= true;
    public static Version AppVersion { get; } = Assembly.GetEntryAssembly()!.GetName().Version!;
    
    private static readonly string DataDir = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData);
    public static readonly string LibPath = Path.Combine(DataDir, "YaeAchievement.dll");
    public static readonly string AppPath = AppDomain.CurrentDomain.BaseDirectory;
    
    public const uint   AppVersionCode = 30;
    public const string AppVersionName = "2.2";
    
    public const string PipeName = "YaeAchievementPipe";
    public const string BucketHost = "https://cn-cd-1259389942.file.myqcloud.com";
    
}
