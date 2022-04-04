#include <string>
#include "utils.h"
#include "define.h"

namespace native {

    Value checkGameIsRunning(Env env, CallbackInfo info) {
        ull argc = 0;
        Value args[1];
        if (GetCallbackInfo(env, info, &argc, args, nullptr, nullptr) != napi_ok) {
            return nullptr;
        }
        char nameBuffer[256];
        GetUTF8String(env, args[0], (char *) &nameBuffer, sizeof(nameBuffer), nullptr);
        wstring pn = StringToWString(nameBuffer);
        bool isRunning = false;
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(entry);
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (Process32First(snapshot, &entry) == TRUE) {
            while (Process32Next(snapshot, &entry) == TRUE) {
                if (wstring(entry.szExeFile) == pn) {
                    isRunning = true;
                }
            }
        }
        CloseHandle(snapshot);
        Value ret;
        GetBoolean(env, isRunning, &ret);
        return ret;
    }

    Value selectGameExecutable(Env env, CallbackInfo args) {
        Value path;
        LSTATUS retcode = OpenFile(env, path);
        if (retcode != ERROR_SUCCESS) {
            SetLastError(retcode);
            return nullptr;
        }
        return path;
    }

    Value init(Env env, Value exports) {
        EnablePrivilege(L"SeDebugPrivilege");
        if (RegisterFunction(env, exports, checkGameIsRunning, "checkGameIsRunning") != napi_ok) {
            ThrowError(env, nullptr, "Failed to register checkGameIsRunning");
            return nullptr;
        }
        if (RegisterFunction(env, exports, selectGameExecutable, "selectGameExecutable") != napi_ok) {
            ThrowError(env, nullptr, "Failed to register selectGameExecutable");
            return nullptr;
        }
        return exports;
    }

    NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

}
