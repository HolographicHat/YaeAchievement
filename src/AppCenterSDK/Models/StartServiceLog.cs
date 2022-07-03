using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

[JsonObject(JsonIdentifier)]
public class StartServiceLog : Log {
    
    public const string JsonIdentifier = "startService";
    
    public StartServiceLog(params string[] services) {
        Services = services;
    }
    
    [JsonProperty(PropertyName = "services")]
    public string[] Services { get; set; }
    
}
