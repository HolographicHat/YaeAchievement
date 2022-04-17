#include "utils.h"
#include "define.h"
#include "wmi/wmi.hpp"
#include "wmi/wmiclasses.hpp"
#include "registry/registry.hpp"
#include <conio.h>
#include <iphlpapi.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"iphlpapi.lib")

using Wmi::Win32_ComputerSystem, Wmi::Win32_OperatingSystem, Wmi::retrieveWmi;

namespace native {

    Value checkGameIsRunning(const CallbackInfo &info) {
        Env env = info.Env();
        if (info.Length() != 1 || !info[0].IsString()) {
            TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
            return env.Undefined();
        }
        wstring name = StringToWString(info[0].As<Napi::String>().Utf8Value());
        bool isRunning = false;
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(entry);
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (Process32First(snapshot, &entry) == TRUE) {
            while (Process32Next(snapshot, &entry) == TRUE) {
                if (wstring(entry.szExeFile) == name) {
                    isRunning = true;
                }
            }
        }
        CloseHandle(snapshot);
        return Napi::Boolean::New(env, isRunning);
    }

    Value selectGameExecutable(const CallbackInfo &info) {
        Env env = info.Env();
        Napi::String path;
        if (OpenFile(env, path) != ERROR_SUCCESS) {
            Error::New(env, "Failed to open file: " + to_string(CommDlgExtendedError())).ThrowAsJavaScriptException();
            return env.Undefined();
        }
        return path;
    }

