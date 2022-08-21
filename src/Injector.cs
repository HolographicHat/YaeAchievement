using System.ComponentModel;
using YaeAchievement.Win32;
using static YaeAchievement.Win32.Native;

namespace YaeAchievement; 

public static class Injector {

    public static unsafe bool CreateProcess(string path, out IntPtr hProc, out IntPtr hThread, out uint pid) {
        var si = new StartupInfo();
        SecurityAttributes* attr = null;
        var dir = Path.GetDirectoryName(path)!;
        var result = Native.CreateProcess(
            path, null, ref *attr, ref *attr, false,
            CreationFlags.CreateSuspended, IntPtr.Zero, dir, ref si, out var pi
        );
        pid = pi.dwProcessID;
        hProc = pi.hProcess;
        hThread = pi.hThread;
        return result;
    }
    
    // todo: refactor
    public static int LoadLibraryAndInject(IntPtr hProc, string libPath) {
        var hKernel = GetModuleHandle("kernel32.dll");
        if (hKernel == IntPtr.Zero) {
            return new Win32Exception().PrintMsgAndReturnErrCode("GetModuleHandle fail");
        }
        var pLoadLibrary = GetProcAddress(hKernel, "LoadLibraryA");
        if (pLoadLibrary == IntPtr.Zero) {
            return new Win32Exception().PrintMsgAndReturnErrCode("GetProcAddress fail");
        }
        var pBase = VirtualAllocEx(hProc, IntPtr.Zero, libPath.Length + 1, AllocationType.Reserve | AllocationType.Commit, MemoryProtection.ReadWrite);
        if (pBase == IntPtr.Zero) {
            return new Win32Exception().PrintMsgAndReturnErrCode("VirtualAllocEx fail");
        }
        if (!WriteProcessMemory(hProc, pBase, libPath.ToCharArray(), libPath.Length, out _)) {
            return new Win32Exception().PrintMsgAndReturnErrCode("WriteProcessMemory fail");
        }
        var hThread = CreateRemoteThread(hProc, IntPtr.Zero, 0, pLoadLibrary, pBase, 0, out _);
        if (hThread == IntPtr.Zero) {
            var e = new Win32Exception();
            VirtualFreeEx(hProc, pBase, 0, AllocationType.Release);
            return e.PrintMsgAndReturnErrCode("CreateRemoteThread fail");
        }
        if (WaitForSingleObject(hThread, 2000) == 0) {
            VirtualFreeEx(hProc, pBase, 0, AllocationType.Release);
        }
        return !CloseHandle(hThread) ? new Win32Exception().PrintMsgAndReturnErrCode("CloseHandle fail") : 0;
    }
    
}