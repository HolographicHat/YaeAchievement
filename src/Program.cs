using System.ComponentModel;
using System.IO.Pipes;
using YaeAchievement;
using YaeAchievement.Win32;

var hThread = IntPtr.Zero;
var hProcess = IntPtr.Zero;
if (!Injector.CreateProcess("exePath", ref hThread, ref hProcess)) {
    Environment.Exit(new Win32Exception().PrintMsgAndReturnErrCode("ICreateProcess fail"));
}
if (Injector.LoadLibraryAndInject(hProcess, "YaeAchievementLib.dll") != 0) {
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

bool OnXXXReceived(string something) {
    return true;
}

var ts = new ThreadStart(() => {
    var server = new NamedPipeServerStream("");
    server.WaitForConnection();
    var reader = new StreamReader(server);
    while (true) {
        var line = reader.ReadLine();
        if (line?.Length > 0) {
            if (OnXXXReceived(line)) {
                break;
            }
            server.Disconnect();
            server.WaitForConnection();
        }
    }
});
new Thread(ts).Start();
