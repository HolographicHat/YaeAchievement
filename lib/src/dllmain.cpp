#include "pch.h"
#include "util.h"
#include "il2cpp-init.h"

using Genshin::ByteArray, Genshin::ClientKcpEvent, Genshin::KcpPacket, Genshin::KcpEventType;
using std::to_string;

HWND unityWnd = 0;
HANDLE hPipe  = 0;

std::set<UINT16> PacketWhitelist = { 109, 131, 141, 2668, 32, 17 };

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
		#ifdef _DEBUG
		printf("Blocked cmdid: %d\n", ReadMapped<UINT16>(data->vector, 2));
		#endif
		delete[] data;
		return false;
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
	return true;
}

namespace Hook {

	int Kcp_Send(void* client, KcpPacket* pkt, void* method) {
		return OnPacket(pkt) ? CALL_ORIGIN(Kcp_Send, client, pkt, method) : 0;
	}

	bool Kcp_Recv(void* client, ClientKcpEvent* evt, void* method) {
		auto result = CALL_ORIGIN(Kcp_Recv, client, evt, method);
		if (result == 0 || evt->fields.type != KcpEventType::EventRecvMsg) {
			return result;
		}
		return OnPacket(evt->fields.packet) ? result : false;
	}
}

void Run(HMODULE* phModule) {
	#ifdef _DEBUG
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	#endif
	while (
		GetModuleHandle("UserAssembly.dll") == nullptr ||
		(unityWnd = FindMainWindowByPID(GetCurrentProcessId())) == 0
	) {
		Sleep(1000);
	}
	InitIL2CPP();
	HookManager::install(Genshin::Kcp_Send, Hook::Kcp_Send);
	HookManager::install(Genshin::Kcp_Recv, Hook::Kcp_Recv);
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
