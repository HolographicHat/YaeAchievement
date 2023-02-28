using Microsoft.Win32;
using Windows.Win32;
using Windows.Win32.Foundation;
using Windows.Win32.Graphics.Gdi;

namespace YaeAchievement.AppCenterSDK;

#pragma warning disable CA1416
public static class DeviceHelper {

    public static string? GetOem() {
        using var root = Registry.LocalMachine;
        using var sub = root.OpenSubKey("HARDWARE\\DESCRIPTION\\System\\BIOS");
        var oem = sub?.GetValue("SystemManufacturer") as string;
        return oem == "System manufacturer" ? null : oem;
    }
    
    public static string? GetModel() {
        using var root = Registry.LocalMachine;
        using var sub = root.OpenSubKey("HARDWARE\\DESCRIPTION\\System\\BIOS");
        var model = sub?.GetValue("SystemProductName") as string;
        return model == "System Product Name" ? null : model;
    }
    
    public static string GetScreenSize() {
        var desktop = Native.GetDC(HWND.Null);
        var size = $"{Native.GetDeviceCaps(desktop, GET_DEVICE_CAPS_INDEX.DESKTOPHORZRES)}x{Native.GetDeviceCaps(desktop, GET_DEVICE_CAPS_INDEX.DESKTOPVERTRES)}";
        _ = Native.ReleaseDC(HWND.Null, desktop);
        return size;
    }
    
    public static string? GetCountry() {
        using var root = Registry.CurrentUser;
        using var sub = root.OpenSubKey("Control Panel\\International\\Geo");
        return sub?.GetValue("Name") as string;
    }

    public static string GetSystemVersion() {
        using var root = Registry.LocalMachine;
        using var sub = root.OpenSubKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
        var majorVersion = sub?.GetValue("CurrentMajorVersionNumber");
        if (majorVersion != null) {
            var minorVersion = sub?.GetValue("CurrentMinorVersionNumber", "0");
            var buildNumber = sub?.GetValue("CurrentBuildNumber", "0");
            return $"{majorVersion}.{minorVersion}.{buildNumber}";
        } else {
            var version = sub?.GetValue("CurrentVersion", "0.0");
            var buildNumber = sub?.GetValue("CurrentBuild", "0");
            return $"{version}.{buildNumber}";
        }
    }
    
    public static int GetSystemBuild() {
        using var root = Registry.LocalMachine;
        using var sub = root.OpenSubKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
        return (int) (sub?.GetValue("UBR") ?? 0);
    }
    
    // ReSharper disable once InconsistentNaming
    public static string GetDeviceID() {
        using var sdk = Registry.CurrentUser.OpenSubKey("SOFTWARE\\miHoYoSDK");
        if (sdk?.GetValue("MIHOYOSDK_DEVICE_ID") is not string id) {
            id = $"{Random.Shared.NextInt64().ToString().SHA1Hash().ToLower()}{DateTimeOffset.Now.ToUnixTimeMilliseconds()}";
            sdk?.SetValue("MIHOYOSDK_DEVICE_ID", id);
        }
        id = id.MD5Hash().ToLower();
        return $"{id[..8]}-{id[8..12]}-{id[12..16]}-{id[16..20]}-{id[20..]}";
    }
}
#pragma warning restore CA1416
