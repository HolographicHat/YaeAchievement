#include "utils.h"
#include "define.h"
#include <iphlpapi.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"iphlpapi.lib")

namespace native {

    Value checkGameIsRunning(const CallbackInfo &info) {
        Env env = info.Env();
        if (info.Length() != 1 || !info[0].IsString()) {
            TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
            return env.Null();
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
            return env.Null();
        }
        return path;
    }

    Value whoUseThePort(const CallbackInfo &info) {
        Env env = info.Env();
        if (info.Length() != 1 || !info[0].IsNumber()) {
            TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
            return env.Null();
        }
        DWORD dwSize = 0;
        PMIB_TCPTABLE_OWNER_PID pTcpTable = nullptr;
        GetExtendedTcpTable(pTcpTable, &dwSize, TRUE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0);
        pTcpTable = (PMIB_TCPTABLE_OWNER_PID)new byte[dwSize];
        if(GetExtendedTcpTable(pTcpTable,&dwSize,TRUE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0) != NO_ERROR) {
            Error::New(env, "GetExtendedTcpTable failed").ThrowAsJavaScriptException();
            return env.Null();
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
        Value ret = env.Null();
        if (pid != 0) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
            if (hProcess == nullptr) {
                Error::New(env, "OpenProcess error: " + to_string(GetLastError())).ThrowAsJavaScriptException();
                return env.Null();
            }
            TCHAR fnBuf[MAX_PATH];
            DWORD length = MAX_PATH;
            if (QueryFullProcessImageName(hProcess, 0, fnBuf, &length) == 0) {
                Error::New(env, "QueryFullProcessImageName error: " + to_string(GetLastError())).ThrowAsJavaScriptException();
                return env.Null();
            }
            Object obj = Object::New(env);
            obj.Set("pid", Napi::Number::New(env, pid));
            obj.Set("path", Napi::String::New(env, WStringToString(fnBuf)));
            ret = obj;
        }
        return ret;
    }

    Object init(Env env, Object exports) {
        EnablePrivilege(env, L"SeDebugPrivilege");
        exports.Set("whoUseThePort", Function::New(env, whoUseThePort));
        exports.Set("checkGameIsRunning", Function::New(env, checkGameIsRunning));
        exports.Set("selectGameExecutable", Function::New(env, selectGameExecutable));
        return exports;
    }

    NODE_API_MODULE(addon, init)

}
