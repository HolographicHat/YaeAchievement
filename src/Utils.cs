using System.ComponentModel;
using System.Diagnostics;
using System.IO.Pipes;
using System.Net;
using System.Net.Http.Headers;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using YaeAchievement.AppCenterSDK;
using YaeAchievement.res;
using YaeAchievement.Win32;
using static YaeAchievement.Win32.OpenFileFlags;

namespace YaeAchievement;

public static class Utils {

    public static readonly Lazy<HttpClient> CHttpClient = new (() => {
        var c = new HttpClient(new HttpClientHandler {
            Proxy = GlobalVars.DebugProxy ? new WebProxy("http://127.0.0.1:8888") : null,
            AutomaticDecompression = DecompressionMethods.Brotli | DecompressionMethods.GZip
        }) {
            DefaultRequestHeaders = {
                UserAgent = {
                    new ProductInfoHeaderValue("YaeAchievement", GlobalVars.AppVersion.ToString(2))
                }
            }
        };
        return c;
    });

    public static byte[] GetBucketFileAsByteArray(string path, bool cache = true) {
        using var msg = new HttpRequestMessage {
            Method = HttpMethod.Get,
            RequestUri = new Uri($"{GlobalVars.BucketHost}/{path}")
        };
        var cacheFile = new CacheFile(path);
        if (cache && cacheFile.Exists()) {
            msg.Headers.TryAddWithoutValidation("If-None-Match", $"{cacheFile.Read().Etag}");
        }
        using var response = CHttpClient.Value.Send(msg);
        if (cache && response.StatusCode == HttpStatusCode.NotModified) {
            return cacheFile.Read().Content.ToByteArray();
        }
        response.EnsureSuccessStatusCode();
        var responseBytes = response.Content.ReadAsByteArrayAsync().Result;
        if (cache) {
            var etag = response.Headers.ETag!.Tag;
            cacheFile.Write(responseBytes, etag);
        }
        return responseBytes;
    }

    public static void CopyToClipboard(string text) {
        if (Native.OpenClipboard(IntPtr.Zero)) {
            Native.EmptyClipboard();
            var hGlobal = Marshal.AllocHGlobal((text.Length + 1) * 2);
            var hPtr = Native.GlobalLock(hGlobal);
            Marshal.Copy(text.ToCharArray(), 0, hPtr, text.Length);
            Native.GlobalUnlock(hPtr);
            Native.SetClipboardData(13, hGlobal);
            Marshal.FreeHGlobal(hGlobal);
            Native.CloseClipboard();
        } else {
            throw new Win32Exception();
        }
    }
    
    public static void CheckUpdate() {
        var info = UpdateInfo.Parser.ParseFrom(GetBucketFileAsByteArray("schicksal/version"))!;
        if (GlobalVars.AppVersionCode != info.VersionCode) {
            Console.WriteLine(App.UpdateNewVersion, GlobalVars.AppVersionName, info.VersionName);
            Console.WriteLine(App.UpdateDescription, info.Description);
            if (info.EnableAutoDownload) {
                Console.WriteLine(App.UpdateDownloading);
                var fullPath = Path.GetFullPath($"update.{Path.GetExtension(info.PackageLink)}");
                File.WriteAllBytes(fullPath, GetBucketFileAsByteArray(info.PackageLink));
                Console.WriteLine(App.UpdateDownloadFinish);
                ShellOpen(fullPath);
                Environment.Exit(0);
            }
            Console.WriteLine(App.DownloadLink, info.PackageLink);
            if (info.ForceUpdate) {
                Environment.Exit(0);
            }
        }
        if (info.EnableLibDownload) {
            File.WriteAllBytes(GlobalVars.LibName, GetBucketFileAsByteArray("schicksal/lib.dll"));
        }
    }
    
