#include "pch.h"
#include "util.h"

#pragma region FindMainWindowByPID

struct HandleData {
    DWORD pid;
    HWND hwnd;
};

BOOL IsMainWindow(HWND handle) {
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle) == TRUE;
}

BOOL IsUnityWindow(HWND handle) {
    TCHAR name[256];
    GetClassName(handle, name, 256);
    return _strcmpi(name, "UnityWndClass") == 0;
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam) {
    HandleData& data = *(HandleData*)lParam;
    DWORD pid = 0;
    GetWindowThreadProcessId(handle, &pid);
    if (data.pid != pid || !IsMainWindow(handle) || !IsUnityWindow(handle))
        return TRUE;
    data.hwnd = handle;
    return FALSE;
}

HWND FindMainWindowByPID(DWORD pid) {
    HandleData data = { pid, 0 };
    EnumWindows(EnumWindowsCallback, (LPARAM)&data);
    return data.hwnd;
}

#pragma endregion
