// ReSharper disable CppCStyleCast
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyBugproneMacroParentheses
// ReSharper disable CppClangTidyClangDiagnosticCastAlign

#include "pch.h"

#include "il2cpp-init.h"

#define DO_APP_FUNC(ca, oa, r, n, p) r (*n) p
namespace Genshin {
#include "il2cpp-functions.h"
}
#undef DO_APP_FUNC

using std::string;

void InitIL2CPP(void* &ppRecordUserData) {
	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(nullptr, szFileName, MAX_PATH);
	const auto isCN = strstr(szFileName, "YuanShen.exe");
	const auto uBase = reinterpret_cast<uint64_t>(GetModuleHandle("UserAssembly.dll"));
	#define DO_APP_FUNC(ca, oa, r, n, p) n = (r (*) p)(uBase + (isCN ? ca : oa))
	#include "il2cpp-functions.h"
	#undef DO_APP_FUNC
	auto sPtr = reinterpret_cast<uint8_t*>(RecordUserData);
	for (int i = 0; i < 0x64; ++i) {
		if ((*(uint32_t*) sPtr & 0xFFFFFF) == 0x25FF48) { // 48 FF 25 ??
			ppRecordUserData = sPtr + 7 + *(int*) (sPtr + 3);
			break;
		}
		sPtr += 1;
	}
}
