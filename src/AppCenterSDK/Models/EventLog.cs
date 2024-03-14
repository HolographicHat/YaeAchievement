using YaeAchievement.AppCenterSDK.Models.Serialization;

namespace YaeAchievement.AppCenterSDK.Models; 

[LogId(JsonIdentifier)]
public class EventLog(string name) : LogWithProperties {
    
    public const string JsonIdentifier = "event";

    public Guid Id { get; set; } = Guid.NewGuid();

    public string Name { get; set; } = name;
    
}
