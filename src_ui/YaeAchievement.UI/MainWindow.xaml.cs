using System;
using System.Windows;
using YaeAchievement.AppCenterSDK;
using YaeAchievement.AppCenterSDK.Models;
using static YaeAchievement.Utils;

namespace YaeAchievement.UI;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        Title = $"YaeAchievement {GlobalVars.AppVersionName}";
        InitializeComponent();
    }

    private async void OnWindowLoaded(object sender, RoutedEventArgs e)
    {
        // Don't pause.
        GlobalVars.PauseOnExit = false;
        try
        {
            InstallExitHook();
            InstallExceptionHook();

            // dangerous await in async void, so we catch in this method
            await CheckVcRuntime();

            CheckSelfIsRunning();
            // TryDisableQuickEdit();
            CheckGenshinIsRunning();

            AppConfig.Load("auto");
            Export.ExportTo = uint.MaxValue;
#if RELEASE
            CheckUpdate(false);
#endif
            AppCenter.Init();
            new EventLog("AppInit")
            {
                Properties =
                {
                    { "AppVersion", GlobalVars.AppVersionName },
                    { "SystemVersion", DeviceHelper.GetSystemVersion() }
                }
            }.Enqueue();
        }
        catch(Exception ex)
        {
            _ = MessageBox.Show(ex.Message, "Exception Occurred!", MessageBoxButton.OK);
        }
    }

    private void OnWindowClosed(object sender, EventArgs e)
    {

    }

    private void OnExportButtonClick(object sender, RoutedEventArgs e)
    {
        ExportTargetRadioButtons.IsEnabled = false;
        Export.ExportTo = (uint)(ExportTargetRadioButtons.SelectedIndex + 1);

        var historyCache = new CacheFile("ExportData");
        if (historyCache.LastWriteTime.AddMinutes(10) > DateTime.UtcNow)
        {
            //Console.WriteLine(GetResourceString("UsePreviousData"));
            //Console.WriteLine(GetResourceString("RefreshData"));
            Export.Choose(AchievementAllDataNotify.Parser.ParseFrom(historyCache.Read().Content));
        }
        else
        {
            StartAndWaitResult(AppConfig.GamePath, str => {
                GlobalVars.UnexpectedExit = false;
                var data = Convert.FromBase64String(str);
                var list = AchievementAllDataNotify.Parser.ParseFrom(data);
                historyCache.Write(data);
                Export.Choose(list);
                return true;
            });
        }
    }
}
