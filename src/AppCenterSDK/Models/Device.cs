using System.Globalization;
using System.Reflection;

namespace YaeAchievement.AppCenterSDK.Models;

public class Device {
    
    public string SdkName { get; set; } = "appcenter.wpf.netcore";

    public string SdkVersion { get; set; } = "4.5.0";

    public string OsName { get; set; } = "WINDOWS";

    public string OsVersion { get; set; } = DeviceHelper.GetSystemVersion();

    public string OsBuild { get; set; } = $"{DeviceHelper.GetSystemVersion()}.{DeviceHelper.GetSystemBuild()}";

    public string? Model { get; set; } = DeviceHelper.GetModel();

    public string? OemName { get; set; } = DeviceHelper.GetOem();

    public string ScreenSize { get; set; } = DeviceHelper.GetScreenSize();

    public string CarrierCountry { get; set; } = DeviceHelper.GetCountry() ?? "CN";

    public string Locale { get; set; } = CultureInfo.CurrentCulture.Name;

    public int TimeZoneOffset { get; set; } = (int) TimeZoneInfo.Local.BaseUtcOffset.TotalMinutes;

    public string AppVersion { get; set; } = GlobalVars.AppVersionName;

    public string AppBuild { get; set; } = GlobalVars.AppVersionCode.ToString();

    public string AppNamespace { get; set; } = Assembly.GetEntryAssembly()?.EntryPoint?.DeclaringType?.Namespace ?? string.Empty;
    
}
