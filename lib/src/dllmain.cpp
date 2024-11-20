// ReSharper disable CppClangTidyCertErr33C
#include <Windows.h>
#include <string>
#include <future>
#include <TlHelp32.h>

#include "globals.h"
#include "util.h"
#include "il2cpp-init.h"
#include "il2cpp-types.h"

CRITICAL_SECTION CriticalSection;
void SetBreakpoint(HANDLE thread, uintptr_t address, bool enable, uint8_t index = 0);

namespace Hook {


	uint16_t __fastcall BitConverter_ToUInt16(Array<uint8_t>* val, const int startIndex) 
	{
		using namespace Globals;
		const auto ToUInt16 = reinterpret_cast<decltype(&BitConverter_ToUInt16)>(Offset.BitConverter_ToUInt16);

		EnterCriticalSection(&CriticalSection);
		SetBreakpoint((HANDLE)-2, 0, false);
		const auto ret = ToUInt16(val, startIndex);
		SetBreakpoint((HANDLE)-2, Offset.BitConverter_ToUInt16, true);
		LeaveCriticalSection(&CriticalSection);

		const auto packet = reinterpret_cast<PacketMeta*>(val->data());

		auto CheckPacket = [](const PacketMeta* packet) -> bool {
			const auto cmdid = _byteswap_ushort(packet->CmdId);
			const auto dataLength = _byteswap_ulong(packet->DataLength);

			if (dataLength < 500) {
				return false;
			}

			if (CmdId != 0) {
				return cmdid == CmdId;
			}

			return DynamicCmdIds.contains(cmdid);
		};

		using namespace Globals;
		if (ret == 0xAB89 && CheckPacket(packet))
		{
			const auto headLength = _byteswap_ushort(packet->HeaderLength);
			const auto dataLength = _byteswap_ulong(packet->DataLength);

			printf("CmdId: %d\n", _byteswap_ushort(packet->CmdId));
			printf("DataLength: %d\n", dataLength);

			const auto base64 = Util::Base64Encode(packet->Data + headLength, dataLength) + "\n";
			printf("Base64: %s\n", base64.c_str());

#ifdef _DEBUG
			system("pause");
#endif

			WriteFile(MessagePipe, base64.c_str(), (DWORD)base64.length(), nullptr, nullptr);
			CloseHandle(MessagePipe);
			ExitProcess(0);
		}

		return ret;
	}
}

LONG __stdcall VectoredExceptionHandler(PEXCEPTION_POINTERS ep)
{
	using namespace Globals;
	const auto exceptionRecord = ep->ExceptionRecord;
	const auto contextRecord = ep->ContextRecord;

	if (exceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		if (exceptionRecord->ExceptionAddress != reinterpret_cast<void*>(Offset.BitConverter_ToUInt16)) {
			return EXCEPTION_CONTINUE_SEARCH;
		}

		contextRecord->Rip = reinterpret_cast<DWORD64>(Hook::BitConverter_ToUInt16);
		contextRecord->EFlags &= ~0x100; // clear the trap flag
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void SetBreakpoint(HANDLE thread, uintptr_t address, bool enable, uint8_t index)
{
	using namespace Globals;

	if (index > 3) {
		return;
	}

	if (!BaseAddress || Offset.BitConverter_ToUInt16 <= BaseAddress) {
		// not initialized yet
		return;
	}

	CONTEXT ctx{};
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(thread, &ctx);

	DWORD64* dr = &ctx.Dr0;
	dr[index] = enable ? address : 0;

	const auto mask = 1ull << (index * 2);
	ctx.Dr7 |= mask;

	SetThreadContext(thread, &ctx);
}

DWORD __stdcall ThreadProc(LPVOID hInstance) 
{
#ifdef _DEBUG
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif
	InitializeCriticalSection(&CriticalSection);

	auto initFuture = std::async(std::launch::async, InitIL2CPP);

	using namespace Globals;
	const auto pid = GetCurrentProcessId();

    while ((GameWindow = Util::FindMainWindowByPID(pid)) == nullptr) {
		SwitchToThread();
	}

	initFuture.get();

	MessagePipe = CreateFileA(R"(\\.\pipe\YaeAchievementPipe)", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (MessagePipe == INVALID_HANDLE_VALUE)
	{
#ifdef _DEBUG
		printf("CreateFile failed: %d\n", GetLastError());
#else
		Util::Win32ErrorDialog(1001, GetLastError());
		ExitProcess(0);
#endif
	}

	AddVectoredExceptionHandler(1, VectoredExceptionHandler);
	while (true)
	{
		THREADENTRY32 te32{};
		te32.dwSize = sizeof(THREADENTRY32);
		const auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		for (Thread32First(hSnapshot, &te32); Thread32Next(hSnapshot, &te32);)
		{
			if (te32.th32OwnerProcessID != pid) {
				continue;
			}

			if (const auto hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID))
			{
				EnterCriticalSection(&CriticalSection);
				SetBreakpoint(hThread, Offset.BitConverter_ToUInt16, true);
				CloseHandle(hThread);
				LeaveCriticalSection(&CriticalSection);
			}
		}
		CloseHandle(hSnapshot);
		Sleep(1);
	}

	return 0;
}

// DLL entry point
BOOL __stdcall DllMain(HMODULE hInstance, DWORD fdwReason, LPVOID lpReserved)
{

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (const auto hThread = CreateThread(nullptr, 0, ThreadProc, hInstance, 0, nullptr)) {
			CloseHandle(hThread);
		}
	}

	return TRUE;
}
