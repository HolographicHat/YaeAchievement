#pragma once

#include <string>
#include <iostream>
#include <napi.h>
#include <Windows.h>
#include <TlHelp32.h>

using std::string, std::wstring, std::cout, std::to_string;
using Napi::Object, Napi::Env, Napi::Function, Napi::Value, Napi::CallbackInfo, Napi::TypeError, Napi::Error;

typedef unsigned char byte;
typedef unsigned long ul;
typedef unsigned long long ull;
