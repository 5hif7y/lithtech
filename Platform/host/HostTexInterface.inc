#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <ilttexinterface.h>
class HostTexInterface : public ILTTexInterface {
public:
LTRESULT FindTextureFromName(HTEXTURE &hTexture, const char *pFilename) override { return LT_OK; }
LTRESULT CreateTextureFromName(HTEXTURE &hTexture, const char *pFilename) override { return LT_OK; }
LTRESULT CreateTextureFromData(HTEXTURE &hTexture, ETextureType eTextureType, uint32 TextureFlags, uint8 *pData, uint32 nWidth, uint32 nHeight, uint32 nAutoGenMipMaps) override { return LT_OK; }
LTRESULT GetTextureData(const HTEXTURE hTexture, const uint8* &pData, uint32& nWidth, uint32& nHeight, uint32& nPitch, ETextureType& eType) override { return LT_OK; }
LTRESULT FlushTextureData(const HTEXTURE hTexture, ETextureMod eChanged, uint32 nMipMap) override { return LT_OK; }
LTRESULT GetTextureDims(const HTEXTURE hTexture, uint32 &nWidth, uint32 &nHeight) override { return LT_OK; }
LTRESULT GetTextureType(const HTEXTURE hTexture, ETextureType &eTextureType) override { return LT_OK; }
bool ReleaseTextureHandle(const HTEXTURE hTexture) override { return false; }
uint32 AddRefTextureHandle(const HTEXTURE hTexture) override { return uint32(); }
const char* _InterfaceImplementation() override { return ""; }
};
