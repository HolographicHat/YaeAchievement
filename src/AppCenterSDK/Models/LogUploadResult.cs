using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

public class LogUploadResult {
    
    [JsonProperty(PropertyName = "status")]
    public string Status { get; set; } = null!;

    [JsonProperty(PropertyName = "validDiagnosticsIds")]
    public Guid[] ValidDiagnosticsIds { get; set; } = Array.Empty<Guid>();

    [JsonProperty(PropertyName = "throttledDiagnosticsIds")]
    public Guid[] ThrottledDiagnosticsIds { get; set; } = Array.Empty<Guid>();

    [JsonProperty(PropertyName = "correlationId")]
    public Guid CorrelationId { get; set; }

}
