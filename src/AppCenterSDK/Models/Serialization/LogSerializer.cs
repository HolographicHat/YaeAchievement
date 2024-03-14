using System.Text.Json;
using System.Text.Json.Serialization;

namespace YaeAchievement.AppCenterSDK.Models.Serialization;

public static class LogSerializer {
    
    private static readonly JsonSerializerOptions SerializationSettings;

    static LogSerializer() {
        SerializationSettings = new JsonSerializerOptions {
            DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull,
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            ReferenceHandler = ReferenceHandler.IgnoreCycles,
            Converters = {
                GuidConverter.Instance,
                LogJsonConverter.Instance,
            }
        };
    }
    
    public static string Serialize(LogContainer logContainer) {
        return JsonSerializer.Serialize(logContainer, SerializationSettings);
    }
}
