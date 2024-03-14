using System.Diagnostics;
using YaeAchievement.AppCenterSDK.Models.Serialization;

namespace YaeAchievement.AppCenterSDK.Models; 

[LogId(JsonIdentifier)]
public class ManagedErrorLog : Log {
    
    public const string JsonIdentifier = "managedError";
    
    public ManagedErrorLog(
        Exception exception,
        bool fatal = true
    ) {
        var p = Process.GetCurrentProcess();
        Id = Guid.NewGuid();
        Fatal = fatal;
        UserId = AppCenter.DeviceID;
        ProcessId = p.Id;
        Exception = ErrorLogHelper.CreateModelExceptionAndBinaries(exception);
        ProcessName = p.ProcessName;
        Architecture = Environment.GetEnvironmentVariable("PROCESSOR_ARCHITECTURE");
        AppLaunchTimestamp = p.StartTime.ToUniversalTime();
    }

    public Guid Id { get; set; }

    public string? UserId { get; set; }

    public int ProcessId { get; set; }

    public string ProcessName { get; set; }

    public bool Fatal { get; set; }

    public DateTime? AppLaunchTimestamp { get; set; }

    public string? Architecture { get; set; }

    public MException Exception { get; set; }

}
