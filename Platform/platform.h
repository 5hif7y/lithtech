#pragma once
// Shim Windows -> Linux para LithTech Jupiter + gcc/g++
#ifdef _WIN32
  #include <windows.h>
#else
  #include <cstdint>
  #include <cstring>
  #include <cstddef>
  #include <unistd.h>
  #include <strings.h> // strcasecmp

  // Tipos Windows
  using BYTE = uint8_t;
  using WORD = uint16_t;
  using DWORD = uint32_t;
  using LONG = int32_t;
  using ULONG = uint32_t;
  using HANDLE = void*;
  using HINSTANCE = void*;
  using HWND = void*;
  using HRESULT = int32_t;
  using BOOL = int;
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

  // Otras
  #include <cctype>
  #define _vsnprintf vsnprintf
  #define _snprintf snprintf

#endif
