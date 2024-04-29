// ReSharper disable CppClangTidyBugproneMacroParentheses

#pragma once
#include "il2cpp-types.h"

// Application-specific functions
#define DO_APP_FUNC(ca, oa, r, n, p) extern r (*n) p
namespace Genshin {
#include "il2cpp-functions.h"
}
#undef DO_APP_FUNC
