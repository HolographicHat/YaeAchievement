// ReSharper disable CppClangTidyBugproneMacroParentheses

#include "pch.h"

#include "il2cpp-init.h"

#define DO_APP_FUNC(ca, oa, r, n, p) r (*n) p
#define DO_UNI_FUNC(ca, oa, r, n, p) r (*n) p
namespace Genshin {
#include "il2cpp-functions.h"
}
#undef DO_UNI_FUNC
#undef DO_APP_FUNC

#define DO_TYPEDEF(ca, oa, n) n##__Class **n##__TypeInfo
namespace Genshin {
#include "il2cpp-types-ptr.h"
}
#undef DO_TYPEDEF

using std::string;

void InitIL2CPP() {
	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	auto isCN = strstr(szFileName, "YuanShen.exe");//string(szFileName).contains();
	auto hBase = GetModuleHandle("UserAssembly.dll");
	auto bAddr = (UINT64)hBase;
	auto cAddr = (UINT64)GetModuleHandle("UnityPlayer.dll");
	#define DO_APP_FUNC(ca, oa, r, n, p) n = (r (*) p)(bAddr + (isCN ? ca : oa))
	#define DO_UNI_FUNC(ca, oa, r, n, p) n = (r (*) p)(cAddr + (isCN ? ca : oa))
	#include "il2cpp-functions.h"
	#undef DO_UNI_FUNC
	#undef DO_APP_FUNC
	#define DO_TYPEDEF(ca, oa, n) n##__TypeInfo = (n##__Class **)(bAddr + (isCN ? ca : oa))
	#include "il2cpp-types-ptr.h"
	#undef DO_TYPEDEF
}