    public static void CheckSelfIsRunning() {
        Process.EnterDebugMode(); 
        var cur = Process.GetCurrentProcess();
        foreach (var process in Process.GetProcesses().Where(process => process.Id != cur.Id)) {
            if (process.ProcessName == cur.ProcessName) {
                Console.WriteLine(App.AnotherInstance);
                Environment.Exit(302);
            }
        }
        Process.LeaveDebugMode();
    }

    public static bool ShellOpen(string path) {
        return new Process {
            StartInfo = {
                FileName = path,
                UseShellExecute = true
            }
        }.Start();
    }

    public static bool CheckGamePathValid(string? path) {
        if (path == null) return false;
        var dir = Path.GetDirectoryName(path)!;
        return !GlobalVars.CheckGamePath || File.Exists(Path.Combine(dir, "UnityPlayer.dll"));
    }
    
    public static string SelectGameExecutable() {
        var fnPtr = Marshal.AllocHGlobal(32768);
        Native.RtlZeroMemory(fnPtr, 32768);
        var ofn = new OpenFileName {
            file    = fnPtr,
            size    = Marshal.SizeOf<OpenFileName>(),
            owner   = Native.GetConsoleWindow(),
            flags   = Explorer | NoNetworkButton | FileMustExist | NoChangeDir,
            title   = App.SelectTitle,
            filter  = $"{App.SelectFilterName} (YuanShen/GenshinImpact.exe)\0YuanShen.exe;GenshinImpact.exe\0",
            maxFile = 32768
        };
        new Thread(() => {
            var handle = Native.FindWindow("#32770", App.SelectTitle);
            while (handle == IntPtr.Zero) {
                handle = Native.FindWindow("#32770", App.SelectTitle);
                Thread.Sleep(1);
            }
            var currentThreadId = Native.GetCurrentThreadId();
            var foregroundThreadId = Native.GetWindowThreadProcessId(Native.GetForegroundWindow(), out _);
            Native.AttachThreadInput(currentThreadId, foregroundThreadId, true);
            Native.SetWindowPos(handle, new IntPtr(-1), 0, 0, 0, 0, 1 | 2);
            Native.SetForegroundWindow(handle);
            Native.AttachThreadInput(currentThreadId, foregroundThreadId, false);
        }).Start();
        if(!Native.GetOpenFileName(ofn)) {
            var err = Native.CommDlgExtendedError();
            if (err != 0) {
                throw new SystemException($"Dialog error: {err}");
            }
            Console.WriteLine(App.SelectCanceled);
            Environment.Exit(0);
        }
        var path = Marshal.PtrToStringAuto(fnPtr)!;
        Marshal.FreeHGlobal(fnPtr);
        return path;
    }

    // ReSharper disable once UnusedMethodReturnValue.Global
    public static bool TryDisableQuickEdit() {
        var handle = Native.GetStdHandle();
        return Native.GetConsoleMode(handle, out var mode) && Native.SetConsoleMode(handle, mode&~64);
    }

    public static void CheckGenshinIsRunning() {
        Process.EnterDebugMode();
        foreach (var process in Process.GetProcesses()) {
            if (process.ProcessName is "GenshinImpact" or "YuanShen" && !process.HasExited) {
                Console.WriteLine(App.GenshinIsRunning, process.Id);
                Environment.Exit(301);
            }
        }
        Process.LeaveDebugMode();
    }
    
    // ReSharper disable once InconsistentNaming
    private static Process? proc;
    
    public static void InstallExitHook() {
        AppDomain.CurrentDomain.ProcessExit += (_, _) => {
            proc?.Kill();
            Console.WriteLine(App.PressKeyToExit);
            Console.ReadKey();
        };
    }

    public static void InstallExceptionHook() {
        AppDomain.CurrentDomain.UnhandledException += (_, e) => {
            Console.WriteLine(e.ExceptionObject.ToString());
            Console.WriteLine(App.UploadError);
            AppCenter.TrackCrash((Exception) e.ExceptionObject);
            AppCenter.Upload();
            Environment.Exit(-1);
        };
    }
    
