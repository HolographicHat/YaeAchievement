using System.Text.Json;
using System.Text.Json.Serialization;
using YaeAchievement.Parsers;

namespace YaeAchievement.Outputs;

// ReSharper disable MemberCanBePrivate.Global
// ReSharper disable PropertyCanBeMadeInitOnly.Global
#pragma warning disable CA1822 // ReSharper disable MemberCanBeMadeStatic.Global

public class PaimonRoot {
    
    public Dictionary<uint, Dictionary<uint, bool>> Achievement { get; set; } = null!;

    public static PaimonRoot FromNotify(AchievementAllDataNotify ntf) {
        var info = GlobalVars.AchievementInfo.Items.ToDictionary(pair => pair.Key, pair => pair.Value.Group);
        return new PaimonRoot {
            Achievement = ntf.AchievementList
                .Where(a => a.Status >= AchievementStatus.Finished && info.ContainsKey(a.Id))
                .GroupBy(a => info[a.Id], a => a.Id)
                .OrderBy(g => g.Key)
                .ToDictionary(g => g.Key, g => g.ToDictionary(id => id, _ => true))
        };
    }
}

[JsonSerializable(typeof(PaimonRoot))]
[JsonSourceGenerationOptions(
    GenerationMode = JsonSourceGenerationMode.Serialization,
    PropertyNamingPolicy = JsonKnownNamingPolicy.SnakeCaseLower
)]
public partial class PaimonSerializer : JsonSerializerContext {

    public static string Serialize(AchievementAllDataNotify ntf) {
        return JsonSerializer.Serialize(Outputs.PaimonRoot.FromNotify(ntf), Default.PaimonRoot);
    }
}
