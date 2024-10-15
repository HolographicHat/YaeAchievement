#include <string>
#include "util.h"
#include "globals.h"

#ifdef _DEBUG
#pragma runtime_checks("", off)
#endif

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

	std::tuple<std::vector<uint8_t>, std::vector<bool>> PatternToBytes(const char* pattern)
	{
		std::vector<uint8_t> bytes;
		std::vector<bool> maskBytes;

		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
				maskBytes.push_back(false);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
				maskBytes.push_back(true);
			}
		}
		return { bytes, maskBytes };
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

	uintptr_t PatternScan(uintptr_t start, uintptr_t end, const char* pattern)
	{
		const auto [patternBytes, patternMask] = PatternToBytes(pattern);
		const auto scanBytes = reinterpret_cast<uint8_t*>(start);

		const auto patternSize = patternBytes.size();
		const auto pBytes = patternBytes.data();

		for (auto i = 0ul; i < end - start - patternSize; ++i) {
			bool found = true;
			for (auto j = 0ul; j < patternSize; ++j) {
				if (scanBytes[i + j] != pBytes[j] && patternMask[j]) {
					found = false;
					break;
				}
			}
			if (found) {
				return reinterpret_cast<uintptr_t>(&scanBytes[i]);
			}
		}

		return 0;
	}

	std::vector<uintptr_t> PatternScanAll(uintptr_t start, uintptr_t end, const char* pattern)
	{
		std::vector<uintptr_t> results;
		const auto [patternBytes, patternMask] = PatternToBytes(pattern);
		const auto scanBytes = reinterpret_cast<uint8_t*>(start);

		const auto patternSize = patternBytes.size();
		const auto pBytes = patternBytes.data();

		for (auto i = 0ul; i < end - start - patternSize; ++i) {
			bool found = true;
			for (auto j = 0ul; j < patternSize; ++j) {
				if (scanBytes[i + j] != pBytes[j] && patternMask[j]) {
					found = false;
					break;
				}
			}
			if (found) {
				results.push_back(reinterpret_cast<uintptr_t>(&scanBytes[i]));
				i += patternSize - 1;
			}
		}

		return results;
	}
}

#ifdef _DEBUG
#pragma runtime_checks("", restore)
#endif