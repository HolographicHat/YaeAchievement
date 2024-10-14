// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken
#pragma once
#include <Windows.h>
#include <type_traits>
#include <vector>

namespace Util
{
	HWND FindMainWindowByPID(DWORD pid);
	std::string Base64Encode(BYTE const* buf, unsigned int bufLen);

	void ErrorDialog(LPCSTR title, LPCSTR msg);
	void ErrorDialog(LPCSTR msg);
	void Win32ErrorDialog(DWORD code, DWORD winerrcode);

	uintptr_t PatternScan(uintptr_t start, uintptr_t end, const char* pattern);
	std::vector<uintptr_t> PatternScanAll(uintptr_t start, uintptr_t end, const char* pattern);
}