#pragma once

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "../define.h"

namespace RegUtils {

    DWORD GetInt(Env env, HKEY hKey, const wstring &path, const wstring &value, DWORD defaultValue) {
        DWORD data = 0;
        DWORD size = sizeof(DWORD);
        LSTATUS retcode = RegGetValue(hKey, path.c_str(), value.c_str(), RRF_RT_REG_DWORD, nullptr, &data, &size);
        if (retcode != ERROR_SUCCESS) {
            return defaultValue;
        }
        return data;
    }

    LSTATUS GetString(HKEY hKey, const wstring &path, const wstring &value, wstring &result) {
        DWORD size = 0;
        LSTATUS retcode = RegGetValue(hKey, path.c_str(), value.c_str(), RRF_RT_REG_SZ, nullptr, nullptr, &size);
        if (retcode != ERROR_SUCCESS) {
            return retcode;
        }
        wstring data;
        DWORD len = size / sizeof(WCHAR);
        data.resize(len);
        retcode = RegGetValue(hKey, path.c_str(), value.c_str(), RRF_RT_REG_SZ, nullptr, &data[0], &size);
        if (retcode != ERROR_SUCCESS) {
            return retcode;
        }
        data.resize((len-1));
        result = data;
        return ERROR_SUCCESS;
    }
}

#endif //REGISTRY_HPP
