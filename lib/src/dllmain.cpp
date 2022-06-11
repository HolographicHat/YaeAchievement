#include "pch.h"
#include "util.h"
#include "il2cpp-init.h"
#include "HookManager.h"

HWND unityWnd = 0;

std::string il2cppi_to_string(Il2CppString* str, UINT codePage) {
	auto chars = reinterpret_cast<const wchar_t *>(str->chars);
	auto len = WideCharToMultiByte(codePage, 0, chars, -1, nullptr, 0, nullptr, nullptr);
	auto buffer = new CHAR[len];
	WideCharToMultiByte(codePage, 0, chars, -1, buffer, len, nullptr, nullptr);
	std::string strTemp(buffer);
	delete[] buffer;
	return strTemp;
}

void SetDispstchData(Genshin::ByteArray* data, MethodInfo* m) {
	CALL_ORIGIN(SetDispstchData, data, m);
	auto ilStr = Genshin::Convert_ToBase64String(data, nullptr);
	auto cStr = il2cppi_to_string(reinterpret_cast<Il2CppString*>(ilStr), CP_ACP);
	ErrorDialog(cStr.c_str());
	return;
}

void Run(HMODULE* phModule) {
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	while (
		GetModuleHandle("UserAssembly.dll") == nullptr || 
		(unityWnd = FindMainWindowByPID(GetCurrentProcessId())) == 0
	) {
		printf("Wait game initialize...\n");
		Sleep(1000);
	}
	InitIL2CPP();
	HookManager::install(Genshin::Packet_SetDispstchData, SetDispstchData);
	/*HANDLE hPipe = CreateFile(R"(\\.\pipe\YaeAchievementPipe)", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hPipe == INVALID_HANDLE_VALUE) {
		printf("Failed to open pipe: %d\n", GetLastError());
		ExitProcess(0);
		return;
	}*/
	//ErrorDialog("Press ok to continue.");
	//ConnectNamedPipe(hPipe, nullptr);
	//DWORD written;
	//WriteFile(hPipe, (checksum + "\n").c_str(), checksum.length() + 1, &written, nullptr);
	//DisconnectNamedPipe(hPipe);
	//CloseHandle(hPipe);
	//ExitProcess(0);
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
