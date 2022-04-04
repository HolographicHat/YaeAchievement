#pragma once

#include "define.h"

string WStringToString(const wstring &src);
wstring StringToWString(const string &src);
LSTATUS OpenFile(Env env, Value &result, HWND parent = GetConsoleWindow());
Status RegisterFunction(Env env, Value exports, Callback cb, const string &name);
BOOL EnablePrivilege(const wstring &name);

#ifndef GENSHIN_EXPORT_NATIVE_UTILS_H
#define GENSHIN_EXPORT_NATIVE_UTILS_H

#endif //GENSHIN_EXPORT_NATIVE_UTILS_H
