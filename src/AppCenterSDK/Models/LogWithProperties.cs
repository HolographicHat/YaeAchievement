using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

public class LogWithProperties : Log {
    
    [JsonProperty(PropertyName = "properties")]
    public IDictionary<string, string> Properties { get; set; } = new Dictionary<string, string>();

}
