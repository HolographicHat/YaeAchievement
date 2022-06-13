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

    private static readonly Lazy<HttpClient> CHttpClient = new (() => {
        var c = new HttpClient(new HttpClientHandler {
            Proxy = new WebProxy("http://127.0.0.1:8888")
        }) {
            DefaultRequestHeaders = {
                UserAgent = {
                    new ProductInfoHeaderValue("YaeAchievement", GlobalVars.AppVersion.ToString(2))
                }
            }
        };
        return c;
    });

    public static string GetBucketFileAsString(string path) {
        return Encoding.UTF8.GetString(GetBucketFileAsByteArray(path));
    }
    
    public static byte[] GetBucketFileAsByteArray(string path) {
        using var msg = new HttpRequestMessage {
            Method = HttpMethod.Get,
            RequestUri = new Uri($"{GlobalVars.BucketHost}/{path}")
        };
        CacheItem? ci = null;
        string?  etag = null;
        var cacheName = $"./cache/{CalculateMD5(path)[..16]}.miko";
        if (File.Exists(cacheName)) {
            using var input = File.OpenRead(cacheName);
            using var dInput = new GZipStream(input, CompressionMode.Decompress);
            ci = CacheItem.Parser.ParseFrom(dInput);
            etag = ci.Etag;
        }
        if (etag != null) {
            msg.Headers.TryAddWithoutValidation("If-None-Match", $"{etag}");
        }
        using var response = CHttpClient.Value.Send(msg);
        if (response.StatusCode == HttpStatusCode.NotModified) {
            return ci!.Content.ToByteArray();
        }
        response.EnsureSuccessStatusCode();
        etag = response.Headers.ETag!.Tag;
        var responseBytes = response.Content.ReadAsByteArrayAsync().Result;
        using var os = File.OpenWrite(cacheName);
        using var cos = new GZipStream(os, CompressionLevel.SmallestSize);
        new CacheItem {
            Etag = etag,
            Version = 3,
            Checksum = CalculateMD5(responseBytes),
            Content = ByteString.CopyFrom(responseBytes)
        }.WriteTo(cos);
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
    
    public static void LoadConfig() {
        Directory.CreateDirectory("cache");
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

    private static bool CheckGamePathValid(string path) {
        var dir = Path.GetDirectoryName(path)!;
        return File.Exists($"{dir}/UnityPlayer.dll") && File.Exists($"{dir}/mhypbase.dll");
    }
    
    private static string SelectGameExecutable() {
        var fnPtr = Marshal.AllocHGlobal(32768);
        var ofn = new OpenFileName {
            file    = fnPtr,
            size    = Marshal.SizeOf<OpenFileName>(),
            owner   = Native.GetConsoleWindow(),
            flags   = Explorer | NoNetworkButton | FileMustExist | NoChangeDir,
            title   = "选择主程序",
            filter  = "国服/国际服主程序 (YuanShen/GenshinImpact.exe)\0YuanShen.exe;GenshinImpact.exe\0",
            maxFile = 32768
        };
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
        if (Injector.LoadLibraryAndInject(hProcess, GlobalVars.LibName) != 0) {
            if (!Native.TerminateProcess(hProcess, 0)) {
                Environment.Exit(new Win32Exception().PrintMsgAndReturnErrCode("TerminateProcess fail"));
            }
        }
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
        var proc = Process.GetProcessById(Convert.ToInt32(pid));
        proc.EnableRaisingEvents = true;
        proc.Exited += (_, _) => {
            if (GlobalVars.UnexpectedExit) {
                Console.WriteLine("游戏进程异常退出");
                Environment.Exit(114514);
            }
        };
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