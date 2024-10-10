#include <Windows.h>
#include <string>
#include "globals.h"

void InitIL2CPP()
{
	std::string buffer;
	buffer.resize(MAX_PATH);
	ZeroMemory(buffer.data(), MAX_PATH);
	const auto pathLength = GetModuleFileNameA(nullptr, buffer.data(), MAX_PATH);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		buffer.resize(pathLength);
		ZeroMemory(buffer.data(), pathLength);
		GetModuleFileNameA(nullptr, buffer.data(), pathLength);
	}
	buffer.shrink_to_fit();

	using namespace Globals;
	IsCNREL = buffer.find("YuanShen.exe") != std::string::npos;
	BaseAddress = (uintptr_t)GetModuleHandleA(nullptr);

	if (Offset.BitConverter_ToUInt16 != 0) {
		Offset.BitConverter_ToUInt16 += BaseAddress;
	}

#ifdef _DEBUG
	printf("BaseAddress: 0x%llX\n", BaseAddress);
	printf("IsCNREL: %d\n", IsCNREL);
	printf("BitConverter_ToUInt16: 0x%llX\n", Offset.BitConverter_ToUInt16);
#endif
}
