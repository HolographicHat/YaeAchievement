using System.Reflection;
using Proto;

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

    public const uint   AppVersionCode = 233;
    public const string AppVersionName = "5.2";

    public const string PipeName = "YaeAchievementPipe";
    public const string BucketHost = "https://cn-cd-1259389942.file.myqcloud.com";

    public static AchievementInfo AchievementInfo { get; } 
        = AchievementInfo.Parser.ParseFrom(Utils.GetBucketFileAsByteArray("schicksal/metadata"));

    static GlobalVars() {
        Directory.CreateDirectory(DataPath);
        Directory.CreateDirectory(CachePath);
    }
}
