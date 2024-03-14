namespace YaeAchievement.AppCenterSDK.Models; 

public class LogUploadResult {
    
    public string Status { get; set; } = null!;

    public Guid[] ValidDiagnosticsIds { get; set; } = Array.Empty<Guid>();

    public Guid[] ThrottledDiagnosticsIds { get; set; } = Array.Empty<Guid>();

    public Guid CorrelationId { get; set; }

}
