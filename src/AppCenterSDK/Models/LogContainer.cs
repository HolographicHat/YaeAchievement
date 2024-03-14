namespace YaeAchievement.AppCenterSDK.Models; 

public class LogContainer {
    
    public LogContainer(IEnumerable<Log> logs) {
        Logs = logs;
    }

    public IEnumerable<Log> Logs { get; set; }
    
}
