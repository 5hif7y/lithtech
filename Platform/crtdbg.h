#pragma once
// Dispatcher: Platform/ shadows the SDK on the include path, so a plain
// <crtdbg.h> always lands here. On real Windows forward to the UCRT header
// by absolute path (LITH_REAL_CRTDBG_H, set by CMake); elsewhere use the
// Linux shim (MFCStub expects the header but gcc doesn't need it).
#ifdef _WIN32
#ifndef LITH_REAL_CRTDBG_H
#error "Platform/crtdbg.h: LITH_REAL_CRTDBG_H not defined (configure via CMake)"
#endif
#include LITH_REAL_CRTDBG_H
#else
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
#endif
