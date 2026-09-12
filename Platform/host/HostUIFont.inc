#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <cuifont.h>
class HostUIFont : public CUIFont {
public:
const char* GetClassName() override { return ""; }
bool IsValid() override { return false; }
bool IsProportional() override { return false; }
HTEXTURE GetTexture() override { return HTEXTURE(); }
uint32 GetAttributes() override { return uint32(); }
uint8 GetDefCharScreenWidth() override { return 0; }
uint8 GetDefCharScreenHeight() override { return 0; }
uint8 GetCharTexWidth() override { return 0; }
uint8 GetCharTexHeight() override { return 0; }
uint8 GetDefSpacingH() override { return 0; }
uint8 GetDefSpacingV() override { return 0; }
int8 GetDefSlant() override { return 0; }
int8 GetDefBold() override { return 0; }
uint16* GetFontTable() override { return nullptr; }
uint8* GetFontMap() override { return nullptr; }
uint32* GetDefColors() override { return nullptr; }
CUI_RESULTTYPE SetTexture(HTEXTURE hTex, uint8 cwidth, uint8 cheight) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE SetTexture(HTEXTURE hTex, uint16* pTable, uint8 cheight) override { return CUI_RESULTTYPE(); }
void SetMap(uint8* pMap) override {}
void SetAttributes(uint32 attrs) override {}
void UnsetAttributes(uint32 attrs) override {}
void SetDefCharWidth(uint8 width) override {}
void SetDefCharHeight(uint8 height) override {}
void SetDefSpacingH(uint8 hspacing) override {}
void SetDefSpacingV(uint8 vspacing) override {}
void SetDefSlant(int8 slant) override {}
void SetDefBold(int8 bold) override {}
void SetDefColor(uint32 argb) override {}
void SetDefColors(uint32 argb0, uint32 argb1, uint32 argb2, uint32 argb3) override {}
void DrawString(float x, float y, char* text) override {}
void Apply(CUIPolyString* pPolyStr, int16 index, int16 num) override {}
};
