#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltfontmanager.h>
class HostFontManager : public ILTFontManager {
public:
void Init() override {}
void Term() override {}
bool IsInitted() override { return false; }
CUIFont* CreateFont(HTEXTURE hTex, uint8 charw, uint8 charh, uint8* pMap) override { return nullptr; }
CUIFont* CreateFont(HTEXTURE hTex, uint16* pTable, uint8 charh, uint8* pMap) override { return nullptr; }
CUIFont* CreateFont(HTEXTURE hTex, uint8* pMap) override { return nullptr; }
CUIFont* CreateFont(char const* pszFontFile, char const* pszFontFace, uint32 pointSize, uint8 asciiStart, uint8 asciiEnd, LTFontParams* fontParams) override { return nullptr; }
CUIFont* CreateFont(char const* pszFontFile, char const* pszFontFace, uint32 pointSize, char* pCharacters, LTFontParams* fontParams) override { return nullptr; }
void DestroyFont(CUIFont* pFont) override {}
uint32 GetNumFontsInUse() override { return uint32(); }
HTEXTURE CharToTexture(uint8* pTTFData, uint32 dataSize, uint32 character, uint32 pointSize) override { return HTEXTURE(); }
HTEXTURE StringToTexture(uint8* pTTFData, uint32 dataSize, char* pCharacters, uint32 pointSize) override { return HTEXTURE(); }
HTEXTURE StringToTexture(uint8* pTTFData, uint32 dataSize, uint32* pCharacters, uint32 pointSize) override { return HTEXTURE(); }
CUIPolyString* CreatePolyString(CUIFont* pFont, char* pBuffer, float x, float y) override { return nullptr; }
CUIFormattedPolyString* CreateFormattedPolyString(CUIFont* pFont, char* pBuffer, float x, float y, CUI_ALIGNMENTTYPE alignment) override { return nullptr; }
void DestroyPolyString(CUIPolyString* pPolystr) override {}
const char* _InterfaceImplementation() override { return ""; }
};
