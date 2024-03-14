using YaeAchievement.AppCenterSDK.Models.Serialization;

namespace YaeAchievement.AppCenterSDK.Models; 

[LogId(JsonIdentifier)]
public class StartServiceLog : Log {
    
    public const string JsonIdentifier = "startService";
    
    public StartServiceLog(params string[] services) {
        Services = services;
    }
    
    public string[] Services { get; set; }
    
}
