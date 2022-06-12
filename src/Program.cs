using System.ComponentModel;
using System.Diagnostics;
using System.IO.Pipes;
using YaeAchievement;
using YaeAchievement.Events;
using YaeAchievement.Win32;

// ReSharper disable once UnusedLocalFunctionReturnValue
Thread StartAndWaitResult(string exePath, Func<IEventBase, bool> onEvent) {
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
            Console.WriteLine($"Game process exit at {proc.ExitTime:HH:mm:ss}");
            Environment.Exit(114514);
        }
    };
    var ts = new ThreadStart(() => {
        var server = new NamedPipeServerStream(GlobalVars.PipeName);
        server.WaitForConnection();
        var reader = new StreamReader(server);
        while (true) {
            var line = reader.ReadLine();
            if (line?.Length > 0) {
                if (onEvent(new PipeRecvEvent(line))) {
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

StartAndWaitResult(@"D:\Genshin Impact Dev\2.8\YuanShen.exe", evt => {
    switch (evt) {
        case PipeRecvEvent @event: {
            var bytes = @event.GetData();
            GlobalVars.UnexpectedExit = false;
            Console.WriteLine("On PRE");
            Console.WriteLine(Convert.ToBase64String(bytes));
            return false;
        }
        default:
            return false;
    }
});
