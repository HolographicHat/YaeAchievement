using YaeAchievement.AppCenterSDK.Models.Serialization;

namespace YaeAchievement.AppCenterSDK.Models; 

[LogId(JsonIdentifier)]
public class PageLog : LogWithProperties {
    
    public const string JsonIdentifier = "page";

    public PageLog(string name) {
        Name = name;
    }

    public string Name { get; set; }
    
}
