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

void InitIL2CPP() {
	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(nullptr, szFileName, MAX_PATH);
	const auto isCN = strstr(szFileName, "YuanShen.exe");
	const auto uBase = reinterpret_cast<uint64_t>(GetModuleHandle(isCN ? "YuanShen.exe" : "GenshinImpact.exe"));
	#define DO_APP_FUNC(ca, oa, r, n, p) n = (r (*) p)(uBase + (isCN ? ca : oa))
	#include "il2cpp-functions.h"
	#undef DO_APP_FUNC
}
