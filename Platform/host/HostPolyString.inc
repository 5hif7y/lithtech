#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <cuipolystring.h>
#include <cuiformattedpolystring.h>
class HostPolyString : public CUIPolyString {
public:
const char* GetClassName() override { return ""; }
CUI_RESULTTYPE SetText(const char* pBuffer) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE SetColor(uint32 argb) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE SetColors(uint32 argb0, uint32 argb1, uint32 argb2, uint32 argb3) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE SetCharScreenSize(uint8 height, uint8 width) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE SetCharScreenWidth(uint8 width) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE SetCharScreenHeight(uint8 height) override { return CUI_RESULTTYPE(); }
uint8 GetCharScreenWidth() override { return 0; }
uint8 GetCharScreenHeight() override { return 0; }
CUI_RESULTTYPE SetPosition(float x, float y) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE SetFont(CUIFont* pFont) override { return CUI_RESULTTYPE(); }
const char* GetText() override { return ""; }
LT_POLYGT4* GetPolys() override { return nullptr; }
CUIFont* GetFont() override { return nullptr; }
CUI_RESULTTYPE GetDims(float* pWidth, float* pHeight) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE GetPosition(float* pX, float* pY) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE GetRect(CUIRECT* pRect) override { return CUI_RESULTTYPE(); }
float GetHeight() override { return 0.0f; }
float GetWidth() override { return 0.0f; }
float GetX() override { return 0.0f; }
float GetY() override { return 0.0f; }
uint16 GetLength() override { return uint16(); }
CUI_RESULTTYPE ApplyFont(CUIFont* pFont, int16 index, int16 num, bool bProcessRemainder) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE Render(int32 start, int32 end) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE RenderClipped(CUIRECT* pClipRect, int32 start, int32 end) override { return CUI_RESULTTYPE(); }
bool IsValid() override { return false; }
};
