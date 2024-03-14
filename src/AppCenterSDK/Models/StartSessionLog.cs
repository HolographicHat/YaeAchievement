using YaeAchievement.AppCenterSDK.Models.Serialization;

namespace YaeAchievement.AppCenterSDK.Models; 

[LogId(JsonIdentifier)]
public class StartSessionLog : Log {
    public const string JsonIdentifier = "startSession";
}
