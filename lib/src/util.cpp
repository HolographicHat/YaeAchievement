#include <string>
#include <array>
#include <ranges>
#include <intrin.h>
#include "util.h"

#include "globals.h"

#ifdef _DEBUG
#pragma runtime_checks("", off)
#endif

#pragma region FindMainWindowByPID

namespace
{
	struct HandleData {
		DWORD Pid;
		HWND Hwnd;
	};

	bool IsMainWindow(HWND handle) {
		return GetWindow(handle, GW_OWNER) == nullptr && IsWindowVisible(handle) == TRUE;
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
		if (data.Pid != pid || !IsMainWindow(handle) || !IsUnityWindow(handle))
			return TRUE;
		data.Hwnd = handle;
		return FALSE;
	}

	std::tuple<std::vector<uint8_t>, std::string> PatternToBytes(const char* pattern)
	{
		std::vector<uint8_t> bytes;
		std::string mask;

		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
				mask.push_back('?');
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
				mask.push_back('x');
			}
		}
		return { bytes, mask };
	}

}

#pragma endregion

static constexpr LPCSTR base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

namespace Util
{
	HWND FindMainWindowByPID(DWORD pid)
	{
		HandleData data = { 
			.Pid = pid,
			.Hwnd = nullptr
		};
		EnumWindows(EnumWindowsCallback, (LPARAM)&data);
		return data.Hwnd;
	}

	std::string Base64Encode(std::span<uint8_t> data)
	{
		return Base64Encode(data.data(), data.size());
	}

	std::string Base64Encode(uint8_t const* buf, size_t bufLen)
	{
		std::string ret;
		int i = 0;
		uint8_t char_array_3[3];
		uint8_t char_array_4[4];
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

	std::vector<uintptr_t> PatternScanAll(std::span<uint8_t> bytes, const char* pattern)
	{
		std::vector<uintptr_t> results;
		const auto [patternBytes, patternMask] = PatternToBytes(pattern);
		constexpr std::size_t chunkSize = 16;

		const auto maskCount = static_cast<std::size_t>(std::ceil(patternMask.size() / chunkSize));
		std::array<int32_t, 32> masks{};

		auto chunks = patternMask | std::views::chunk(chunkSize);
		for (std::size_t i = 0; auto chunk : chunks) {
			int32_t mask = 0;
			for (std::size_t j = 0; j < chunk.size(); ++j) {
				if (chunk[j] == 'x') {
					mask |= 1 << j;
				}
			}
			masks[i++] = mask;
		}

		__m128i xmm1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(patternBytes.data()));
		__m128i xmm2, xmm3, mask;

		auto pData = bytes.data();
		const auto end = pData + bytes.size() - patternMask.size();

		while (pData < end)
		{
			_mm_prefetch(reinterpret_cast<const char*>(pData + 64), _MM_HINT_NTA);

			if (patternBytes[0] == pData[0])
			{
				xmm2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pData));
				mask = _mm_cmpeq_epi8(xmm1, xmm2);

				if ((_mm_movemask_epi8(mask) & masks[0]) == masks[0])
				{
					bool found = true;
					for (int i = 1; i < maskCount; ++i)
					{
						xmm2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pData + i * chunkSize));
						xmm3 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(patternBytes.data() + i * chunkSize));
						mask = _mm_cmpeq_epi8(xmm2, xmm3);
						if ((_mm_movemask_epi8(mask) & masks[i]) != masks[i])
						{
							found = false;
							break;
						}
					}


					if (found) {
						results.push_back(reinterpret_cast<uintptr_t>(pData));
					}

				}


			}

			++pData;
		}

		return results;
	}
}

#ifdef _DEBUG
#pragma runtime_checks("", restore)
#endif