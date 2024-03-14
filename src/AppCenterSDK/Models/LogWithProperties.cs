namespace YaeAchievement.AppCenterSDK.Models; 

public class LogWithProperties : Log {
    
    public IDictionary<string, string> Properties { get; set; } = new Dictionary<string, string>();

}
