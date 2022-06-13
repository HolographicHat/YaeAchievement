using System.ComponentModel;
using System.Diagnostics;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Text.Json.Nodes;
using YaeAchievement.Win32;
using static YaeAchievement.Win32.OpenFileFlags;

namespace YaeAchievement;

public static class Utils {

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
                    if (onReceive(line)) break;
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