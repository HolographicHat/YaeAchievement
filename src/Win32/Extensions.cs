using System.ComponentModel;
using YaeAchievement.AppCenterSDK;

namespace YaeAchievement.Win32; 

public static class Extensions {
    
    public static int PrintMsgAndReturnErrCode(this Win32Exception ex, string msg) {
        Console.WriteLine($"{msg}: {ex.Message}");
        AppCenter.TrackCrash(ex, false);
        return ex.NativeErrorCode;
    }

}