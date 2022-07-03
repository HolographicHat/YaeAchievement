// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System.Reflection;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace YaeAchievement.AppCenterSDK.Models.Serialization;

#pragma warning disable CS8604, CS8765
public class LogJsonConverter : JsonConverter {
    
    private readonly Dictionary<string, Type> _logTypes = new ();
    private readonly object _jsonConverterLock = new ();
    private static readonly JsonSerializerSettings SerializationSettings;

    static LogJsonConverter() {
        SerializationSettings = new JsonSerializerSettings {
            Formatting = Formatting.None,
            NullValueHandling = NullValueHandling.Ignore,
            DateFormatHandling = DateFormatHandling.IsoDateFormat,
            DateTimeZoneHandling = DateTimeZoneHandling.Utc,
            ReferenceLoopHandling = ReferenceLoopHandling.Serialize
        };
    }

    public void AddLogType(string typeName, Type type) {
        lock (_jsonConverterLock) {
            _logTypes[typeName] = type;
        }
    }

    public override bool CanConvert(Type objectType) {
        return typeof(Log).IsAssignableFrom(objectType);
    }

    public override object? ReadJson(JsonReader reader, Type t, object o, JsonSerializer s) {
        Type logType;
        var jsonObject = JObject.Load(reader);
        var typeName = jsonObject.GetValue("type")?.ToString();
        lock (_jsonConverterLock) {
            if (typeName == null || !_logTypes.ContainsKey(typeName)) {
                throw new JsonReaderException("Could not identify type of log");
            }
            logType = _logTypes[typeName];
            jsonObject.Remove("type");
        }
        return jsonObject.ToObject(logType);
    }

    public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer) {
        var info = value.GetType().GetTypeInfo();
        if (info.GetCustomAttribute(typeof(JsonObjectAttribute)) is not JsonObjectAttribute attribute) {
            throw new JsonWriterException("Log type is missing JsonObjectAttribute");
        }
        var jsonObject = JObject.FromObject(value, JsonSerializer.CreateDefault(SerializationSettings));
        jsonObject.Add("type", JToken.FromObject(attribute.Id));
        writer.WriteRawValue(jsonObject.ToString(Formatting.None));
    }
}
