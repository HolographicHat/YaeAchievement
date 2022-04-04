#pragma once

#include <string>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <node_api.h>

using std::string, std::wstring, std::cout, std::endl;

typedef unsigned char byte;
typedef unsigned long long ull;

typedef napi_env Env;
typedef napi_value Value;
typedef napi_status Status;
typedef napi_callback Callback;
typedef napi_callback_info CallbackInfo;

#define GetBoolean       napi_get_boolean
#define ThrowError       napi_throw_error
#define GetUTF8String    napi_get_value_string_utf8
#define CreateFunction   napi_create_function
#define GetCallbackInfo  napi_get_cb_info
#define CreateUTF8String napi_create_string_utf8
#define SetNamedProperty napi_set_named_property
