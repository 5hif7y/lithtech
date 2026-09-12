#pragma once
#include "Platform/platform.h"
#include "Platform/windows.h"
typedef void* LPDIRECT3D9;
typedef void* LPDIRECT3DDEVICE9;
typedef void* LPDIRECT3DTEXTURE9;
#define D3D_SDK_VERSION 32
typedef struct _D3DVERTEXELEMENT9 { WORD Stream; WORD Offset; BYTE Type; BYTE Method; BYTE Usage; BYTE UsageIndex; } D3DVERTEXELEMENT9;
