#pragma once
// Centraliza fixes de tipos MSVC para LithTech
#include <cstdint>
#include "Platform/platform.h"

// Asegura que __int64/__int32 estén definidos antes de ltinteger.h
#ifndef __int64
#define __int64 int64_t
#endif
#ifndef __int32
#define __int32 int32_t
#endif
#ifndef __int16
#define __int16 int16_t
#endif
// NULL scope fix para lithbaselist.h
#include <cstddef>
