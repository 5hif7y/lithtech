#pragma once
// LithTech Jupiter - Math shim para gcc Linux
// Mapea funciones MSVC/lt* a estándar C++17
#include <cmath>

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
