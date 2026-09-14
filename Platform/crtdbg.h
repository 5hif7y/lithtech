#pragma once
// Shim crtdbg.h for Linux - MFCStub expects it but not needed on gcc
#include <cassert>
#include <cstdlib>
#ifndef _ASSERTE
#define _ASSERTE(x) assert(x)
#endif
#ifndef _ASSERT
#define _ASSERT(x) assert(x)
#endif
#define _CrtDbgReport(...)
#define _CrtSetDbgFlag(x) (0)
#define _CrtSetBreakAlloc(x) (0)
