using System.Text.Json.Serialization;

namespace YaeAchievement.AppCenterSDK.Models; 

public abstract class Log {

    [JsonPropertyName("sid")]
    public Guid? Session { get; set; } = AppCenter.SessionID;
    
    public DateTime Timestamp { get; set; } = DateTime.UtcNow;
    
    public Device Device { get; set; } = AppCenter.DeviceInfo;

    [JsonIgnore]
    internal LogStatus Status = LogStatus.Pending;

}

public enum LogStatus {
    Pending, Uploading, Uploaded
}
