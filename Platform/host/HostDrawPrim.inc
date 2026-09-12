#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltdrawprim.h>
class HostDrawPrim : public ILTDrawPrim {
public:
LTRESULT BeginDrawPrim() override { return LT_OK; }
LTRESULT EndDrawPrim() override { return LT_OK; }
LTRESULT SetCamera(const HOBJECT hCamera) override { return LT_OK; }
LTRESULT SetTexture(const HTEXTURE hTexture) override { return LT_OK; }
LTRESULT SetTransformType(const ELTTransformType eType) override { return LT_OK; }
LTRESULT SetColorOp(const ELTColorOp eColorOp) override { return LT_OK; }
LTRESULT SetAlphaBlendMode(const ELTBlendMode eBlendMode) override { return LT_OK; }
LTRESULT SetZBufferMode(const ELTZBufferMode eZBufferMode) override { return LT_OK; }
LTRESULT SetAlphaTestMode(const ELTTestMode eTestMode) override { return LT_OK; }
LTRESULT SetClipMode(const ELTClipMode eClipMode) override { return LT_OK; }
LTRESULT SetFillMode(ELTDPFillMode efillmode) override { return LT_OK; }
LTRESULT SetCullMode(ELTDPCullMode ecullmode) override { return LT_OK; }
LTRESULT SetFogEnable(bool bFogEnable) override { return LT_OK; }
LTRESULT SetReallyClose(bool bReallyClose) override { return LT_OK; }
LTRESULT SetEffectShaderID(uint32 nEffectShaderID) override { return LT_OK; }
void SaveViewport() override {}
void RestoreViewport() override {}
LTRESULT DrawPrim(LT_POLYGT3 *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_POLYFT3 *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_POLYG3 *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_POLYF3 *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_POLYGT4 *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_POLYGT4 **ppPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_POLYFT4 *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_POLYG4 *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_POLYF4 *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_LINEGT *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_LINEFT *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_LINEG *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrim(LT_LINEF *pPrim, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrimPoint(LT_VERTGT *pVerts, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrimPoint(LT_VERTG *pVerts, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrimFan(LT_VERTGT *pVerts, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrimFan(LT_VERTFT *pVerts, const uint32 nCount, LT_VERTRGBA rgba) override { return LT_OK; }
LTRESULT DrawPrimFan(LT_VERTG *pVerts, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrimFan(LT_VERTF *pVerts, const uint32 nCount, LT_VERTRGBA rgba) override { return LT_OK; }
LTRESULT DrawPrimStrip(LT_VERTGT *pVerts, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrimStrip(LT_VERTFT *pVerts, const uint32 nCount, LT_VERTRGBA rgba) override { return LT_OK; }
LTRESULT DrawPrimStrip(LT_VERTG *pVerts, const uint32 nCount) override { return LT_OK; }
LTRESULT DrawPrimStrip(LT_VERTF *pVerts, const uint32 nCount, LT_VERTRGBA rgba) override { return LT_OK; }
void SetXY4(LT_POLYGT4 *pPrim, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) override {}
void SetXY4(LT_POLYFT4 *pPrim, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) override {}
void SetXY4(LT_POLYG4 *pPrim, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) override {}
void SetXY4(LT_POLYF4 *pPrim, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) override {}
void SetXYWH(LT_POLYGT4 *pPrim, float x, float y, float w, float h) override {}
void SetXYWH(LT_POLYFT4 *pPrim, float x, float y, float w, float h) override {}
void SetXYWH(LT_POLYG4 *pPrim, float x, float y, float w, float h) override {}
void SetXYWH(LT_POLYF4 *pPrim, float x, float y, float w, float h) override {}
void SetUV4(LT_POLYGT4 *pPrim, float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3) override {}
void SetUV4(LT_POLYFT4 *pPrim, float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3) override {}
void SetUVWH(LT_POLYGT4 *pPrim, float u, float v, float w, float h) override {}
void SetUVWH(LT_POLYGT4 *pPrim, HTEXTURE pTex, float u, float v, float w, float h) override {}
void SetUVWH(LT_POLYFT4 *pPrim, float u, float v, float w, float h) override {}
void SetRGB(LT_POLYGT4 *pPrim, uint32 color) override {}
void SetRGB(LT_POLYFT4 *pPrim, uint32 color) override {}
void SetRGB(LT_POLYG4 *pPrim, uint32 color) override {}
void SetRGB(LT_POLYF4 *pPrim, uint32 color) override {}
void SetRGBA(LT_POLYGT4 *pPrim, uint32 color) override {}
void SetRGBA(LT_POLYFT4 *pPrim, uint32 color) override {}
void SetRGBA(LT_POLYG4 *pPrim, uint32 color) override {}
void SetRGBA(LT_POLYF4 *pPrim, uint32 color) override {}
void SetRGB4(LT_POLYGT4 *pPrim, uint32 color0, uint32 color1, uint32 color2, uint32 color3) override {}
void SetRGB4(LT_POLYG4 *pPrim, uint32 color0, uint32 color1, uint32 color2, uint32 color3) override {}
void SetRGBA4(LT_POLYGT4 *pPrim, uint32 color0, uint32 color1, uint32 color2, uint32 color3) override {}
void SetRGBA4(LT_POLYG4 *pPrim, uint32 color0, uint32 color1, uint32 color2, uint32 color3) override {}
void SetALPHA(LT_POLYGT4 *pPrim, uint8 alpha) override {}
void SetALPHA(LT_POLYG4 *pPrim, uint8 alpha) override {}
void SetALPHA(LT_POLYFT4 *pPrim, uint8 alpha) override {}
void SetALPHA(LT_POLYF4 *pPrim, uint8 alpha) override {}
void SetALPHA4(LT_POLYGT4 *pPrim, uint8 alpha0, uint8 alpha1, uint8 alpha2, uint8 alpha3) override {}
void SetALPHA4(LT_POLYG4 *pPrim, uint8 alpha0, uint8 alpha1, uint8 alpha2, uint8 alpha3) override {}
const char* _InterfaceImplementation() override { return ""; }
};
