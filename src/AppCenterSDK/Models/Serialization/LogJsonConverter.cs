using System.Reflection;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace YaeAchievement.AppCenterSDK.Models.Serialization;

public class GuidConverter : JsonConverter<Guid> {

    public static GuidConverter Instance { get; } = new ();
    
    public override Guid Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options) {
        throw new NotSupportedException();
    }

    public override void Write(Utf8JsonWriter writer, Guid value, JsonSerializerOptions options) {
        writer.WriteStringValue(value.ToString("D"));
    }
}

public class LogJsonConverter : JsonConverter<Log> {
    
    public static LogJsonConverter Instance { get; } = new ();

    private static readonly JsonSerializerOptions SerializationSettings;

    static LogJsonConverter() {
        SerializationSettings = new JsonSerializerOptions {
            DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull,
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            ReferenceHandler = ReferenceHandler.IgnoreCycles,
        };
    }

    public override bool CanConvert(Type objectType) => typeof(Log).IsAssignableFrom(objectType);

    public override Log Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options) {
        throw new NotSupportedException();
    }

    public override void Write(Utf8JsonWriter writer, Log value, JsonSerializerOptions options) {
        var attr = value.GetType().GetCustomAttribute<LogIdAttribute>();
        if (attr == null) {
            throw new JsonException("Log type is missing LogTypeAttribute");
        }
        var cNode = JsonSerializer.SerializeToNode((object) value, SerializationSettings)!;
        cNode["type"] = attr.Id;
        writer.WriteRawValue(cNode.ToString());
    }
}
