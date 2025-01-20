// ReSharper disable CppClangTidyCertErr33C
#include <Windows.h>
#include <print>
#include <string>
#include <future>
#include <TlHelp32.h>

#include "globals.h"
#include "util.h"
#include "il2cpp-init.h"
#include "il2cpp-types.h"
#include "ntprivate.h"

CRITICAL_SECTION CriticalSection;
void SetBreakpoint(HANDLE thread, uintptr_t address, bool enable, uint8_t index);

namespace
{
	PacketType GetPacketType(const PacketMeta* packet)
	{
		using namespace Globals;
		const auto cmdid = packet->CmdId;
		
		if (AchievementId && cmdid == AchievementId)
			return PacketType::Achivement;

		if (AchievementIdSet.contains(cmdid) && packet->DataLength > 500)
			return PacketType::Achivement;

		if (PlayerStoreId && cmdid == PlayerStoreId)
			return PacketType::Inventory;

		return PacketType::None;
	}
}

namespace Hook {


	uint16_t __fastcall BitConverter_ToUInt16(Array<uint8_t>* val, const int startIndex) 
	{
		using namespace Globals;
		const auto ToUInt16 = reinterpret_cast<decltype(&BitConverter_ToUInt16)>(Offset.BitConverter_ToUInt16);

		EnterCriticalSection(&CriticalSection);
		SetBreakpoint((HANDLE)-2, 0, false, 0);
		const auto ret = ToUInt16(val, startIndex);
		SetBreakpoint((HANDLE)-2, Offset.BitConverter_ToUInt16, true, 0);
		LeaveCriticalSection(&CriticalSection);

		if (ret != 0xAB89)
			return ret;

		const auto packet = val->As<PacketMeta*>();
		const auto packetType = GetPacketType(packet);
		if (packetType == PacketType::None)
			return ret;

#ifdef _DEBUG
		std::println("PacketType: {}", static_cast<uint8_t>(packetType));
		std::println("CmdId: {}", packet->CmdId);
		std::println("DataLength: {}", packet->DataLength);
		//std::println("Data: {}", Util::Base64Encode(packet->AsSpan()));
#endif

		if (!MessagePipe.Write(packetType))
			Util::Win32ErrorDialog(1002, GetLastError());

		if (!MessagePipe.Write(packet->DataLength))
			Util::Win32ErrorDialog(1003, GetLastError());

		if (!MessagePipe.Write(packet->AsSpan()))
			Util::Win32ErrorDialog(1004, GetLastError());
		
		if (!AchievementsWritten)
			AchievementsWritten = packetType == PacketType::Achivement;

		if (!PlayerStoreWritten)
			PlayerStoreWritten = packetType == PacketType::Inventory;

		if (AchievementsWritten && PlayerStoreWritten && RequiredPlayerProperties.size() == 0)
		{
			if (!MessagePipe.Write(PacketType::End))
				Util::Win32ErrorDialog(9001, GetLastError());
#ifdef _DEBUG
			system("pause");
#endif
			ExitProcess(0);
		}

		return ret;
	}

	void __fastcall AccountDataItem_UpdateNormalProp(const void* __this, const int type, const double value, const double lastValue, const int state)
	{
		using namespace Globals;
		const auto UpdateNormalProp = reinterpret_cast<decltype(&AccountDataItem_UpdateNormalProp)>(Offset.AccountDataItem_UpdateNormalProp);

		EnterCriticalSection(&CriticalSection);
		SetBreakpoint((HANDLE)-2, 0, false, 1);
		UpdateNormalProp(__this, type, value, lastValue, state);
		SetBreakpoint((HANDLE)-2, Offset.AccountDataItem_UpdateNormalProp, true, 1);
		LeaveCriticalSection(&CriticalSection);

#ifdef _DEBUG
		std::println("PropType: {}", type);
		std::println("PropState: {}", state);
		std::println("PropValue: {}", value);
		std::println("PropLastValue: {}", lastValue);
#endif
		if (RequiredPlayerProperties.erase(type) != 0)
		{
			if (!MessagePipe.Write(PacketType::PropData))
				Util::Win32ErrorDialog(2002, GetLastError());
			if (!MessagePipe.Write(type))
				Util::Win32ErrorDialog(2003, GetLastError());
			if (!MessagePipe.Write(value))
				Util::Win32ErrorDialog(2004, GetLastError());
		}

		if (AchievementsWritten && PlayerStoreWritten && RequiredPlayerProperties.size() == 0)
		{
			if (!MessagePipe.Write(PacketType::End))
				Util::Win32ErrorDialog(9001, GetLastError());
#ifdef _DEBUG
			system("pause");
#endif
			ExitProcess(0);
		}
	}
}

LONG __stdcall VectoredExceptionHandler(PEXCEPTION_POINTERS ep)
{
	using namespace Globals;
	const auto exceptionRecord = ep->ExceptionRecord;
	const auto contextRecord = ep->ContextRecord;

	if (exceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		if (exceptionRecord->ExceptionAddress == reinterpret_cast<void*>(Offset.BitConverter_ToUInt16)) {
			contextRecord->Rip = reinterpret_cast<DWORD64>(Hook::BitConverter_ToUInt16);
			contextRecord->EFlags &= ~0x100; // clear the trap flag
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (exceptionRecord->ExceptionAddress == reinterpret_cast<void*>(Offset.AccountDataItem_UpdateNormalProp)) {
			contextRecord->Rip = reinterpret_cast<DWORD64>(Hook::AccountDataItem_UpdateNormalProp);
			contextRecord->EFlags &= ~0x100; // clear the trap flag
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void SetBreakpoint(HANDLE thread, uintptr_t address, bool enable, uint8_t index)
{
	using namespace Globals;

	if (index > 3) {
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
	system("pause");
#endif
	InitializeCriticalSection(&CriticalSection);

	auto initFuture = std::async(std::launch::async, InitIL2CPP);

	using namespace Globals;
	const auto pid = GetCurrentProcessId();

    while ((GameWindow = Util::FindMainWindowByPID(pid)) == nullptr) {
		SwitchToThread();
	}

	if (!initFuture.get())
		ExitProcess(0);

	MessagePipe = CreateFileA(R"(\\.\pipe\YaeAchievementPipe)", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (!MessagePipe)
	{
#ifdef _DEBUG
		std::println("CreateFile failed: {}", GetLastError());
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
				SetBreakpoint(hThread, Offset.BitConverter_ToUInt16, true, 0);
				SetBreakpoint(hThread, Offset.AccountDataItem_UpdateNormalProp, true, 1);
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
	// Check injectee
	WCHAR szFileName[MAX_PATH]{};
	DWORD length = 0;
	GetModuleFileNameW(NULL, szFileName, MAX_PATH);
	if (!(wcsstr(szFileName, L"YuanShen.exe") || wcsstr(szFileName, L"GenshinImpact.exe")))
	{
		return TRUE;
	}

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (hInstance)
		{
			LdrAddRefDll(LDR_ADDREF_DLL_PIN, hInstance);
		}

		if (const auto hThread = CreateThread(nullptr, 0, ThreadProc, hInstance, 0, nullptr)) {
			CloseHandle(hThread);
		}
	}

	return TRUE;
}

static LRESULT WINAPI YaeGetWindowHookImpl(int code, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(NULL, code, wParam, lParam);
}

EXTERN_C __declspec(dllexport) HRESULT WINAPI YaeGetWindowHook(_Out_ HOOKPROC* pHookProc)
{
	*pHookProc = YaeGetWindowHookImpl;
	return S_OK;
}