using YaeAchievement.AppCenterSDK.Models.Serialization;

namespace YaeAchievement.AppCenterSDK.Models; 

[LogId(JsonIdentifier)]
public class HandledErrorLog : LogWithProperties {
    
    public const string JsonIdentifier = "handledError";

    public HandledErrorLog(MException exception) {
        Id = Guid.NewGuid();
        Exception = exception;
    }

    public Guid? Id { get; set; }
    
    public MException Exception { get; set; }
    
}
