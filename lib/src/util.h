// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken
#pragma once
#include <Windows.h>
#include <type_traits>
#include <vector>
#include <span>

namespace Util
{
	HWND FindMainWindowByPID(DWORD pid);
	std::string Base64Encode(std::span<uint8_t> data);
	std::string Base64Encode(uint8_t const* buf, size_t bufLen);

	void ErrorDialog(LPCSTR title, LPCSTR msg);
	void ErrorDialog(LPCSTR msg);
	void Win32ErrorDialog(DWORD code, DWORD winerrcode);

	std::vector<uintptr_t> PatternScanAll(std::span<uint8_t> bytes, const char* pattern);
}