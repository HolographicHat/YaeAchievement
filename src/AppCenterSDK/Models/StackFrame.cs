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

    public string Address { get; set; }

    public string Code { get; set; }

    public string ClassName { get; set; }

    public string MethodName { get; set; }

    public int? LineNumber { get; set; }

    public string FileName { get; set; }
    
}
