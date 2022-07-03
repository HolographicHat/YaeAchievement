using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

[JsonObject(JsonIdentifier)]
public class EventLog : LogWithProperties {
    
    public const string JsonIdentifier = "event";

    public EventLog(string name) {
        Name = name;
    }

    [JsonProperty(PropertyName = "id")]
    private Guid Id { get; set; } = Guid.NewGuid();

    [JsonProperty(PropertyName = "name")]
    private string Name { get; set; }

}
