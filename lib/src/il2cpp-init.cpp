#include "pch.h"

#include "il2cpp-init.h"

#define DO_API(r, n, p) r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

#define DO_APP_FUNC(ca, oa, r, n, p) r (*n) p
namespace Genshin {
#include "il2cpp-functions.h"
}
#undef DO_APP_FUNC

#define DO_UNI_FUNC(ca, oa, r, n, p) r (*n) p
namespace Genshin {
#include "il2cpp-unity-functions.h"
}
#undef DO_UNI_FUNC

using std::string;

UINT64 GetAddressByExports(HMODULE base, const char* name) {
	UINT64 funcAddr = reinterpret_cast<UINT64>(GetProcAddress(base, name));
	return funcAddr == 0 ? 0 : funcAddr;
}

void InitIL2CPP() {
	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	auto isCN = strstr(szFileName, "YuanShen.exe");//string(szFileName).contains();
	auto hBase = GetModuleHandle("UserAssembly.dll");
	auto bAddr = (UINT64)hBase;
	auto cAddr = (UINT64)GetModuleHandle("UnityPlayer.dll");
	#define DO_API(r, n, p) n = (r (*) p) GetAddressByExports(hBase, #n);
	#include "il2cpp-api-functions.h"
	#undef DO_API
	#define DO_APP_FUNC(ca, oa, r, n, p) n = (r (*) p)(bAddr + (isCN ? ca : oa))
	#include "il2cpp-functions.h"
	#undef DO_APP_FUNC
	#define DO_UNI_FUNC(ca, oa, r, n, p) n = (r (*) p)(cAddr + (isCN ? ca : oa))
	#include "il2cpp-unity-functions.h"
	#undef DO_UNI_FUNC
}
