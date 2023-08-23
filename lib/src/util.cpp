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

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(BYTE const* buf, unsigned int bufLen) {
    std::string ret;
    int i = 0;
    BYTE char_array_3[3];
    BYTE char_array_4[4];
    while (bufLen--) {
        char_array_3[i++] = *buf++;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    if (i) {
        int j;
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        for (j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];
        while (i++ < 3)
            ret += '=';
    }
    return ret;
}
