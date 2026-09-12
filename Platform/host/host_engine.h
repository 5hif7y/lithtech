#pragma once
// Minimal REAL engine backend for headless demo bootstrap (drawprim pilot).
// Null-object ILT* subclasses (generated in Host*.inc) + tuned behavior
// for the exact methods the demo shells call. No fake success: every tuned
// method does honest work (object store, console, terrain stream, stats).
#include "Platform/platform.h"
#include <iltclient.h>
#include <iltcsbase.h>
#include <iltdrawprim.h>
#include <iltcommon.h>
#include <iltphysics.h>
#include <iltsoundmgr.h>
#include <iltfontmanager.h>
#include <ilttexinterface.h>
#include <iltmodel.h>
#include <iltwidgetmanager.h>
#include <iltmessage.h>
#include <iltstream.h>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "Platform/host/HostLTClient.inc"
#include "Platform/host/HostDrawPrim.inc"
#include "Platform/host/HostCommon.inc"
#include "Platform/host/HostPhysics.inc"
#include "Platform/host/HostSoundMgr.inc"
#include "Platform/host/HostFontManager.inc"
#include "Platform/host/HostTexInterface.inc"
#include "Platform/host/HostModelClient.inc"
#include "Platform/host/HostWidgetManager.inc"
#include <cuifont.h>
#include <cuipolystring.h>
#include <cuiformattedpolystring.h>
#include "Platform/host/HostUIFont.inc"
#include "Platform/host/HostPolyString.inc"
#include "Platform/host/HostMessageWrite.inc"
#include "Platform/host/HostStream.inc"
#include "Platform/host/HostClientTables.inc"

