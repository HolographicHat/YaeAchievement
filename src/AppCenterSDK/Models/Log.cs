using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

public abstract class Log {

    [JsonProperty(PropertyName = "sid")]
    private Guid? Session { get; set; } = AppCenter.SessionID;
    
    [JsonProperty(PropertyName = "timestamp")]
    private DateTime Timestamp { get; set; } = DateTime.UtcNow;
    
    [JsonProperty(PropertyName = "device")]
    private Device Device { get; set; } = AppCenter.DeviceInfo;

    [JsonIgnore]
    internal LogStatus Status = LogStatus.Pending;

}

public enum LogStatus {
    Pending, Uploading, Uploaded
}
