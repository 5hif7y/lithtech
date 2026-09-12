#pragma once
// LithTech Jupiter - Math shim para gcc Linux
// Mapea funciones MSVC/lt* a estándar C++17
// Este header se inyecta vía -include en todo el build Linux, por lo que debe contener shims críticos antes de que ltbasedefs.h/ltlink.h los necesiten
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cwchar>
#include <wchar.h>
#include <cstddef>
#include <cstring>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#ifndef _MAX_PATH
#define _MAX_PATH MAX_PATH
#endif
#ifndef MODULE_EXPORT
#define MODULE_EXPORT
#endif
#ifndef __forceinline
#define __forceinline inline
#endif
#ifndef __inline
#define __inline inline
#endif
#ifndef force_inline
#define force_inline inline
#endif
#ifndef local_force_inline
#define local_force_inline inline
#endif
#ifndef _vsnprintf
#define _vsnprintf vsnprintf
#endif
#ifndef _snprintf
#define _snprintf snprintf
#endif
#ifndef _vsnwprintf
#define _vsnwprintf vswprintf
#endif

#ifndef ltsinf
#define ltsinf(a) sinf(a)
#endif
#ifndef ltcosf
#define ltcosf(a) cosf(a)
#endif
#ifndef ltsin
#define ltsin(a) sin(a)
#endif
#ifndef ltcos
#define ltcos(a) cos(a)
#endif
#ifndef lttanf
#define lttanf(a) tanf(a)
#endif
#ifndef ltsqrtf
#define ltsqrtf(a) sqrtf(a)
#endif
#ifndef ltatan2f
#define ltatan2f(a,b) atan2f(a,b)
#endif

// MSVC _isnan / _finite -> std::isnan / std::isfinite
#include <cfloat>
#ifndef _isnan
#define _isnan std::isnan
#endif
#ifndef _finite
#define _finite std::isfinite
#endif

#ifndef SCREEN_NEAR_Z
#define SCREEN_NEAR_Z 0.0f
#endif
#ifndef SCREEN_FAR_Z
#define SCREEN_FAR_Z 1.0f
#endif

// Portable new-array fallback (graphify effects demo, Windows+Unix)
#include <string>
#ifndef debug_newa
#define debug_newa(t,c) new t[c]
#endif
#ifndef debug_new
#define debug_new(t) new t
#endif
#ifndef debug_deletea
#define debug_deletea(p) delete [] (p)
#endif
#ifndef debug_delete
#define debug_delete(p) delete (p)
#endif
