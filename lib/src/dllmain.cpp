// ReSharper disable CppCStyleCast
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseStdPrint
// ReSharper disable CppClangTidyClangDiagnosticCastAlign
// ReSharper disable CppClangTidyHicppMultiwayPathsCovered
// ReSharper disable CppDefaultCaseNotHandledInSwitchStatement
// ReSharper disable CppClangTidyClangDiagnosticCastFunctionTypeStrict

#include "pch.h"
#include "util.h"
#include "il2cpp-init.h"

using Genshin::ByteArray;

HWND unityWnd = nullptr;
HANDLE hPipe  = nullptr;

std::string checksum;

namespace Hook {

	ByteArray* UnityEngine_RecordUserData(const INT type) {
		if (type == 0) {
			const auto arr = new ByteArray {};
			const auto len = checksum.length();
			arr->max_length = len;
			memcpy(&arr->vector[0], checksum.data(), len);
			return arr;
		}
		return new ByteArray {};
	}
	
	uint16_t BitConverter_ToUInt16(ByteArray* val, const int startIndex) {
		const auto ret = CALL_ORIGIN(BitConverter_ToUInt16, val, startIndex);
		if (ret == 0xAB89 && ReadMapped<UINT16>(val->vector, 2) == 20248) {
			const auto headLength = ReadMapped<UINT16>(val->vector, 4);
			const auto dataLength = ReadMapped<UINT32>(val->vector, 6);
			const auto cStr = base64_encode(val->vector + 10 + headLength, dataLength) + "\n";
			WriteFile(hPipe, cStr.c_str(), (DWORD) cStr.length(), nullptr, nullptr);
			CloseHandle(hPipe);
			ExitProcess(0);
		}
		return ret;
	}
}

void Run(HMODULE* phModule) {
	//AllocConsole();
	//freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	while ((unityWnd = FindMainWindowByPID(GetCurrentProcessId())) == nullptr) {
		Sleep(1000);
	}
	Sleep(5000);
	DisableVMProtect();
	InitIL2CPP();
	for (int i = 0; i < 3; i++) {
		const auto result = Genshin::RecordUserData(i);
		checksum += string(reinterpret_cast<char*>(&result->vector[0]), result->max_length);
	}
	HookManager::install(Genshin::BitConverter_ToUInt16, Hook::BitConverter_ToUInt16);
	HookManager::install(Genshin::UnityEngine_RecordUserData, Hook::UnityEngine_RecordUserData);
	hPipe = CreateFile(R"(\\.\pipe\YaeAchievementPipe)", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hPipe == INVALID_HANDLE_VALUE) {
		Win32ErrorDialog(1001);
		ExitProcess(0);
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
