// ReSharper disable CppClangTidyBugproneMacroParentheses

#pragma once
#include "il2cpp-types.h"

// Application-specific functions
#define DO_APP_FUNC(ca, oa, r, n, p) extern r (*n) p
#define DO_UNI_FUNC(ca, oa, r, n, p) extern r (*n) p
namespace Genshin {
#include "il2cpp-functions.h"
}
#undef DO_UNI_FUNC
#undef DO_APP_FUNC

#define DO_TYPEDEF(ca, oa, n) extern n##__Class **n##__TypeInfo
namespace Genshin {
#include "il2cpp-types-ptr.h"
}
#undef DO_TYPEDEF