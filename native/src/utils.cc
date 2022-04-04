#include "utils.h"
#include "define.h"

wstring StringToWString(const string &src) {
    wstring result;
    int len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), src.size(), nullptr, 0); // NOLINT(cppcoreguidelines-narrowing-conversions)
    auto *buffer = new WCHAR[len + 1];
    MultiByteToWideChar(CP_ACP, 0, src.c_str(), src.size(), buffer, len); // NOLINT(cppcoreguidelines-narrowing-conversions)
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;
    return result;
}

string WStringToString(const wstring &src) {
    string result;
    int len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), src.size(), nullptr, 0, nullptr, nullptr); // NOLINT(cppcoreguidelines-narrowing-conversions)
    char *buffer = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, src.c_str(), src.size(), buffer, len, nullptr, nullptr); // NOLINT(cppcoreguidelines-narrowing-conversions)
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;
    return result;
}

void Log(Env env, const string &msg) {
    auto logFunc = env.Global().Get("console").As<Object>().Get("log").As<Function>();
    logFunc.Call({ Napi::String::New(env, msg) });
}

void Log(Env env, const wstring &msg) {
    auto logFunc = env.Global().Get("console").As<Object>().Get("log").As<Function>();
    logFunc.Call({ Napi::String::New(env, WStringToString(msg)) });
}

LSTATUS OpenFile(Env env, Napi::String &result, HWND parent) {
    OPENFILENAME open;
    ZeroMemory(&open, sizeof(open));
    WCHAR file[32768];
    file[0]=L'\0';
    open.Flags       = OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_EXPLORER;
    open.hwndOwner   = parent;
    open.nMaxFile    = 32768;
    open.lpstrFile   = file;
    open.lpstrTitle  = L"选择原神主程序";
    open.lpstrFilter = L"国服/国际服主程序 (YuanShen/GenshinImpact.exe)\0YuanShen.exe;GenshinImpact.exe\0";
    open.lStructSize = sizeof(open);
    if(GetOpenFileName(&open)) {
        result = Napi::String::New(env, WStringToString(file));
        return ERROR_SUCCESS;
    } else {
        return ERROR_ERRORS_ENCOUNTERED;
    }
}

BOOL EnablePrivilege(Env env, const wstring &name) {
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
        Error::New(env, "OpenProcessToken error: " + to_string(GetLastError())).ThrowAsJavaScriptException();
        return FALSE;
    }
    TOKEN_PRIVILEGES tp;
    ZeroMemory(&tp, sizeof(tp));
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!LookupPrivilegeValue(nullptr, name.c_str(), &tp.Privileges[0].Luid)) {
        Error::New(env, "LookupPrivilegeValue error: " + to_string(GetLastError())).ThrowAsJavaScriptException();
        return FALSE;
    }
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), nullptr, nullptr)) {
        Error::New(env, "AdjustTokenPrivileges error: " + to_string(GetLastError())).ThrowAsJavaScriptException();
        return FALSE;
    }
    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        Error::New(env, "The token does not have the specified privilege.").ThrowAsJavaScriptException();
        return FALSE;
    }
    CloseHandle(hToken);
    return TRUE;
}
