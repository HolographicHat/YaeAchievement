#include "pch.h"
#include "util.h"

void Run(HMODULE* phModule) {
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	HWND unityWnd = 0;
	while (
		GetModuleHandle("UserAssembly.dll") == nullptr || 
		(unityWnd = FindMainWindowByPID(GetCurrentProcessId())) == 0
	) {
		printf("Wait game initialize...\n");
		Sleep(1000);
	}
	MessageBox(unityWnd, "YaeAchievementLib injection test on 原神 Beta (CNCBWIN2.7.53)", "", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	//init_il2cpp();
	//HookManager::install(app::Unity_RecordUserData, OnRecordUserData);
	for (int i = 0; i < 4; i++) {
		//app::Application_RecordUserData(i, nullptr);
	}
	HANDLE hPipe = CreateFile(R"(\\.\pipe\genshin-checksum)", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hPipe == INVALID_HANDLE_VALUE) {
		printf("Failed to open pipe: %d\n", GetLastError());
		ExitProcess(0);
		return;
	}
	ConnectNamedPipe(hPipe, nullptr);
	DWORD written;
	//WriteFile(hPipe, (checksum + "\n").c_str(), checksum.length() + 1, &written, nullptr);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
	ExitProcess(0);
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
