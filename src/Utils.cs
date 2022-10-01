using System.ComponentModel;
using System.Diagnostics;
using System.IO.Pipes;
using System.Net;
using System.Net.Http.Headers;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using YaeAchievement.AppCenterSDK;
using YaeAchievement.res;
using YaeAchievement.Win32;

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
        try {
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
        } catch (Exception e) {
            Console.WriteLine(App.NetworkError, e.Message);
            Environment.Exit(-1);
            return null!;
        }
    }

    public static T? GetOrNull<T>(this T[] array, uint index) where T : class {
        return array.Length > index ? array[index] : null;
    }

    public static uint? ToUIntOrNull(string? value) {
        return value != null ? uint.TryParse(value, out var result) ? result : null : null;
    }
    
    public static bool ToBooleanOrFalse(string? value) {
        return value != null && bool.TryParse(value, out var result) && result;
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
    
    public static void CheckUpdate(bool useLocalLib) {
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
        if (useLocalLib) {
            Console.WriteLine(@"[DEBUG] Use local native lib.");
            File.Copy(Path.Combine(GlobalVars.AppPath, "YaeAchievementLib.dll"), GlobalVars.LibFilePath, true);
        } else if (info.EnableLibDownload) {
            File.WriteAllBytes(GlobalVars.LibFilePath, GetBucketFileAsByteArray("schicksal/lib.dll"));
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

    // ReSharper disable once UnusedMethodReturnValue.Global
    public static bool ShellOpen(string path) {
        try {
            return new Process {
                StartInfo = {
                    FileName = path,
                    UseShellExecute = true
                }
            }.Start();
        } catch (Exception) {
            return false;
        }
    }

    // ReSharper disable once UnusedMethodReturnValue.Global
    public static bool TryDisableQuickEdit() {
        var handle = Native.GetStdHandle();
        return Native.GetConsoleMode(handle, out var mode) && Native.SetConsoleMode(handle, mode&~64);
    }

    public static void CheckGenshinIsRunning() {
        Process.EnterDebugMode();
        foreach (var process in Process.GetProcesses()) {
            if (process.ProcessName is "GenshinImpact" or "YuanShen" 
                && !process.HasExited 
                && process.MainWindowHandle != IntPtr.Zero
            ) {
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
            var ex = e.ExceptionObject;
            switch (ex) {
                case ApplicationException ex1:
                    Console.WriteLine(ex1.Message);
                    break;
                case SocketException ex2:
                    Console.WriteLine(App.ExceptionNetwork, nameof(SocketException), ex2.Message);
                    break;
                case HttpRequestException ex3:
                    Console.WriteLine(App.ExceptionNetwork, nameof(HttpRequestException), ex3.Message);
                    break;
                default:
                    Console.WriteLine(ex.ToString());
                    Console.WriteLine(App.UploadError);
                    AppCenter.TrackCrash((Exception) e.ExceptionObject);
                    AppCenter.Upload();
                    break;
            }
            Environment.Exit(-1);
        };
    }
    
    // ReSharper disable once UnusedMethodReturnValue.Global
    public static Thread StartAndWaitResult(string exePath, Func<string, bool> onReceive) {
        AppDomain.CurrentDomain.ProcessExit += (_, _) => {
            try { 
                File.Delete(GlobalVars.LibFilePath);
            } catch (Exception) { /* ignored */ }
        };
        if (!Injector.CreateProcess(exePath, out var hProcess, out var hThread, out var pid)) {
            Environment.Exit(new Win32Exception().PrintMsgAndReturnErrCode("ICreateProcess fail"));
        }
        if (Injector.LoadLibraryAndInject(hProcess, GlobalVars.LibFilePath) != 0) {
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

    public static async Task CheckVcRuntime() {
        using var root = Registry.LocalMachine;
        using var sub = root.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall")!;
        var installed = sub.GetSubKeyNames()
            .Select(subKeyName => sub.OpenSubKey(subKeyName))
            .Select(item => item?.GetValue("DisplayName") as string ?? string.Empty)
            .Any(name => name.Contains("Microsoft Visual C++ 2022 X64 "));
        if (!installed) {
            Console.WriteLine(App.VcRuntimeDownload);
            var pkgPath = Path.Combine(GlobalVars.DataPath, "vc_redist.x64.exe");
            var stream = await CHttpClient.Value.GetStreamAsync("https://aka.ms/vs/17/release/vc_redist.x64.exe");
            var output = File.OpenWrite(pkgPath);
            await stream.CopyToAsync(output);
            stream.Close();
            output.Close();
            Console.WriteLine(App.VcRuntimeInstalling);
            using var process = new Process {
                StartInfo = {
                    FileName = pkgPath,
                    Arguments = "/install /passive /norestart"
                }
            };
            process.Start();
            await process.WaitForExitAsync();
            File.Delete(pkgPath);
        }
    }
}
