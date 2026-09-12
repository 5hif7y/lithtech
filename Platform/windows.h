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
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef void* LPSTR;
typedef const void* LPCSTR;
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
// Additional Win32 shims for demos (graphify 9 remaining)
#ifndef VK_F12
#define VK_F12 0x7B
#define VK_ESCAPE 0x1B
#define VK_RETURN 0x0D
#define VK_BACK 0x08
#define VK_SHIFT 0x10
#endif
inline bool GetComputerName(char* buf, unsigned long* len) { if(buf && len && *len>0){ const char* h="LinuxDemo"; size_t n=9; if(n>=*len) n=*len-1; for(size_t i=0;i<n;i++) buf[i]=h[i]; buf[n]=0; *len=n; return true; } return false; }
inline bool GetComputerNameW(wchar_t* buf, unsigned long* len) { (void)buf; (void)len; return false; }
// Portable keyboard/input shims (graphify 6 remaining, Windows+Unix)
#ifndef LPWORD
typedef WORD* LPWORD;
#endif
inline int GetKeyboardState(unsigned char* s) { if(s) for(int i=0;i<256;i++) s[i]=0; return 1; }
inline unsigned int ToAscii(unsigned int v, unsigned int s, const unsigned char* k, WORD* out, unsigned int f) { (void)v;(void)s;(void)k;(void)f; if(out) *out=0; return 0; }
// Full virtual-key set + resource macros (graphify GuiMgr/sealhunter)
#ifndef VK_LEFT
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_TAB 0x09
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_DELETE 0x2E
#define VK_HOME 0x24
#define VK_END 0x23
#endif
#ifndef MAKEINTRESOURCE
#define MAKEINTRESOURCE(i) ((char*)(uintptr_t)(i))
#endif
#ifndef UINT_DEFINED
#define UINT_DEFINED
typedef unsigned int UINT;
#endif
// Window/message types + resource stubs (graphify GuiMgr)
#ifndef HWND_DEFINED
#define HWND_DEFINED
typedef void* HWND;
typedef void* HINSTANCE;
typedef void* HMODULE;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HBRUSH;
typedef void* HMENU;
typedef long LRESULT;
typedef unsigned int WPARAM;
typedef long LPARAM;
typedef void* FARPROC;
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
#ifndef CALLBACK
#define CALLBACK
#endif
#endif
inline unsigned int LoadStringW(void* h, unsigned int id, wchar_t* buf, int n) { (void)h; (void)id; if(buf && n>0) buf[0]=0; return 0; }
inline unsigned int LoadStringA(void* h, unsigned int id, char* buf, int n) { (void)h; (void)id; if(buf && n>0) buf[0]=0; return 0; }
#ifndef LoadString
#define LoadString LoadStringA
#endif
// Message constants, cracker macro, window-management stubs (graphify GuiMgr)
#ifndef WM_MOUSEMOVE
#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_LBUTTONDBLCLK 0x0203
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_RBUTTONDBLCLK 0x0206
#define WM_CHAR 0x0102
#define WM_SETCURSOR 0x0020
#endif
#ifndef HANDLE_MSG
#define HANDLE_MSG(h, m, fn) case (m): return 0
#endif
#ifndef GWL_WNDPROC
#define GWL_WNDPROC (-4)
#define GWL_STYLE (-16)
#define WS_VISIBLE 0x10000000
#define SWP_FRAMECHANGED 0x0020
#define HWND_TOPMOST ((void*)-1)
#define HWND_NOTOPMOST ((void*)-2)
#endif
inline long GetWindowLong(void* h, int i) { (void)h; (void)i; return 0; }
inline long SetWindowLong(void* h, int i, long v) { (void)h; (void)i; (void)v; return 0; }
inline long CallWindowProc(void* p, void* h, unsigned int m, unsigned int w, long l) { (void)p; (void)h; (void)m; (void)w; (void)l; return 0; }
inline int SetWindowPos(void* h, void* a, int x, int y, int cx, int cy, unsigned int f) { (void)h; (void)a; (void)x; (void)y; (void)cx; (void)cy; (void)f; return 0; }
// Desktop/clip/show stubs (graphify GuiMgr)
#ifndef SW_NORMAL
#define SW_NORMAL 1
#define SW_HIDE 0
#define SW_SHOW 5
#endif
inline void* GetDesktopWindow() { return 0; }
inline int GetWindowRect(void* h, void* r) { if(r){ long* p=(long*)r; p[0]=0; p[1]=0; p[2]=800; p[3]=600; } (void)h; return 0; }
inline int ClipCursor(const void* r) { (void)r; return 0; }
inline int ShowWindow(void* h, int c) { (void)h; (void)c; return 0; }
