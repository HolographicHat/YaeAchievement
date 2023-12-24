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
	
	void OnAchievementAllDataNotify(LPVOID obj, const LPVOID ntf) {
		const auto cos = Genshin::il2cpp_object_new(*Genshin::CodedOutputStream__TypeInfo);
		const auto len = Genshin::CalculateSize(ntf);
		const auto buf = (ByteArray*) new uint8_t[0x20 + len] {};
		buf->max_length = len;
		Genshin::CodedOutputStreamInit(cos, buf, 0, len);
		Genshin::ProtoWriteTo(ntf, cos);
		const auto str = base64_encode(buf->vector, len) + "\n";
		WriteFile(hPipe, str.c_str(), (DWORD) str.length(), nullptr, nullptr);
		CloseHandle(hPipe);
		ExitProcess(0);
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
	HookManager::install(Genshin::OnAchievementAllDataNotify, Hook::OnAchievementAllDataNotify);
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
