#pragma once
// Shim Windows -> Linux para LithTech Jupiter + gcc/g++
#ifdef _WIN32
  #include <windows.h>
#else
  // Alias _LINUX <-> __LINUX para compat VC6 (stdlithdefs.h usa __LINUX, CMake usa _LINUX)
  #if defined(_LINUX) && !defined(__LINUX)
  #define __LINUX 1
  #endif
  #if defined(__LINUX) && !defined(_LINUX)
  #define _LINUX 1
  #endif
  #include <cstdint>
  #include <cstring>
  #include <cstddef>
  #include <unistd.h>
  #include <strings.h> // strcasecmp

  // Tipos Windows — guarded con #ifndef para compat con lithtypes.h / rezmgr.h (#ifndef DWORD)
  #ifndef BYTE
  using BYTE = uint8_t;
  #define BYTE BYTE
  #endif
  #ifndef WORD
  using WORD = uint16_t;
  #define WORD WORD
  #endif
  #ifndef DWORD
  using DWORD = uint32_t;
  #define DWORD DWORD
  #endif
  #ifndef LONG
  using LONG = int32_t;
  #define LONG LONG
  #endif
  #ifndef ULONG
  using ULONG = uint32_t;
  #define ULONG ULONG
  #endif
  #ifndef HANDLE
  using HANDLE = void*;
  #define HANDLE HANDLE
  #endif
  #ifndef HINSTANCE
  using HINSTANCE = void*;
  #define HINSTANCE HINSTANCE
  #endif
  #ifndef HWND
  using HWND = void*;
  #define HWND HWND
  #endif
  #ifndef HRESULT
  using HRESULT = int32_t;
  #define HRESULT HRESULT
  #endif
  #ifndef BOOL
  using BOOL = int;
  #define BOOL BOOL
  #endif
  #ifndef TRUE
  #define TRUE 1
  #define FALSE 0
  #endif
  #ifndef MAX_PATH
  #define MAX_PATH 260
  #endif
  #define S_OK 0
  #define S_FALSE 1
  #define E_FAIL 0x80004005
  #define E_INVALIDARG 0x80070057
  #define FAILED(hr) ((hr) < 0)
  #define SUCCEEDED(hr) ((hr) >= 0)

  // Calling conventions vacias
  #ifndef __stdcall
  #define __stdcall
  #endif
  #ifndef __cdecl
  #define __cdecl
  #endif
  #ifndef __declspec
  #define __declspec(x)
  #endif
  #ifndef STDMETHODCALLTYPE
  #define STDMETHODCALLTYPE
  #endif
  #ifndef WINAPI
  #define WINAPI
  #endif
  #ifndef CALLBACK
  #define CALLBACK
  #endif

  // Funciones string MSVC -> POSIX
  #ifndef stricmp
  #define stricmp strcasecmp
  #endif
  #ifndef strnicmp
  #define strnicmp strncasecmp
  #endif
  #ifndef _stricmp
  #define _stricmp strcasecmp
  #endif
  #ifndef _strnicmp
  #define _strnicmp strncasecmp
  #endif

  // strupr/strlwr — en __LINUX los provee ltbasedefs.h; aqui solo si no esta __LINUX
  #if !defined(__LINUX)
  #ifndef strupr
  #include <cctype>
  inline char* strupr(char* s){ if(!s) return s; for(char* p=s;*p;++p) *p=toupper((unsigned char)*p); return s; }
  #endif
  #ifndef strlwr
  inline char* strlwr(char* s){ if(!s) return s; for(char* p=s;*p;++p) *p=tolower((unsigned char)*p); return s; }
  #endif
  #endif

  // Otras — math, wchar, vsnprintf, MAX_PATH aliases
  #include <cctype>
  #include <cstdio>
  #include <cwchar>
  #include <wchar.h>
  #include <cmath>
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
  #define _vsnprintf vsnprintf
  #define _snprintf snprintf
  #ifndef _vsnwprintf
  #define _vsnwprintf vswprintf
  #endif
  // Lith math shims — asegurar que ltsinf/ltcosf/ltsqrtf existan sin necesidad de -include math_shim.h
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

#endif
