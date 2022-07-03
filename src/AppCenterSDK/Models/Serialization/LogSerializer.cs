// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models.Serialization;

#pragma warning disable CS8604, CS8765, CS8603
public static class LogSerializer {
    
    private static readonly JsonSerializerSettings SerializationSettings;
    private static readonly LogJsonConverter Converter = new ();

    static LogSerializer() {
        SerializationSettings = new JsonSerializerSettings {
            Formatting = Formatting.None,
            NullValueHandling = NullValueHandling.Ignore,
            DateFormatHandling = DateFormatHandling.IsoDateFormat,
            DateTimeZoneHandling = DateTimeZoneHandling.Utc,
            ReferenceLoopHandling = ReferenceLoopHandling.Serialize,
            Converters = { Converter }
        };
    }

    public static void AddLogType(string typeName, Type type) {
        Converter.AddLogType(typeName, type);
    }
    
    public static string Serialize(LogContainer logContainer) {
        return JsonConvert.SerializeObject(logContainer, SerializationSettings);
    }

    public static string Serialize(Log log) {
        return JsonConvert.SerializeObject(log, SerializationSettings);
    }
    
    public static LogContainer? DeserializeLogs(string json) {
        return JsonConvert.DeserializeObject<LogContainer>(json, SerializationSettings);
    }
}