    // ReSharper disable once UnusedMethodReturnValue.Global
    public static Thread StartAndWaitResult(string exePath, Func<string, bool> onReceive) {
        const string lib = "C:/ProgramData/yae.dll";
        File.Copy(Path.GetFullPath(GlobalVars.LibName), lib, true);
        AppDomain.CurrentDomain.ProcessExit += (_, _) => {
            File.Delete(lib);
        };
        if (!Injector.CreateProcess(exePath, out var hProcess, out var hThread, out var pid)) {
            Environment.Exit(new Win32Exception().PrintMsgAndReturnErrCode("ICreateProcess fail"));
        }
        if (Injector.LoadLibraryAndInject(hProcess, lib) != 0) {
            if (!Native.TerminateProcess(hProcess, 0)) {
                Environment.Exit(new Win32Exception().PrintMsgAndReturnErrCode("TerminateProcess fail"));
            }
        }
        Console.WriteLine(App.GameLoading, pid);
        proc = Process.GetProcessById(Convert.ToInt32(pid));
        proc.EnableRaisingEvents = true;
        proc.Exited += (_, _) => {
            if (GlobalVars.UnexpectedExit) {
                proc = null;
                Console.WriteLine(App.GameProcessExit);
                Environment.Exit(114514);
            }
        };
        if (Native.ResumeThread(hThread) == 0xFFFFFFFF) {
            var e = new Win32Exception();
            if (!Native.TerminateProcess(hProcess, 0)) {
                new Win32Exception().PrintMsgAndReturnErrCode("TerminateProcess fail");
            }
            Environment.Exit(e.PrintMsgAndReturnErrCode("ResumeThread fail"));
        }
        if (!Native.CloseHandle(hProcess)) {
            Environment.Exit(new Win32Exception().PrintMsgAndReturnErrCode("CloseHandle fail"));
        }
        var ts = new ThreadStart(() => {
            var server = new NamedPipeServerStream(GlobalVars.PipeName);
            server.WaitForConnection();
            using var reader = new StreamReader(server);
            while (!proc.HasExited) {
                var line = reader.ReadLine();
                if (line?.Length > 0) {
                    if (onReceive(line)) {
                        break;
                    }
                    server.Disconnect();
                    server.WaitForConnection();
                }
            }
        });
        var th = new Thread(ts);
        th.Start();
        return th;
    }
    
    #pragma warning disable CA1416
    /// <summary>
    /// 从注册表中寻找安装路径 暂时只支持国服
    /// </summary>
    /// <returns></returns>
    public static string? FindGamePathFromRegistry() {
        try {
            using var root = Registry.LocalMachine;
            using var sub = root.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\原神");
            if (sub == null) {
                return null;
            }
            var installLocation = sub.GetValue("InstallPath")?.ToString();
            if (!string.IsNullOrEmpty(installLocation)) {
                var folder = Path.Combine(installLocation, "Genshin Impact Game\\");
                var exePath = Path.Combine(folder, "YuanShen.exe");
                if (File.Exists(Path.Combine(folder, "UnityPlayer.dll")) && File.Exists(exePath)) {
                    return exePath;
                }
            }
        } catch (Exception e) {
            Console.WriteLine(e.Message);
        }
        return null;
    }

    public static void CheckVcRuntime() {
        using var root = Registry.LocalMachine;
        using var sub = root.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall")!;
        var installed = sub.GetSubKeyNames()
            .Select(subKeyName => sub.OpenSubKey(subKeyName))
            .Select(item => item?.GetValue("DisplayName") as string ?? string.Empty)
            .Any(name => name.Contains("Microsoft Visual C++ 2022 X64 "));
        if (!installed) {
            const string vcDownloadUrl = "https://aka.ms/vs/17/release/vc_redist.x64.exe";
            Console.WriteLine(App.VcRuntimeNotInstalled);
            Console.WriteLine(App.DownloadLink, vcDownloadUrl);
            Console.WriteLine(App.VcRuntimeAfterInstall);
            ShellOpen(vcDownloadUrl);
            Environment.Exit(303);
        }
    }
}
