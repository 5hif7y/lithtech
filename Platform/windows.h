#pragma once
// Wrapper windows.h -> Platform/platform.h for Linux/gcc
#include "platform.h"
#ifndef _WIN32
// Extra Windows compat for Engine that expects full windows.h
#include <cstdint>
#include <cstdlib>
#include <sys/types.h>
// Missing types normally from windows.h
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID { uint32_t Data1; uint16_t Data2; uint16_t Data3; uint8_t Data4[8]; } GUID;
#endif
typedef GUID IID;
typedef GUID CLSID;
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
#endif
// Common Windows macros
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WIN32
#define WIN32
#endif
// Stub for CRITICAL_SECTION used in ltthread.h
typedef void* CRITICAL_SECTION;
typedef void* HANDLE;
inline void InitializeCriticalSection(CRITICAL_SECTION*) {}
inline void DeleteCriticalSection(CRITICAL_SECTION*) {}
inline void EnterCriticalSection(CRITICAL_SECTION*) {}
inline void LeaveCriticalSection(CRITICAL_SECTION*) {}
// Avoid MFC/ATL collisions
#ifndef _WINDOWS_
#define _WINDOWS_
#endif
#endif
