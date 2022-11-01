#pragma once
#include "il2cpp-types.h"

// IL2CPP APIs
#define DO_API(ca, oa, r, n, p) extern r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

// Application-specific functions
#define DO_APP_FUNC(ca, oa, r, n, p) extern r (*n) p
namespace Genshin {
#include "il2cpp-functions.h"
}
#undef DO_APP_FUNC

#define DO_UNI_FUNC(ca, oa, r, n, p) extern r (*n) p
namespace Genshin {
#include "il2cpp-unity-functions.h"
}
#undef DO_UNI_FUNC
