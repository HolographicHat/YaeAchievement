using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

[JsonObject(JsonIdentifier)]
public class PageLog : LogWithProperties {
    
    public const string JsonIdentifier = "page";

    public PageLog(string name) {
        Name = name;
    }

    [JsonProperty(PropertyName = "name")]
    public string Name { get; set; }
    
}
