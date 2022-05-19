#include "utils.h"
#include "define.h"

wstring StringToWString(const string &src, UINT codePage) {
    int len = MultiByteToWideChar(codePage, 0, src.c_str(), -1, nullptr, 0);
    auto *buffer = new WCHAR[len];
    MultiByteToWideChar(codePage, 0, src.c_str(), -1, buffer, len);
    wstring strTemp(buffer);
    delete[] buffer;
    return strTemp;
}

string WStringToString(const wstring &src, UINT codePage) {
    int len = WideCharToMultiByte(codePage, 0, src.c_str(), -1, nullptr, 0, nullptr, nullptr);
    auto *buffer = new CHAR[len];
    WideCharToMultiByte(codePage, 0, src.c_str(), -1, buffer, len, nullptr, nullptr);
    string strTemp(buffer);
    delete[] buffer;
    return strTemp;
}

void Log(Env env, const string &msg) {
    auto logFunc = env.Global().Get("console").As<Object>().Get("log").As<Function>();
    logFunc.Call({ Napi::String::New(env, msg) });
}

void Log(Env env, const wstring &msg) {
    auto logFunc = env.Global().Get("console").As<Object>().Get("log").As<Function>();
    logFunc.Call({ Napi::String::New(env, WStringToString(msg)) });
}

char* ToHex(const char *bin, int binsz, char **result) {
    char hexStr[] = "0123456789ABCDEF";
    if (!(*result = (char *) malloc(binsz * 2 + 1))) return nullptr;
    (*result)[binsz * 2] = 0;
    if (!binsz) return nullptr;
    for (int i = 0; i < binsz; i++) {
        (*result)[i * 2 + 0] = hexStr[(bin[i] >> 4) & 0x0F];
        (*result)[i * 2 + 1] = hexStr[(bin[i]) & 0x0F];
    }
    return *result;
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
        result = GetACP() == 936 ? Napi::String::New(env, WStringToString(file, CP_UTF8)) : Napi::String::New(env, WStringToString(file));
        return ERROR_SUCCESS;
    } else {
        return ERROR_ERRORS_ENCOUNTERED;
    }
}
