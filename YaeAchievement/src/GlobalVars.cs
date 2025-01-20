using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using Proto;

namespace YaeAchievement;

// ReSharper disable InconsistentNaming
// ReSharper disable ConvertToConstant.Global
// ReSharper disable FieldCanBeMadeReadOnly.Global
// ReSharper disable once MemberCanBePrivate.Global

public static class GlobalVars {

    public static bool PauseOnExit { get; set; } = true;
    public static Version AppVersion { get; } = Assembly.GetEntryAssembly()!.GetName().Version!;

    public static readonly string AppPath = AppDomain.CurrentDomain.BaseDirectory;
    private static readonly string CommonData = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData);
    public static readonly string DataPath = Path.Combine(CommonData, "Yae");
    public static readonly string CachePath = Path.Combine(DataPath, "cache");
    public static readonly string LibFilePath = Path.Combine(DataPath, "YaeAchievement.dll");

    public const uint   AppVersionCode = 234;
    public const string AppVersionName = "5.2";

    public const string PipeName = "YaeAchievementPipe";
    public const string RinBucketHost = "https://rin.holohat.work";
    public const string SakuraBucketHost = "https://cn-cd-1259389942.file.myqcloud.com";

    public static CacheFile AchievementDataCache { get; } = new ("achievement_data");

    [field:MaybeNull]
    public static AchievementInfo AchievementInfo =>
        field ??= AchievementInfo.Parser.ParseFrom(Utils.GetBucketFile("schicksal/metadata").GetAwaiter().GetResult());

    static GlobalVars() {
        Directory.CreateDirectory(DataPath);
        Directory.CreateDirectory(CachePath);
    }

}
