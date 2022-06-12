#include "pch.h"
#include "util.h"
#include "il2cpp-init.h"

using Genshin::ByteArray, std::to_string;

HWND unityWnd = 0;
HANDLE hPipe  = 0;

namespace Hook {

	std::set<UINT16> PacketWhitelist = { 109, 131, 141, 2668 };

	void Packet_Xor(ByteArray** src, int len, MethodInfo* m) {
		auto data = (ByteArray*)new BYTE[len + 32];
		data->max_length = len;
		memcpy(data->vector, (*src)->vector, len);
		if (ReadMapped<UINT16>(data->vector, 0) != 0x4567) {
			CALL_ORIGIN(Packet_Xor, &data, len, m);
		}
		if (ReadMapped<UINT16>(data->vector, 2) == 2668) {
			auto headLength = ReadMapped<UINT16>(data->vector, 4);
			auto dataLength = ReadMapped<UINT32>(data->vector, 6);
			auto iStr = Genshin::Convert_ToBase64String(data, 10 + headLength, dataLength, nullptr);
			auto cStr = IlStringToString(reinterpret_cast<Il2CppString*>(iStr)) + "\n";
			WriteFile(hPipe, cStr.c_str(), cStr.length(), nullptr, nullptr);
			CloseHandle(hPipe);
			ExitProcess(0);
		}
		delete[] data;
		CALL_ORIGIN(Packet_Xor, src, len, m);
		return;
	}
}

void Run(HMODULE* phModule) {
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	while (
		GetModuleHandle("UserAssembly.dll") == nullptr ||
		(unityWnd = FindMainWindowByPID(GetCurrentProcessId())) == 0
	) {
		Sleep(1000);
	}
	InitIL2CPP();
	HookManager::install(Genshin::Packet_Xor, Hook::Packet_Xor);
	hPipe = CreateFile(R"(\\.\pipe\YaeAchievementPipe)", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hPipe == INVALID_HANDLE_VALUE) {
		Win32ErrorDialog(1001);
		ExitProcess(0);
		return;
	}
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved) {
	switch (ulReasonForCall) {
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Run, new HMODULE(hModule), 0, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
