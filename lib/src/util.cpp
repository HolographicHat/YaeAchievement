#include "pch.h"
#include "util.h"

VOID DisableVMProtect() {
    DWORD oldProtect = 0;
    auto ntdll = GetModuleHandleA("ntdll.dll");
    auto pNtProtectVirtualMemory = GetProcAddress(ntdll, "NtProtectVirtualMemory");
    auto pNtQuerySection = GetProcAddress(ntdll, "NtQuerySection");
    DWORD old;
    VirtualProtect(pNtProtectVirtualMemory, 1, PAGE_EXECUTE_READWRITE, &old);
    *(uintptr_t*)pNtProtectVirtualMemory = *(uintptr_t*)pNtQuerySection & ~(0xFFui64 << 32) | (uintptr_t)(*(uint32_t*)((uintptr_t)pNtQuerySection + 4) - 1) << 32;
    VirtualProtect(pNtProtectVirtualMemory, 1, old, &old);
}

#pragma region StringConvert

string ToString(Il2CppString* str, UINT codePage) {
    auto chars = reinterpret_cast<const wchar_t*>(str->chars);
    auto len = WideCharToMultiByte(codePage, 0, chars, -1, nullptr, 0, nullptr, nullptr);
    auto buffer = new char[len];
    WideCharToMultiByte(codePage, 0, chars, -1, buffer, len, nullptr, nullptr);
    return string(buffer);
}

#pragma endregion

#pragma region ByteUtils

bool IsLittleEndian() {
    UINT i = 1;
    char* c = (char*)&i;
    return (*c);
}

#pragma endregion

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
