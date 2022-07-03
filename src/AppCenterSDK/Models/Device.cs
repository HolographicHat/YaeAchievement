using System.Globalization;
using System.Reflection;
using Newtonsoft.Json;

namespace YaeAchievement.AppCenterSDK.Models;

public class Device {
    
    [JsonProperty(PropertyName = "sdkName")]
    public string SdkName { get; set; } = "appcenter.wpf.netcore";

    [JsonProperty(PropertyName = "sdkVersion")]
    public string SdkVersion { get; set; } = "4.5.0";

    [JsonProperty(PropertyName = "osName")]
    public string OsName { get; set; } = "WINDOWS";

    [JsonProperty(PropertyName = "osVersion")]
    public string OsVersion { get; set; } = DeviceHelper.GetSystemVersion();

    [JsonProperty(PropertyName = "osBuild")]
    public string OsBuild { get; set; } = $"{DeviceHelper.GetSystemVersion()}.{DeviceHelper.GetSystemBuild()}";

    [JsonProperty(PropertyName = "model")] 
    public string? Model { get; set; } = DeviceHelper.GetModel();

    [JsonProperty(PropertyName = "oemName")]
    public string? OemName { get; set; } = DeviceHelper.GetOem();

    [JsonProperty(PropertyName = "screenSize")]
    public string ScreenSize { get; set; } = DeviceHelper.GetScreenSize();

    [JsonProperty(PropertyName = "carrierCountry")]
    public string Country { get; set; } = DeviceHelper.GetCountry() ?? "CN";

    [JsonProperty(PropertyName = "locale")]
    public string Locale { get; set; } = CultureInfo.CurrentCulture.Name;

    [JsonProperty(PropertyName = "timeZoneOffset")]
    public int TimeZoneOffset { get; set; } = (int) TimeZoneInfo.Local.BaseUtcOffset.TotalMinutes;

    [JsonProperty(PropertyName = "appVersion")]
    public string AppVersion { get; set; } = GlobalVars.AppVersionName;

    [JsonProperty(PropertyName = "appBuild")]
    public string AppBuild { get; set; } = GlobalVars.AppVersionCode.ToString();

    [JsonProperty(PropertyName = "appNamespace")]
    public string AppNamespace { get; set; } = Assembly.GetEntryAssembly()?.EntryPoint?.DeclaringType?.Namespace ?? string.Empty;
    
}
