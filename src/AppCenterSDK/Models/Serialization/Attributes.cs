namespace YaeAchievement.AppCenterSDK.Models.Serialization;

[AttributeUsage(AttributeTargets.Class)]
public sealed class LogIdAttribute(string id) : Attribute {
    
    public string Id { get; } = id;
    
}
