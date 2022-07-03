using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

public class MException {

    [JsonProperty(PropertyName = "type")]
    public string Type { get; set; } = "UnknownType";

    [JsonProperty(PropertyName = "message")]
    public string? Message { get; set; }

    [JsonProperty(PropertyName = "stackTrace")]
    public string? StackTrace { get; set; }

    [JsonProperty(PropertyName = "frames")]
    public IList<StackFrame>? Frames { get; set; }

    [JsonProperty(PropertyName = "innerExceptions")]
    public IList<MException>? InnerExceptions { get; set; }

}
