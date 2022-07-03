using System.Diagnostics;
using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

[JsonObject(JsonIdentifier)]
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

    [JsonProperty(PropertyName = "id")] 
    public Guid Id { get; set; }

    [JsonProperty(PropertyName = "userId")]
    public string? UserId { get; set; }

    [JsonProperty(PropertyName = "processId")]
    public int ProcessId { get; set; }

    [JsonProperty(PropertyName = "processName")]
    public string ProcessName { get; set; }

    [JsonProperty(PropertyName = "fatal")]
    public bool Fatal { get; set; }

    [JsonProperty(PropertyName = "appLaunchTimestamp")]
    public DateTime? AppLaunchTimestamp { get; set; }

    [JsonProperty(PropertyName = "architecture")]
    public string? Architecture { get; set; }

    [JsonProperty(PropertyName = "exception")]
    public MException Exception { get; set; }

}
