using Microsoft.Win32.SafeHandles;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using Windows.Win32;
using Windows.Win32.System.Memory;
using Windows.Win32.System.Threading;
using YaeAchievement.Win32;

namespace YaeAchievement;

public static class Injector {
    public static unsafe bool CreateProcess(string path, out SafeHandle hProc, out SafeHandle hThread, out uint pid) {
        Span<char> cmdLines = stackalloc char[1]; // "\0"
        var si = new STARTUPINFOW() { cb = unchecked((uint)sizeof(STARTUPINFOW)) };
        var dir = Path.GetDirectoryName(path)!;
        var result = Native.CreateProcess(
            path, ref cmdLines, default, default, false,
            PROCESS_CREATION_FLAGS.CREATE_SUSPENDED, default, dir, in si, out var pi
        );
        pid = pi.dwProcessId;
        hProc = new SafeFileHandle(pi.hProcess, true);
        hThread = new SafeFileHandle(pi.hThread, true);
        return result;
    }

    // todo: refactor
    public static unsafe int LoadLibraryAndInject(SafeHandle hProc, string libPath)
    {
        var hKernel = Native.GetModuleHandle("kernel32.dll");
        if (hKernel.IsInvalid)
        {
            return new Win32Exception().PrintMsgAndReturnErrCode("GetModuleHandle fail");
        }
        var pLoadLibrary = Native.GetProcAddress(hKernel, "LoadLibraryA");
        if (pLoadLibrary.IsNull)
        {
            return new Win32Exception().PrintMsgAndReturnErrCode("GetProcAddress fail");
        }
        var pBase = Native.VirtualAllocEx(hProc, default, unchecked((uint)libPath.Length + 1), VIRTUAL_ALLOCATION_TYPE.MEM_RESERVE | VIRTUAL_ALLOCATION_TYPE.MEM_COMMIT, PAGE_PROTECTION_FLAGS.PAGE_READWRITE);
        if ((nint)pBase == 0)
        {
            return new Win32Exception().PrintMsgAndReturnErrCode("VirtualAllocEx fail");
        }
        fixed (void* lpBuffer = libPath.ToCharArray())
        {
            if (!Native.WriteProcessMemory(hProc, pBase, lpBuffer, unchecked((uint)libPath.Length), default))
            {
                return new Win32Exception().PrintMsgAndReturnErrCode("WriteProcessMemory fail");
            }
        }
        var lpStartAddress = pLoadLibrary.CreateDelegate<LPTHREAD_START_ROUTINE>();

        using (var hThread = Native.CreateRemoteThread(hProc, default, 0, lpStartAddress, pBase, 0, default))
        {
            if (hThread.IsInvalid)
            {
                var e = new Win32Exception();
                Native.VirtualFreeEx(hProc, pBase, 0, VIRTUAL_FREE_TYPE.MEM_RELEASE);
                return e.PrintMsgAndReturnErrCode("CreateRemoteThread fail");
            }
            if (Native.WaitForSingleObject(hThread, 2000) == 0)
            {
                Native.VirtualFreeEx(hProc, pBase, 0, VIRTUAL_FREE_TYPE.MEM_RELEASE);
            }
            //return !Native.CloseHandle(hThread) ? new Win32Exception().PrintMsgAndReturnErrCode("CloseHandle fail") : 0;
            return 0;
        }
    }

}