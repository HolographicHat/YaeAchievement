#pragma once
#include "il2cpp-types.h"

// IL2CPP APIs
#define DO_API(r, n, p) extern r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

// Application-specific functions
#define DO_APP_FUNC(a, r, n, p) extern r (*n) p
namespace Genshin {
#include "il2cpp-functions.h"
}
#undef DO_APP_FUNC