    Value whoUseThePort(const CallbackInfo &info) {
        Env env = info.Env();
        if (info.Length() != 1 || !info[0].IsNumber()) {
            TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
            return env.Undefined();
        }
        DWORD dwSize = 0;
        PMIB_TCPTABLE_OWNER_PID pTcpTable = nullptr;
        GetExtendedTcpTable(pTcpTable, &dwSize, TRUE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0);
        pTcpTable = (PMIB_TCPTABLE_OWNER_PID)new byte[dwSize];
        if(GetExtendedTcpTable(pTcpTable,&dwSize,TRUE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0) != NO_ERROR) {
            Error::New(env, "GetExtendedTcpTable failed").ThrowAsJavaScriptException();
            return env.Undefined();
        }
        int port = info[0].As<Napi::Number>().Int32Value();
        auto nNum = (int)pTcpTable->dwNumEntries;
        DWORD pid = 0;
        for(int i = 0; i < nNum; i++) {
            if (htons(pTcpTable->table[i].dwLocalPort) == port) {
                pid = pTcpTable->table[i].dwOwningPid;
                break;
            }
        }
        delete pTcpTable;
        Value ret = env.Undefined();
        if (pid != 0) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
            if (hProcess == nullptr) {
                Error::New(env, "OpenProcess error: " + to_string(GetLastError())).ThrowAsJavaScriptException();
                return env.Undefined();
            }
            TCHAR fnBuf[MAX_PATH];
            DWORD length = MAX_PATH;
            if (QueryFullProcessImageName(hProcess, 0, fnBuf, &length) == 0) {
                Error::New(env, "QueryFullProcessImageName error: " + to_string(GetLastError())).ThrowAsJavaScriptException();
                return env.Undefined();
            }
            Object obj = Object::New(env);
            obj.Set("pid", Napi::Number::New(env, pid));
            obj.Set("path", Napi::String::New(env, WStringToString(fnBuf)));
            ret = obj;
        }
        return ret;
    }

    Value getDeviceID(const CallbackInfo &info) {
        Env env = info.Env();
        wstring wd;
        if (RegUtils::GetString(HKEY_CURRENT_USER, L"SOFTWARE\\miHoYoSDK", L"MIHOYOSDK_DEVICE_ID", wd) != ERROR_SUCCESS) {
            return env.Undefined();
        }
        string id = WStringToString(wd);
        return Napi::String::New(env, id.substr(0, 8) + "-" + id.substr(8, 4) + "-" + id.substr(12, 4) + "-" + id.substr(16, 4) + "-" + id.substr(20, 12));
    }

    Value getDeviceInfo(const CallbackInfo &info) {
        Env env = info.Env();
        HDC desktop = GetDC(nullptr);
        int sw = GetDeviceCaps(desktop, DESKTOPHORZRES);
        int sh = GetDeviceCaps(desktop, DESKTOPVERTRES);
        ReleaseDC(nullptr, desktop);
        DWORD buildNum = RegUtils::GetInt(env, HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"UBR", 0);
        wstring locale;
        if (RegUtils::GetString(HKEY_CURRENT_USER, L"Control Panel\\International", L"LocaleName", locale) != ERROR_SUCCESS) {
            locale = L"zh-CN";
        }
        wstring country;
        if (RegUtils::GetString(HKEY_CURRENT_USER, L"Control Panel\\International\\Geo", L"Name", country) != ERROR_SUCCESS) {
            country = L"CN";
        }
        Object obj = Object::New(env);
        obj.Set("locale", Napi::String::New(env, WStringToString(locale)));
        obj.Set("screenSize", Napi::String::New(env, to_string(sw) + "x" + to_string(sh)));
        obj.Set("carrierCountry", Napi::String::New(env, WStringToString(country)));
        try {
            auto computer = retrieveWmi<Win32_ComputerSystem>();
            obj.Set("model", Napi::String::New(env, computer.Model));
            obj.Set("oemName", Napi::String::New(env, computer.Manufacturer));
        } catch (Wmi::WmiException &e) {
            obj.Set("model", Napi::String::New(env, "Unknown"));
            obj.Set("oemName", Napi::String::New(env, "Unknown"));
        }
        try {
            auto os  = retrieveWmi<Win32_OperatingSystem>();
            string osv = os.Version;
            obj.Set("osBuild", Napi::String::New(env, osv + "." + to_string(buildNum)));
            obj.Set("osVersion", Napi::String::New(env, osv));
            obj.Set("timeZoneOffset", Napi::Number::New(env, os.CurrentTimeZone));
        } catch (Wmi::WmiException &e) {
            obj.Set("osBuild", Napi::String::New(env, "Unknown"));
            obj.Set("osVersion", Napi::String::New(env, "Unknown"));
            obj.Set("timeZoneOffset", Napi::Number::New(env, 480));
        }
        return obj;
    }

    Value enablePrivilege(const CallbackInfo &info) {
        Env env = info.Env();
        EnablePrivilege(env, L"SeDebugPrivilege");
        return env.Undefined();
    }

    Value copyToClipboard(const CallbackInfo &info) {
        Env env = info.Env();
        string text = info[0].As<Napi::String>().Utf8Value();
        if (OpenClipboard(nullptr)) {
            EmptyClipboard();
            HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.length() + 1);
            if (hg != nullptr) {
                memcpy(GlobalLock(hg), text.c_str(), text.length() + 1);
                GlobalUnlock(hg);
                SetClipboardData(CF_TEXT, hg);
            }
            CloseClipboard();
        }
        return env.Undefined();
    }

    Value pause(const CallbackInfo &info) {
        while(!_kbhit()) {
            Sleep(10);
        }
        return info.Env().Undefined();
    }

    Value openUrl(const CallbackInfo &info) {
        Env env = info.Env();
        string nUrl = info[0].As<Napi::String>().Utf8Value();
        wstring url = GetACP() == 936 ? StringToWString(nUrl, CP_UTF8) : StringToWString(nUrl);
        Log(env, L"openUrl: " + url);
        HINSTANCE retcode = ShellExecute(GetConsoleWindow(), L"open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        return Napi::Number::New(env, (INT_PTR)retcode); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    Value checkSnapFastcall(const CallbackInfo &info) {
        Env env = info.Env();
        wstring queryResult;
        RegUtils::GetString(HKEY_CLASSES_ROOT, L"snapgenshin", L"", queryResult);
        return Napi::Boolean::New(env, wcscmp(queryResult.c_str(), L"URL:snapgenshin") == 0);
    }

    Object init(Env env, Object exports) {
        exports.Set("pause", Function::New(env, pause));
        exports.Set("openUrl", Function::New(env, openUrl));
        exports.Set("getDeviceID", Function::New(env, getDeviceID));
        exports.Set("getDeviceInfo", Function::New(env, getDeviceInfo));
        exports.Set("whoUseThePort", Function::New(env, whoUseThePort));
        exports.Set("copyToClipboard", Function::New(env, copyToClipboard));
        exports.Set("enablePrivilege", Function::New(env, enablePrivilege));
        exports.Set("checkSnapFastcall", Function::New(env, checkSnapFastcall));
        exports.Set("checkGameIsRunning", Function::New(env, checkGameIsRunning));
        exports.Set("selectGameExecutable", Function::New(env, selectGameExecutable));
        return exports;
    }

    NODE_API_MODULE(addon, init)

}
