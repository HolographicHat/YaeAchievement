namespace YaeAchievement.AppCenterSDK.Models; 

public class MException {

    public string Type { get; set; } = "UnknownType";

    public string? Message { get; set; }

    public string? StackTrace { get; set; }

    public IList<StackFrame>? Frames { get; set; }

    public IList<MException>? InnerExceptions { get; set; }

}
