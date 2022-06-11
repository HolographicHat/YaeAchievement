#include "pch.h"

#include "il2cpp-init.h"

#define DO_API(r, n, p) r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

#define DO_APP_FUNC(a, r, n, p) r (*n) p
namespace Genshin {
#include "il2cpp-functions.h"
}
#undef DO_APP_FUNC

using std::string;

ull GetAddressByExports(HMODULE base, const char* name) {
	ull funcAddr = reinterpret_cast<ull>(GetProcAddress(base, name));
	return funcAddr == 0 ? 0 : funcAddr;
}

ull milliseconds_now() {
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	} else {
		return GetTickCount64();
	}
}

void InitIL2CPP() {
	auto start = milliseconds_now();
	auto hBase = GetModuleHandle("UserAssembly.dll");
	auto bAddr = (ull)hBase;
	#define DO_API(r, n, p) n = (r (*) p) GetAddressByExports(hBase, #n);
	#include "il2cpp-api-functions.h"
	#undef DO_API
	#define DO_APP_FUNC(a, r, n, p) n = (r (*) p)(bAddr + a)
	#include "il2cpp-functions.h"
	#undef DO_APP_FUNC
	printf("Initialized in %llu ms.\n", milliseconds_now() - start);
}
