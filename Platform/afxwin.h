#pragma once
// Shim MFC afxwin.h -> std + Platform/windows.h
#include "Platform/windows.h"
#include <string>
#include <vector>
#define DECLARE_DYNCREATE(x,y)
#define IMPLEMENT_DYNCREATE(x,y)
#define BEGIN_MESSAGE_MAP(x,y)
#define END_MESSAGE_MAP()
#define ON_COMMAND(a,b)
typedef void* HINSTANCE;
class CObject {};
class CWnd : public CObject {};
class CWinApp : public CWnd {};
