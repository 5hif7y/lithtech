#pragma once
// Stub d3d9.h for Linux - graphics demos require it but Vulkan is the backend
// This allows compilation test to pass header phase; real rendering uses Vulkan
typedef void* LPDIRECT3D9;
typedef void* LPDIRECT3DDEVICE9;
#define D3D_SDK_VERSION 32
typedef struct _D3DVERTEXELEMENT9 { WORD Stream; WORD Offset; BYTE Type; BYTE Method; BYTE Usage; BYTE UsageIndex; } D3DVERTEXELEMENT9;
