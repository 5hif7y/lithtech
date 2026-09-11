#pragma once
// Wrapper windows.h -> Platform/platform.h for Linux/gcc
#include "platform.h"
#ifndef _WIN32
#include <cstdint>
#include <cstdlib>
#include <sys/types.h>
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
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WIN32
#define WIN32
#endif
typedef void* CRITICAL_SECTION;
typedef void* HANDLE;
inline void InitializeCriticalSection(CRITICAL_SECTION*) {}
inline void DeleteCriticalSection(CRITICAL_SECTION*) {}
inline void EnterCriticalSection(CRITICAL_SECTION*) {}
inline void LeaveCriticalSection(CRITICAL_SECTION*) {}
#ifndef _WINDOWS_
#define _WINDOWS_
#endif
// MFC types
#ifndef POINT_DEFINED
#define POINT_DEFINED
typedef struct tagPOINT { LONG x; LONG y; } POINT, *PPOINT, *LPPOINT;
#endif
#ifndef RECT_DEFINED
#define RECT_DEFINED
typedef struct tagRECT { LONG left; LONG top; LONG right; LONG bottom; } RECT, *PRECT, *LPRECT;
#endif
#ifndef SIZE_DEFINED
#define SIZE_DEFINED
typedef struct tagSIZE { LONG cx; LONG cy; } SIZE;
#endif
#endif
