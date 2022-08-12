#include "pch.h"
#include "util.h"
#include "il2cpp-init.h"

using Genshin::ByteArray, Genshin::ClientKcpEvent, Genshin::KcpPacket, Genshin::KcpEventType;
using std::to_string;

HWND unityWnd = 0;
HANDLE hPipe  = 0;

std::set<UINT16> PacketWhitelist = { 172, 198, 112, 2676, 7, 21, 135 }; // ping, token, loginreq

bool OnPacket(KcpPacket* pkt) {
	if (pkt->data == nullptr) return true;
	auto len = pkt->length;
	auto data = (ByteArray*)new BYTE[len + 32];
	data->max_length = len;
	memcpy(data->vector, pkt->data, len);
	Genshin::Packet_Xor(&data, len, nullptr);
	if (ReadMapped<UINT16>(data->vector, 0) != 0x4567) {
		delete[] data;
		return true;
	}
	if (!PacketWhitelist.contains(ReadMapped<UINT16>(data->vector, 2))) {
		//ifdef _DEBUG
		printf("Blocked cmdid: %d\n", ReadMapped<UINT16>(data->vector, 2));
		//endif
		delete[] data;
		return false;
	}
	printf("Passed cmdid: %d\n", ReadMapped<UINT16>(data->vector, 2));
	if (ReadMapped<UINT16>(data->vector, 2) == 2676) {
		auto headLength = ReadMapped<UINT16>(data->vector, 4);
		auto dataLength = ReadMapped<UINT32>(data->vector, 6);
		auto iStr = Genshin::Convert_ToBase64String(data, 10 + headLength, dataLength, nullptr);
		auto cStr = IlStringToString(iStr) + "\n";
		WriteFile(hPipe, cStr.c_str(), cStr.length(), nullptr, nullptr);
		CloseHandle(hPipe);
		ExitProcess(0);
	}
	delete[] data;
	return true;
}

namespace Hook {

	int Kcp_Send(void* client, KcpPacket* pkt, void* method) {
		return OnPacket(pkt) ? CALL_ORIGIN(Kcp_Send, client, pkt, method) : 0;
	}

	void MonoLoginMainPage__set_version(void* obj, Il2CppString* value, void* method) {
		auto version = IlStringToString(value);
		value = string_new(version + " YaeAchievement");
		CALL_ORIGIN(MonoLoginMainPage__set_version, obj, value, method);
	}

	bool Kcp_Recv(void* client, ClientKcpEvent* evt, void* method) {
		auto result = CALL_ORIGIN(Kcp_Recv, client, evt, method);
		if (result == 0 || evt->fields.type != KcpEventType::EventRecvMsg) {
			return result;
		}
		return OnPacket(evt->fields.packet) ? result : false;
	}

	std::map<INT, UINT> signatures;

	ByteArray* UnityEngine_RecordUserData(INT type) {
		if (signatures.count(type)) {
			return GCHandle_GetObject<ByteArray>(signatures[type]);
		}
		auto result = CALL_ORIGIN(UnityEngine_RecordUserData, type);
		signatures[type] = GCHandle_New(result, true);
		return result;
	}
}

void Run(HMODULE* phModule) {
	//AllocConsole();
	//freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	while (
		GetModuleHandle("UserAssembly.dll") == nullptr ||
		(unityWnd = FindMainWindowByPID(GetCurrentProcessId())) == 0
	) {
		Sleep(1000);
	}
	InitIL2CPP();
	HookManager::install(Genshin::UnityEngine_RecordUserData, Hook::UnityEngine_RecordUserData);
	for (int i = 0; i < 4; i++) {
		Genshin::Application_RecordUserData(i, nullptr);
	}
	HookManager::install(Genshin::Kcp_Send, Hook::Kcp_Send);
	HookManager::install(Genshin::Kcp_Recv, Hook::Kcp_Recv);
	HookManager::install(Genshin::MonoLoginMainPage__set_version, Hook::MonoLoginMainPage__set_version);
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
