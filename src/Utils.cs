using System.ComponentModel;
using System.Diagnostics;
using System.IO.Compression;
using System.IO.Pipes;
using System.Net;
using System.Net.Http.Headers;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json.Nodes;
using Google.Protobuf;
using YaeAchievement.Win32;
using static YaeAchievement.Win32.OpenFileFlags;

namespace YaeAchievement;

public static class Utils {

    public static readonly Lazy<HttpClient> CHttpClient = new (() => {
        var c = new HttpClient(new HttpClientHandler {
            Proxy = new WebProxy("http://127.0.0.1:8888"),
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

    public static string GetBucketFileAsString(string path, bool cache = true) {
        return Encoding.UTF8.GetString(GetBucketFileAsByteArray(path, cache));
    }
    
    public static byte[] GetBucketFileAsByteArray(string path, bool cache = true) {
        using var msg = new HttpRequestMessage {
            Method = HttpMethod.Get,
            RequestUri = new Uri($"{GlobalVars.BucketHost}/{path}")
        };
        CacheItem? ci = null;
        var cacheName = cache ? $"./cache/{CalculateMD5(path)[..16]}.miko" : "";
        if (cache) {
            Directory.CreateDirectory("cache");
            if (File.Exists(cacheName)) {
                using var input = File.OpenRead(cacheName);
                using var dInput = new GZipStream(input, CompressionMode.Decompress);
                ci = CacheItem.Parser.ParseFrom(dInput);
                msg.Headers.TryAddWithoutValidation("If-None-Match", $"{ci.Etag}");
            }
        }
        using var response = CHttpClient.Value.Send(msg);
        if (cache && response.StatusCode == HttpStatusCode.NotModified) {
            return ci!.Content.ToByteArray();
        }
        response.EnsureSuccessStatusCode();
        var responseBytes = response.Content.ReadAsByteArrayAsync().Result;
        if (cache) {
            var etag = response.Headers.ETag!.Tag;
            using var os = File.OpenWrite(cacheName);
            using var cos = new GZipStream(os, CompressionLevel.SmallestSize);
            new CacheItem {
                Etag = etag,
                Version = 3,
                Checksum = CalculateMD5(responseBytes),
                Content = ByteString.CopyFrom(responseBytes)
            }.WriteTo(cos);
        }
        return responseBytes;
    }

    // ReSharper disable once InconsistentNaming
    private static string CalculateMD5(string text) {
        return CalculateMD5(Encoding.UTF8.GetBytes(text));
    }
    
    // ReSharper disable once InconsistentNaming
    private static string CalculateMD5(byte[] bytes) {
        using var md5 = MD5.Create();
        var b = md5.ComputeHash(bytes);
        return Convert.ToHexString(b).ToLower();
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
    
    public static void LoadConfig() {
        var conf = JsonNode.Parse(File.ReadAllText(GlobalVars.ConfigFileName))!;
        var path = conf["genshinPath"];
        if (path == null || CheckGamePathValid(path.GetValue<string>())) {
            GlobalVars.GamePath = SelectGameExecutable();
            conf["genshinPath"] = GlobalVars.GamePath;
            File.WriteAllText(GlobalVars.ConfigFileName, conf.ToJsonString());
        } else {
            GlobalVars.GamePath = path.GetValue<string>();
        }
    }

    public static void CheckUpdate() {
        var info = UpdateInfo.Parser.ParseFrom(GetBucketFileAsByteArray("schicksal/version"))!;
        if (GlobalVars.AppVersionCode != info.VersionCode) {
            Console.WriteLine($"有可用更新: {GlobalVars.AppVersionName} => {info.VersionName}");
            Console.WriteLine($"更新内容: \n{info.Description}");
            if (info.EnableAutoDownload) {
                Console.WriteLine("正在下载更新包...");
                var fullPath = Path.GetFullPath("update.7z");
                File.WriteAllBytes(fullPath, GetBucketFileAsByteArray(info.PackageLink));
                Console.WriteLine("下载完毕! 关闭程序后, 将压缩包解压至当前目录即可完成更新.");
                ShellOpen(fullPath);
                Environment.Exit(0);
            }
            Console.WriteLine($"下载地址: {info.PackageLink}");
            if (info.ForceUpdate) {
                Console.WriteLine("在完成此次更新前, 程序可能无法正常使用.");
                Environment.Exit(0);
            }
        }
        if (info.EnableLibDownload) {
            File.WriteAllBytes("YaeLib.dll", GetBucketFileAsByteArray("schicksal/lib.dll"));
        }
    }

    public static bool ShellOpen(string path) {
        return new Process {
            StartInfo = {
                FileName = path,
                UseShellExecute = true
            }
        }.Start();
    }

    private static bool CheckGamePathValid(string path) {
        var dir = Path.GetDirectoryName(path)!;
        return File.Exists($"{dir}/UnityPlayer.dll") && File.Exists($"{dir}/mhypbase.dll");
    }
    
    private static string SelectGameExecutable() {
        var fnPtr = Marshal.AllocHGlobal(32768);
        Native.RtlZeroMemory(fnPtr, 32768);
        var ofn = new OpenFileName {
            file    = fnPtr,
            size    = Marshal.SizeOf<OpenFileName>(),
            owner   = Native.GetConsoleWindow(),
            flags   = Explorer | NoNetworkButton | FileMustExist | NoChangeDir,
            title   = "选择主程序",
            filter  = "国服/国际服主程序 (YuanShen/GenshinImpact.exe)\0YuanShen.exe;GenshinImpact.exe\0",
            maxFile = 32768
        };
        new Thread(() => {
            var handle = Native.FindWindow("#32770", "选择主程序");
            while (handle == IntPtr.Zero) {
                handle = Native.FindWindow("#32770", "选择主程序");
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
            Console.WriteLine("操作被取消");
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
            if (process.ProcessName is "GenshinImpact" or "YuanShen") {
                Console.WriteLine("原神正在运行，请关闭后重试");
                Environment.Exit(301);
            }
        }
        Process.LeaveDebugMode();
    }
    
    public static void InstallExitHook() {
        AppDomain.CurrentDomain.ProcessExit += (_, _) => {
            Console.WriteLine("按任意键退出");
            Console.ReadKey();
        };
    }

    public static void InstallExceptionHook() {
        AppDomain.CurrentDomain.UnhandledException += (_, e) => {
            Console.WriteLine(e.ExceptionObject.ToString());
            Console.WriteLine("发生错误，请联系开发者以获取帮助");
            Environment.Exit(-1);
        };
    }
    
    // ReSharper disable once UnusedMethodReturnValue.Global
    public static Thread StartAndWaitResult(string exePath, Func<string, bool> onReceive) {
        if (!Injector.CreateProcess(exePath, out var hProcess, out var hThread, out var pid)) {
            Environment.Exit(new Win32Exception().PrintMsgAndReturnErrCode("ICreateProcess fail"));
        }
        if (Injector.LoadLibraryAndInject(hProcess, Path.GetFullPath(GlobalVars.LibName)) != 0) {
            if (!Native.TerminateProcess(hProcess, 0)) {
                Environment.Exit(new Win32Exception().PrintMsgAndReturnErrCode("TerminateProcess fail"));
            }
        }
        var proc = Process.GetProcessById(Convert.ToInt32(pid));
        proc.EnableRaisingEvents = true;
        proc.Exited += (_, _) => {
            if (GlobalVars.UnexpectedExit) {
                Console.WriteLine("游戏进程异常退出");
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
}