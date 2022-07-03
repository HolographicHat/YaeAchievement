using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

[JsonObject(JsonIdentifier)]
public class HandledErrorLog : LogWithProperties {
    
    public const string JsonIdentifier = "handledError";

    public HandledErrorLog(MException exception) {
        Id = Guid.NewGuid();
        Exception = exception;
    }

    [JsonProperty(PropertyName = "id")]
    public Guid? Id { get; set; }
    
    [JsonProperty(PropertyName = "exception")]
    public MException Exception { get; set; }
    
}
