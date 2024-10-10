// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken
#pragma once
#include <Windows.h>
#include <type_traits>


namespace Util
{
	HWND FindMainWindowByPID(DWORD pid);
	std::string Base64Encode(BYTE const* buf, unsigned int bufLen);

	void ErrorDialog(LPCSTR title, LPCSTR msg);
	void ErrorDialog(LPCSTR msg);
	void Win32ErrorDialog(DWORD code, DWORD winerrcode);

	
}