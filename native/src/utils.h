#pragma once

#include "define.h"

string WStringToString(const wstring &src, UINT codePage = CP_ACP);
wstring StringToWString(const string &src, UINT codePage = CP_ACP);
LSTATUS OpenFile(Env env, Napi::String &result, HWND parent = GetConsoleWindow());
BOOL EnablePrivilege(Env env, const wstring &name);
void Log(Env env, const string &msg);
void Log(Env env, const wstring &msg);

#ifndef GENSHIN_EXPORT_NATIVE_UTILS_H
#define GENSHIN_EXPORT_NATIVE_UTILS_H

#endif //GENSHIN_EXPORT_NATIVE_UTILS_H
