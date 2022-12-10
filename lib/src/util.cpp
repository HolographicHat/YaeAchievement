#include "pch.h"
#include "util.h"

VOID DisableVMProtect() {
    DWORD oldProtect = 0;
    auto ntdll = GetModuleHandleA("ntdll.dll");
    BYTE callcode = ((BYTE*)GetProcAddress(ntdll, "NtQuerySection"))[4] - 1;
    BYTE restore[] = { 0x4C, 0x8B, 0xD1, 0xB8, callcode };
    auto nt_vp = (BYTE*)GetProcAddress(ntdll, "NtProtectVirtualMemory");
    VirtualProtect(nt_vp, sizeof(restore), PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(nt_vp, restore, sizeof(restore));
    VirtualProtect(nt_vp, sizeof(restore), oldProtect, &oldProtect);
}

#pragma region StringConvert

string IlStringToString(Il2CppString* str, UINT codePage) {
    auto chars = reinterpret_cast<const wchar_t*>(str->chars);
    auto len = WideCharToMultiByte(codePage, 0, chars, -1, nullptr, 0, nullptr, nullptr);
    auto buffer = new char[len];
    WideCharToMultiByte(codePage, 0, chars, -1, buffer, len, nullptr, nullptr);
    return string(buffer);
}

#pragma endregion

#pragma region GC

UINT32 GCHandle_New(void* object, bool pinned) {
    return il2cpp_gchandle_new((Il2CppObject*)object, pinned);
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
