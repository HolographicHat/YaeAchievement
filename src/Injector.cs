using System.ComponentModel;
using YaeAchievement.Win32;

namespace YaeAchievement; 

public static class Injector {

    public static unsafe bool CreateProcess(string path, out IntPtr hProc, out IntPtr hThread, out uint pid) {
        var si = new StartupInfo();
        SecurityAttributes* attr = null;
        var result = Native.CreateProcess(
            path, null, ref *attr, ref *attr, false,
            CreationFlags.CreateSuspended, IntPtr.Zero, null, ref si, out var pi
        );
        pid = pi.dwProcessID;
        hProc = pi.hProcess;
        hThread = pi.hThread;
        return result;
    }
    
    public static int LoadLibraryAndInject(IntPtr handle, string libPath) {
        var hKernel = Native.GetModuleHandle("kernel32.dll");
        if (hKernel == IntPtr.Zero) {
            return new Win32Exception().PrintMsgAndReturnErrCode("GetModuleHandle fail");
        }
        var pLoadLibrary = Native.GetProcAddress(hKernel, "LoadLibraryA");
        if (pLoadLibrary == IntPtr.Zero) {
            return new Win32Exception().PrintMsgAndReturnErrCode("GetProcAddress fail");
        }
        var pBase = Native.VirtualAllocEx(handle, IntPtr.Zero, libPath.Length + 1, AllocationType.Reserve | AllocationType.Commit, MemoryProtection.ReadWrite);
        if (pBase == IntPtr.Zero) {
            return new Win32Exception().PrintMsgAndReturnErrCode("VirtualAllocEx fail");
        }
        if (!Native.WriteProcessMemory(handle, pBase, libPath.ToCharArray(), libPath.Length, out _)) {
            return new Win32Exception().PrintMsgAndReturnErrCode("WriteProcessMemory fail");
        }
        var hThread = Native.CreateRemoteThread(handle, IntPtr.Zero, 0, pLoadLibrary, pBase, 0, out _);
        if (hThread == IntPtr.Zero) {
            var e = new Win32Exception();
            if (!Native.VirtualFreeEx(handle, pBase, 0, AllocationType.Release)) {
                new Win32Exception().PrintMsgAndReturnErrCode("VirtualFreeEx fail");
            }
            return e.PrintMsgAndReturnErrCode("CreateRemoteThread fail");
        }
        return !Native.CloseHandle(hThread) ? new Win32Exception().PrintMsgAndReturnErrCode("CloseHandle fail") : 0;
    }
    
}