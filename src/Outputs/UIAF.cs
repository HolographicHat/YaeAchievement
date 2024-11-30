using System.Text.Json;
using System.Text.Json.Serialization;
using YaeAchievement.Parsers;

namespace YaeAchievement.Outputs;

// ReSharper disable InconsistentNaming
// ReSharper disable MemberCanBePrivate.Global
// ReSharper disable PropertyCanBeMadeInitOnly.Global
#pragma warning disable CA1822 // ReSharper disable MemberCanBeMadeStatic.Global

public class UApplicationInfo {

    public string ExportApp => "YaeAchievement";

    public string ExportAppVersion => GlobalVars.AppVersionName;

    public   long ExportTimestamp => DateTimeOffset.Now.ToUnixTimeSeconds();

    public string UIAFVersion => "v1.1";

}

public class UAchievementInfo {

    public uint Id { get; set; }

    public uint Status { get; set; }

    public uint Current { get; set; }

    public uint Timestamp { get; set; }

}

public class UIAFRoot {

    public UApplicationInfo Info => new ();

    public IEnumerable<UAchievementInfo> List { get; set; } = null!;

    public static UIAFRoot FromNotify(AchievementAllDataNotify ntf) => new () {
        List = ntf.AchievementList
            .Where(a => a.Status >= AchievementStatus.Finished || a.CurrentProgress > 0)
            .Select(a => new UAchievementInfo {
                Id = a.Id,
                Status = (uint) a.Status,
                Current = a.CurrentProgress,
                Timestamp = a.FinishTimestamp
            })
    };

}

[JsonSerializable(typeof(UIAFRoot))]
[JsonSourceGenerationOptions(
    GenerationMode = JsonSourceGenerationMode.Serialization,
    PropertyNamingPolicy = JsonKnownNamingPolicy.SnakeCaseLower
)]
public partial class UIAFSerializer : JsonSerializerContext {

    public static string Serialize(AchievementAllDataNotify ntf) {
        return JsonSerializer.Serialize(Outputs.UIAFRoot.FromNotify(ntf), Default.UIAFRoot);
    }
}
