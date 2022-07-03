using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

[JsonObject(JsonIdentifier)]
public class StartSessionLog : Log {
    public const string JsonIdentifier = "startSession";
}
