using System.Text.Json;
using System.Text.Json.Serialization;
using YaeAchievement.Parsers;

namespace YaeAchievement.Outputs;

// ReSharper disable MemberCanBePrivate.Global
// ReSharper disable PropertyCanBeMadeInitOnly.Global
#pragma warning disable CA1822 // ReSharper disable MemberCanBeMadeStatic.Global

public class SeelieRoot {
    
    public class AchievementFinishStatus {

        public bool Done => true;

    }

    public Dictionary<uint, AchievementFinishStatus> Achievements { get; set; } = null!;

    public static SeelieRoot FromNotify(AchievementAllDataNotify ntf) => new () {
        Achievements = ntf.AchievementList
            .Where(a => a.Status >= AchievementStatus.Finished)
            .OrderBy(a => a.Id)
            .ToDictionary(a => a.Id, _ => new AchievementFinishStatus())
    };
}

[JsonSerializable(typeof(SeelieRoot))]
[JsonSourceGenerationOptions(
    GenerationMode = JsonSourceGenerationMode.Serialization,
    PropertyNamingPolicy = JsonKnownNamingPolicy.SnakeCaseLower
)]
public partial class SeelieSerializer : JsonSerializerContext {

    public static string Serialize(AchievementAllDataNotify ntf) {
        return JsonSerializer.Serialize(Outputs.SeelieRoot.FromNotify(ntf), Default.SeelieRoot);
    }
}
