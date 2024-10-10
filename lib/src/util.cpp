#include <string>
#include "util.h"
#include "globals.h"

#pragma region FindMainWindowByPID

namespace
{
	struct HandleData {
		DWORD pid;
		HWND hwnd;
	};

	bool IsMainWindow(HWND handle) {
		return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle) == TRUE;
	}

	bool IsUnityWindow(HWND handle) {
		char szName[256]{};
		GetClassNameA(handle, szName, 256);
		return _stricmp(szName, "UnityWndClass") == 0;
	}

	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam) {
		HandleData& data = *(HandleData*)lParam;
		DWORD pid = 0;
		GetWindowThreadProcessId(handle, &pid);
		if (data.pid != pid || !IsMainWindow(handle) || !IsUnityWindow(handle))
			return TRUE;
		data.hwnd = handle;
		return FALSE;
	}
}

#pragma endregion

static constexpr LPCSTR base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

namespace Util
{
	HWND FindMainWindowByPID(DWORD pid)
	{
		HandleData data = { pid, 0 };
		EnumWindows(EnumWindowsCallback, (LPARAM)&data);
		return data.hwnd;
	}

	std::string Base64Encode(BYTE const* buf, unsigned int bufLen)
	{
		std::string ret;
		int i = 0;
		BYTE char_array_3[3];
		BYTE char_array_4[4];
		while (bufLen--) {
			char_array_3[i++] = *buf++;
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;
				for (i = 0; (i < 4); i++)
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}
		if (i) {
			int j;
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for (j = 0; j < i + 1; j++)
				ret += base64_chars[char_array_4[j]];
			while (i++ < 3)
				ret += '=';
		}
		return ret;
	}

	void ErrorDialog(LPCSTR title, LPCSTR msg)
	{
		MessageBoxA(Globals::GameWindow, msg, title, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}

	void ErrorDialog(LPCSTR msg)
	{
		ErrorDialog("YaeAchievement", msg);
	}

	void Win32ErrorDialog(DWORD code, DWORD winerrcode)
	{
		const std::string msg = "CRITICAL ERROR!\nError code: " + std::to_string(winerrcode) + "-" + std::to_string(code) +
			"\n\nPlease take the screenshot and contact developer by GitHub Issue to solve this problem\nNOT MIHOYO/COGNOSPHERE CUSTOMER SERVICE!";

		ErrorDialog("YaeAchievement", msg.c_str());
	}
}