using System.Diagnostics.CodeAnalysis;

namespace YaeAchievement; 

[SuppressMessage("Usage", "CA2211:非常量字段应当不可见")]
[SuppressMessage("ReSharper", "ConvertToConstant.Global")]
[SuppressMessage("ReSharper", "FieldCanBeMadeReadOnly.Global")]
public static class GlobalVars {
    
    public static bool Verbose = false;
    public static bool UnexpectedExit = true;
    public static string GamePath = "";
    
    public const string LibName = "YaeLib.dll";
    public const string PipeName = "YaeAchievementPipe";
    public const string ConfigFileName = "YaeAchievement.runtimeconfig.json";
    
}