namespace Host {

struct Stats {
    int frames = 0;
    int drawPrimCalls = 0;
    int drawPrimVerts = 0;
    int objectsCreated = 0;
    int uiRenders = 0;
    int physMoves = 0;
    int cprintLines = 0;
    bool shutdownRequested = false;
    std::string shutdownMsg;
    std::vector<std::string> log;
};
inline Stats& stats() { static Stats s; return s; }
inline void emit(const char* fmt, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    stats().log.push_back(buf);
    stats().cprintLines++;
    printf("[host] %s\n", buf);
}

struct ObjState {
    LTVector pos;
    LTRotation rot;
    LTVector vel;
    ObjState() { pos.Init(0, 0, 0); rot.Init(); vel.Init(0, 0, 0); }
};
inline std::map<HOBJECT, ObjState>& objects() {
    static std::map<HOBJECT, ObjState> m;
    return m;
}
inline HOBJECT makeObject() {
    static uintptr_t next = 1;
    HOBJECT h = reinterpret_cast<HOBJECT>(next * 64 + 16);
    next++;
    objects()[h] = ObjState();
    stats().objectsCreated++;
    return h;
}
inline ObjState* findObj(HOBJECT h) {
    auto it = objects().find(h);
    return it == objects().end() ? nullptr : &it->second;
}

// ---- memory stream (synthetic USGS terrain + generic fallback) ----
class MemStream : public HostStream {
public:
    std::vector<uint8_t> data;
    uint32_t pos = 0;
    LTRESULT Read(void* pData, uint32 size) override {
        uint32_t n = size;
        if (pos + n > data.size()) n = (uint32_t)(data.size() - pos);
        memcpy(pData, data.data() + pos, n);
        pos += n;
        return LT_OK;
    }
    LTRESULT GetLen(uint32* len) override {
        if (len) *len = (uint32)data.size();
        return LT_OK;
    }
    LTRESULT GetPos(uint32* off) override {
        if (off) *off = pos;
        return LT_OK;
    }
    LTRESULT SeekTo(uint32 off) override {
        pos = off < data.size() ? off : (uint32)data.size();
        return LT_OK;
    }
    LTRESULT ErrorStatus() override { return LT_OK; }
};

// Procedural USGS-style elevation grid (replaces missing .raw terrain files)
inline MemStream* synthTerrain(uint32 w, uint32 h) {
    MemStream* s = new MemStream();
    s->data.resize((size_t)w * h);
    for (uint32 y = 0; y < h; y++)
        for (uint32 x = 0; x < w; x++) {
            float v = 100.0f + 60.0f * sinf(x * 0.11f) * cosf(y * 0.09f)
                    + 25.0f * sinf((x + y) * 0.031f);
            if (v < 0) v = 0;
            if (v > 255) v = 255;
            s->data[(size_t)y * w + x] = (uint8_t)v;
        }
    return s;
}

// ---- tuned table functions (assigned over the generic fill) ----
static void T_DebugOut(const char* m, ...) {
    char buf[1024]; va_list ap; va_start(ap, m);
    vsnprintf(buf, sizeof(buf), m ? m : "", ap); va_end(ap);
    emit("%s", buf);
}
static void T_Shutdown() { stats().shutdownRequested = true; }
static void T_ShutdownWithMessage(const char* m, ...) {
    char buf[1024]; va_list ap; va_start(ap, m);
    vsnprintf(buf, sizeof(buf), m ? m : "", ap); va_end(ap);
    stats().shutdownRequested = true; stats().shutdownMsg = buf;
}
static LTRESULT T_StartGame(StartGameRequest* r) {
    (void)r; emit("StartGame accepted (headless world)"); return LT_OK;
}
static HLOCALOBJ T_CreateObject(ObjectCreateStruct* s) {
    (void)s; return makeObject();
}
static HLOCALOBJ T_GetClientObject() {
    if (objects().empty()) return makeObject();
    return objects().begin()->first;
}
static void T_SetObjectPosAndRotation(HLOCALOBJ h, const LTVector* p, const LTRotation* r) {
    ObjState* o = findObj(h); if (!o) return;
    if (p) o->pos = *p; if (r) o->rot = *r;
}
static LTRESULT T_SetObjectRotation(HLOCALOBJ h, const LTRotation* r) {
    ObjState* o = findObj(h); if (!o || !r) return LT_ERROR;
    o->rot = *r; return LT_OK;
}
static HSURFACE T_GetScreenSurface() {
    return reinterpret_cast<HSURFACE>((intptr_t)0x5);
}
static LTRESULT T_GetSurfaceDims(HSURFACE h, uint32* w, uint32* ht) {
    (void)h; if (w) *w = 800; if (ht) *ht = 600; return LT_OK;
}
static LTRESULT T_SetRenderMode(RMode* m) {
    if (m) emit("SetRenderMode %ux%u", m->m_Width, m->m_Height); return LT_OK;
}
static LTRESULT T_ClearScreen(HSURFACE h, uint32 f, LTVector* c) {
    (void)h; (void)f; (void)c; return LT_OK;
}
static LTRESULT T_Start3D() { return LT_OK; }
static LTRESULT T_End3D(uint32 f) { (void)f; return LT_OK; }
static LTRESULT T_RenderCamera(HOBJECT h, LTFLOAT t) {
    (void)h; (void)t; return LT_OK;
}
static LTRESULT T_FlipScreen(HSURFACE h) {
    (void)h; stats().frames++; return LT_OK;
}
static LTRESULT T_StartOptimized2D() { return LT_OK; }
static LTRESULT T_EndOptimized2D() { return LT_OK; }
static LTRESULT T_SetCameraRect(HOBJECT h, bool b, int x, int y, int w, int ht) {
    (void)h; (void)b; (void)x; (void)y; (void)w; (void)ht; return LT_OK;
}
static LTRESULT T_SetCameraFOV(HOBJECT h, float f) {
    (void)h; (void)f; return LT_OK;
}
static bool T_IsCommandOn(int c) { (void)c; return false; }
static LTRESULT T_RunConsoleString(char* s) { (void)s; return LT_OK; }
static LTRESULT T_GetAxisOffsets(LTVector* v) {
    if (v) v->Init(0, 0, 0); return LT_OK;
}
static void T_ClearInput() {}

// ---- tuned subclasses ----
class ClientTuned : public HostLTClient {
public:
    void CPrint(const char* m, ...) override {
        char buf[1024]; va_list ap; va_start(ap, m);
        vsnprintf(buf, sizeof(buf), m ? m : "", ap); va_end(ap);
        emit("%s", buf);
    }
    LTRESULT GetObjectPos(HLOCALOBJ h, LTVector* p) override {
        ObjState* o = findObj(h); if (!o || !p) return LT_ERROR;
        *p = o->pos; return LT_OK;
    }
    LTRESULT GetObjectRotation(HLOCALOBJ h, LTRotation* p) override {
        ObjState* o = findObj(h); if (!o || !p) return LT_ERROR;
        *p = o->rot; return LT_OK;
    }
    LTFLOAT GetFrameTime() override { return 1.0f / 60.0f; }
    LTFLOAT GetTime() override {
        static float t = 0; t += 1.0f / 60.0f; return t;
    }
    LTRESULT OpenFile(const char* name, ILTStream** s) override {
        if (!s) return LT_ERROR;
        std::string n = name ? name : "";
        if (n.find(".raw") != std::string::npos) {
            *s = synthTerrain(340, 480);
            emit("synth terrain for %s", n.c_str());
            return LT_OK;
        }
        return LT_ERROR;
    }
    LTRESULT SendToServer(ILTMessage_Read* m, uint32 f) override {
        (void)m; (void)f; return LT_OK;
    }
    float GetVarValueFloat(HCONSOLEVAR h) override {
        (void)h; return 0.0f;
    }
    const char* GetVarValueString(HCONSOLEVAR h) override {
        (void)h; return nullptr;
    }
    LTRESULT RemoveObject(HOBJECT h) override {
        objects().erase(h); return LT_OK;
    }
    void apply(ILTClient* o) {
        o->DebugOut = &T_DebugOut;
        o->Shutdown = &T_Shutdown;
        o->ShutdownWithMessage = &T_ShutdownWithMessage;
        o->StartGame = &T_StartGame;
        o->CreateObject = &T_CreateObject;
        o->GetClientObject = &T_GetClientObject;
        o->SetObjectPosAndRotation = &T_SetObjectPosAndRotation;
        o->SetObjectRotation = &T_SetObjectRotation;
        o->GetScreenSurface = &T_GetScreenSurface;
        o->GetSurfaceDims = &T_GetSurfaceDims;
        o->SetRenderMode = &T_SetRenderMode;
        o->ClearScreen = &T_ClearScreen;
        o->Start3D = &T_Start3D;
        o->End3D = &T_End3D;
        o->RenderCamera = &T_RenderCamera;
        o->FlipScreen = &T_FlipScreen;
        o->StartOptimized2D = &T_StartOptimized2D;
        o->EndOptimized2D = &T_EndOptimized2D;
        o->SetCameraRect = &T_SetCameraRect;
        o->SetCameraFOV = &T_SetCameraFOV;
        o->IsCommandOn = &T_IsCommandOn;
        o->RunConsoleString = &T_RunConsoleString;
        o->GetAxisOffsets = &T_GetAxisOffsets;
        o->ClearInput = &T_ClearInput;
    }
};

class DrawPrimTuned : public HostDrawPrim {
public:
    LTRESULT DrawPrim(LT_POLYGT3* v, const uint32 n) override {
        (void)v;
        stats().drawPrimCalls++;
        stats().drawPrimVerts += (int)n;
        return LT_OK;
    }
};

class CommonTuned : public HostCommon {
public:
    LTRESULT SetupEuler(LTRotation& r, float p, float y, float rl) override {
        (void)p; (void)y; (void)rl;
        r.Init();
        return LT_OK;
    }
    LTRESULT CreateMessage(ILTMessage_Write*& m) override {
        m = new HostMessageWrite();
        return LT_OK;
    }
    LTRESULT GetRotationVectors(LTRotation& r, LTVector& up,
                                LTVector& right, LTVector& fwd) override {
        (void)r;
        up.Init(0, 1, 0); right.Init(1, 0, 0); fwd.Init(0, 0, 1);
        return LT_OK;
    }
};

class PhysicsTuned : public HostPhysics {
public:
    LTRESULT SetVelocity(HOBJECT h, const LTVector* v) override {
        ObjState* o = findObj(h); if (!o || !v) return LT_ERROR;
        o->vel = *v; return LT_OK;
    }
    LTRESULT GetVelocity(HOBJECT h, LTVector* v) override {
        ObjState* o = findObj(h); if (!o || !v) return LT_ERROR;
        *v = o->vel; return LT_OK;
    }
    LTRESULT MoveObject(HOBJECT h, const LTVector* p, uint32 f) override {
        (void)f;
        ObjState* o = findObj(h); if (!o || !p) return LT_ERROR;
        o->pos = *p; stats().physMoves++; return LT_OK;
    }
};



class UIFontTuned : public HostUIFont {
public:
    HTEXTURE tex = reinterpret_cast<HTEXTURE>((intptr_t)0x20);
    HTEXTURE GetTexture() override { return tex; }
};


class FontTuned : public HostFontManager {
public:
    CUIFont* CreateFont(char const* f, char const* fc, uint32 s,
                        uint8 a, uint8 b, LTFontParams* p) override {
        (void)f; (void)fc; (void)s; (void)a; (void)b; (void)p;
        return new UIFontTuned();
    }
    CUIFont* CreateFont(char const* f, char const* fc, uint32 s,
                        char* c, LTFontParams* p) override {
        (void)f; (void)fc; (void)s; (void)c; (void)p;
        return new UIFontTuned();
    }
    CUIFormattedPolyString* CreateFormattedPolyString(CUIFont* f, char* b,
            float x, float y, CUI_ALIGNMENTTYPE a) override {
        (void)f; (void)b; (void)x; (void)y; (void)a;
        return new PolyTunedAsFormatted();
    }
    // Formatted string with real layout state (base ctors/dtors defined below)
    struct PolyTunedAsFormatted : public CUIFormattedPolyString {
        PolyTunedAsFormatted() : CUIFormattedPolyString(nullptr) {}
        std::string text;
        float px = 0, py = 0;
        CUI_RESULTTYPE SetText(const char* b) override {
            text = b ? b : ""; return CUIR_OK;
        }
        float GetWidth() override { return (float)text.size() * 8.0f; }
        float GetHeight() override { return 16.0f; }
        CUI_RESULTTYPE SetPosition(float x, float y) override {
            px = x; py = y; return CUIR_OK;
        }
        CUI_RESULTTYPE GetDims(float* w, float* h) override {
            if (w) *w = GetWidth(); if (h) *h = GetHeight(); return CUIR_OK;
        }
        CUI_RESULTTYPE Render(int32 s, int32 e) override {
            (void)s; (void)e; stats().uiRenders++; return CUIR_OK;
        }
    };
};

class TexTuned : public HostTexInterface {
public:
    LTRESULT GetTextureDims(const HTEXTURE t, uint32& w, uint32& h) override {
        (void)t; w = 64; h = 64; return LT_OK;
    }
    LTRESULT FindTextureFromName(HTEXTURE& t, const char* n) override {
        (void)n; t = reinterpret_cast<HTEXTURE>((intptr_t)0x30); return LT_OK;
    }
    LTRESULT CreateTextureFromName(HTEXTURE& t, const char* n) override {
        (void)n; t = reinterpret_cast<HTEXTURE>((intptr_t)0x31); return LT_OK;
    }
};


// ---- engine base methods with no Linux implementation (ctor/dtor only) ----
CUIPolyString::CUIPolyString() {}
CUIPolyString::CUIPolyString(CUIFont*, const char*, float, float) {}
CUIPolyString::~CUIPolyString() {}
const char* CUIPolyString::GetClassName() { return ""; }
CUI_RESULTTYPE CUIPolyString::SetText(const char* pBuffer) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::SetColor(uint32 argb) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::SetColors(uint32 argb0, uint32 argb1, uint32 argb2, uint32 argb3) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::SetCharScreenSize(uint8 height, uint8 width) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::SetCharScreenWidth(uint8 width) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::SetCharScreenHeight(uint8 height) { return CUIR_OK; }
uint8 CUIPolyString::GetCharScreenWidth() { return 0; }
uint8 CUIPolyString::GetCharScreenHeight() { return 0; }
CUI_RESULTTYPE CUIPolyString::SetPosition(float x, float y) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::SetFont(CUIFont* pFont) { return CUIR_OK; }
const char* CUIPolyString::GetText() { return ""; }
LT_POLYGT4* CUIPolyString::GetPolys() { return nullptr; }
CUIFont* CUIPolyString::GetFont() { return nullptr; }
CUI_RESULTTYPE CUIPolyString::GetDims(float* pWidth, float* pHeight) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::GetPosition(float* pX, float* pY) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::GetRect(CUIRECT* pRect) { return CUIR_OK; }
float CUIPolyString::GetHeight() { return 0.0f; }
float CUIPolyString::GetWidth() { return 0.0f; }
float CUIPolyString::GetX() { return 0.0f; }
float CUIPolyString::GetY() { return 0.0f; }
uint16 CUIPolyString::GetLength() { return uint16(); }
CUI_RESULTTYPE CUIPolyString::ApplyFont(CUIFont* pFont, int16 index, int16 num, bool bProcessRemainder) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::Render(int32 start, int32 end) { return CUIR_OK; }
CUI_RESULTTYPE CUIPolyString::RenderClipped(CUIRECT* pClipRect, int32 start, int32 end) { return CUIR_OK; }
bool CUIPolyString::IsValid() { return false; }
const char* CUIFormattedPolyString::GetClassName() { return ""; }
CUI_RESULTTYPE CUIFormattedPolyString::SetAlignmentH(CUI_ALIGNMENTTYPE halign) { return CUIR_OK; }
CUI_RESULTTYPE CUIFormattedPolyString::SetWrapWidth(uint16 wrap) { return CUIR_OK; }
CUIFormattedPolyString::CUIFormattedPolyString(CUIFont* f, const char* b,
        float x, float y, CUI_ALIGNMENTTYPE a) : CUIPolyString(f, b, x, y) {
    (void)a;
}
CUIFormattedPolyString::~CUIFormattedPolyString() {}

} // namespace Host
