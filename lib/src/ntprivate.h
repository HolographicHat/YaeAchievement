#pragma once
#include <Windows.h>
#include <bcrypt.h>

#pragma comment(lib, "ntdll.lib")

#define LDR_ADDREF_DLL_PIN 0x00000001

EXTERN_C NTSYSAPI NTSTATUS NTAPI LdrAddRefDll(_In_ ULONG Flags, _In_ PVOID DllHandle);