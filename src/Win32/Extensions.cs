using System.ComponentModel;

namespace YaeAchievement.Win32; 

public static class Extensions {
    
    public static int PrintMsgAndReturnErrCode(this Win32Exception ex, string msg) {
        Console.WriteLine($"{msg}: {ex.Message}");
        if (GlobalVars.Verbose) {
            Console.WriteLine(ex.StackTrace);
        }
        return ex.NativeErrorCode;
    }

}