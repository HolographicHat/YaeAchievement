using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models; 

public class StackFrame {
    
    public StackFrame(string address, string code, string className, string methodName, int? lineNumber, string fileName) {
        Address = address;
        Code = code;
        ClassName = className;
        MethodName = methodName;
        LineNumber = lineNumber;
        FileName = fileName;
    }

    [JsonProperty(PropertyName = "address")]
    public string Address { get; set; }

    [JsonProperty(PropertyName = "code")]
    public string Code { get; set; }

    [JsonProperty(PropertyName = "className")]
    public string ClassName { get; set; }

    [JsonProperty(PropertyName = "methodName")]
    public string MethodName { get; set; }

    [JsonProperty(PropertyName = "lineNumber")]
    public int? LineNumber { get; set; }

    [JsonProperty(PropertyName = "fileName")]
    public string FileName { get; set; }
    
}
