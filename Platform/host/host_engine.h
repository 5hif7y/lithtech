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
#include <ltobjectcreate.h>
#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#ifndef _WIN32
#include <unistd.h> // readlink(/proc/self/exe) for exeDir()
#endif
#ifndef _WIN32
#include <strings.h>
#include <glob.h>
#endif
#include <cctype>
#include <set>
#ifdef _LINUX
#include <X11/keysym.h>
#endif
// NOTE: system headers must stay at global scope in this file. Also, do
// NOT scan directories with opendir/readdir here: GCC 16 -O2 miscompiles
// that loop into a call-once halt (treats readdir as pure, drops the body
// and the back-edge; proven via core + GIMPLE dump). std::filesystem is
// precompiled and opaque to that analysis, so it is used instead.
#ifdef HAS_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif
#include <map>
#include <string>
#include <vector>

#include "Platform/host/HostLTClient.inc"
#include "vk_renderer.h"
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

// Hooks implemented by host_server.h (server build only). When null, the
// client keeps its legacy behavior (dropped messages, HostMessageWrite).
inline void (*g_routeToServer)(ILTMessage_Read* m) = nullptr;
inline ILTMessage_Write* (*g_createClientMessage)() = nullptr;

struct Stats {
    int frames = 0;
    int drawPrimCalls = 0;
    int drawPrimVerts = 0;
    int objectsCreated = 0;
    int uiRenders = 0;
    int physMoves = 0;
    int terrainBytes = 0;
    unsigned long long terrainSum = 0;
    int cprintLines = 0;
    bool shutdownRequested = false;
    bool worldStartRequested = false;
    std::string shutdownMsg;
    std::vector<std::string> log;
};
inline Stats& stats() { static Stats s; return s; }

// ---- input state: keys/commands held + accumulated mouse axes ----
// Windows VK codes (own enum to avoid clashing with <windows.h>).
enum HostVK {
    HVK_LBUTTON = 0x01, HVK_TAB = 0x09, HVK_RETURN = 0x0D, HVK_ESCAPE = 0x1B,
    HVK_SPACE = 0x20, HVK_LEFT = 0x25, HVK_UP = 0x26, HVK_RIGHT = 0x27,
    HVK_DOWN = 0x28, HVK_F12 = 0x7B
};
struct InputState {
    std::set<int> cmds; // engine command ids currently held
    std::set<int> keys; // VK codes currently held
    float ax[3] = {0, 0, 0};
};
inline InputState& inputState() { static InputState s; return s; }
// Returns true on the FIRST press of a command (edge for OnCommandOn).
inline bool noteKey(int vk, int cmd, bool down) {
    InputState& s = inputState();
    if (down) {
        s.keys.insert(vk);
        if (cmd < 0) return false;
        if (s.cmds.count(cmd)) return false;
        s.cmds.insert(cmd);
        return true;
    }
    s.keys.erase(vk);
    if (cmd >= 0) s.cmds.erase(cmd);
    return false;
}
inline bool cmdOn(int cmd) { return inputState().cmds.count(cmd) != 0; }
inline void addAxes(float x, float y, float z) {
    float* a = inputState().ax;
    a[0] += x; a[1] += y; a[2] += z;
}
inline void takeAxes(float out[3]) {
    float* a = inputState().ax;
    out[0] = a[0]; out[1] = a[1]; out[2] = a[2];
    a[0] = a[1] = a[2] = 0;
}
// ---- publicacion cliente->host: transform del jugador para dibujar su modelo.
// CPlayerClnt::Update lo publica cada frame (R3-live); el host lo consume.
struct PlayerPub {
    float x = 0, y = 0, z = 0, yaw = 0, pitch = 0;
    bool valid = false;
};
inline PlayerPub& playerPub() {
    static PlayerPub s;
    return s;
}
// Definida una sola vez en host_sealhunter.cpp (este header es single-TU;
// los .cpp del juego solo la declaran para publicar el transform).
void notePlayer(float x, float y, float z, float yaw, float pitch);
inline bool& modelsLiveFlag() {
    static bool b = false;
    return b;
}
#ifdef _LINUX
// X11 KeySym -> (Windows VK, engine command). Commands mirror the demo's
// autoexec.cfg bindings (WASD/arrows move, Enter Start, Space Jump,
// T Chat, Tab ShowStats, Esc Quit, mouse Button0 Shoot).
inline void mapKeysym(unsigned long ks, int& vk, int& cmd) {
    vk = -1; cmd = -1;
    switch (ks) {
    case XK_Up: vk = HVK_UP; cmd = 1; break;
    case XK_Down: vk = HVK_DOWN; cmd = 2; break;
    case XK_Left: vk = HVK_LEFT; cmd = 3; break;
    case XK_Right: vk = HVK_RIGHT; cmd = 4; break;
    case XK_Return: case XK_KP_Enter: vk = HVK_RETURN; cmd = 18; break;
    case XK_space: vk = HVK_SPACE; cmd = 16; break;
    case XK_Tab: vk = HVK_TAB; cmd = 17; break;
    case XK_Escape: vk = HVK_ESCAPE; cmd = 250; break;
    case XK_F12: vk = HVK_F12; cmd = -1; break;
    default:
        if ((ks >= 'a' && ks <= 'z') || (ks >= 'A' && ks <= 'Z')) {
            int u = toupper((int)ks);
            vk = u;
            if (u == 'W') cmd = 1;
            else if (u == 'S') cmd = 2;
            else if (u == 'A') cmd = 3;
            else if (u == 'D') cmd = 4;
            else if (u == 'T') cmd = 19;
        } else if (ks >= '0' && ks <= '9') {
            vk = (int)ks;
        }
        break;
    }
}
#endif
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

// Windows run.bat style: lithtech -rez engine.rez -rez ..\rez
// Accepts repeatable "-rez <path>" (also -rez=<p> / --rez=<p>).
// Engine .REZ archives are NOT mounted (no archive parser yet); the last
// -rez value that is an existing directory becomes the asset root.
// Returns "" when no usable directory was given.
inline std::string ParseRezArgs(int argc, char* argv[]) {
    std::string dir;
    std::error_code ec;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i] ? argv[i] : "";
        std::string v;
        if (a == "-rez" && i + 1 < argc) v = argv[++i];
        else if (a.rfind("-rez=", 0) == 0) v = a.substr(5);
        else if (a.rfind("--rez=", 0) == 0) v = a.substr(6);
        else continue;
        for (char& c : v) if (c == '\\') c = '/';
        ec.clear();
        if (!v.empty() && std::filesystem::is_directory(v, ec)) dir = v;
        else emit("ignoring -rez '%s' (not a directory; .REZ archives are not mounted)", v.c_str());
    }
    return dir;
}

struct ObjState {
    LTVector pos;
    LTRotation rot;
    LTVector vel;
    uint32 type = 0; // m_ObjectType at creation (OT_CAMERA=5 finds the camera)
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
    (void)r; emit("StartGame accepted (headless world)");
    stats().worldStartRequested = true;
    return LT_OK;
}
static LTRESULT T_InitNetworking(const char* pDriver, uint32 dwFlags) {
    (void)pDriver; (void)dwFlags;
    return LT_OK;
}
static LTRESULT T_AddInternetDriver() { return LT_OK; }
static LTRESULT T_GetSessionList(NetSession*& pListHead, const char* pInfo) {
    (void)pInfo;
    pListHead = nullptr;
    return LT_OK;
}
static LTRESULT T_GetServiceList(NetService*& pListHead) {
    pListHead = nullptr;
    return LT_ERROR;
}
static LTRESULT T_FreeServiceList(NetService* pListHead) {
    (void)pListHead;
    return LT_OK;
}
static LTRESULT T_SelectService(HNETSERVICE hNetService) {
    (void)hNetService;
    return LT_OK;
}
// Consumes a pending world start (set by T_StartGame). The main loop calls
// shell->OnEnterWorld() when this returns true, so menu-initiated games
// (StartNormalGame/Host/Join from the GUI) enter the world like the
// auto-start path does. Returns false when already consumed.
inline bool consumeWorldStart() {
    bool r = stats().worldStartRequested;
    stats().worldStartRequested = false;
    return r;
}
static HLOCALOBJ T_CreateObject(ObjectCreateStruct* s) {
    HLOCALOBJ h = makeObject();
    if (s) {
        ObjState* o = findObj(h);
        if (o) {
            o->type = s->m_ObjectType;
            o->pos = s->m_Pos;
            o->rot = s->m_Rotation;
        }
    }
    return h;
}
static HLOCALOBJ T_GetClientObject() {
    if (objects().empty()) return makeObject();
    return objects().begin()->first;
}
static void T_SetObjectPosAndRotation(HLOCALOBJ h, const LTVector* p, const LTRotation* r) {
    ObjState* o = findObj(h); if (!o) return;
    if (p) o->pos = *p; if (r) o->rot = *r;
}
static void T_SetObjectRotation(HLOCALOBJ h, const LTRotation* r) {
    ObjState* o = findObj(h); if (!o || !r) return;
    o->rot = *r;
}
static HSURFACE T_GetScreenSurface() {
    return reinterpret_cast<HSURFACE>((intptr_t)0x5);
}
static void T_GetSurfaceDims(HSURFACE h, uint32* w, uint32* ht) {
    (void)h; if (w) *w = 800; if (ht) *ht = 600;
}
static LTRESULT T_SetRenderMode(RMode* m) {
    if (m) emit("SetRenderMode %ux%u", m->m_Width, m->m_Height); return LT_OK;
}
static LTRESULT T_ClearScreen(LTRect* r, uint32 f, LTRGB* c) {
    (void)r; (void)f; (void)c; return LT_OK;
}
static LTRESULT T_Start3D() { return LT_OK; }
static LTRESULT T_End3D(uint32 f) { (void)f; return LT_OK; }
static LTRESULT T_RenderCamera(HOBJECT h, LTFLOAT t) {
    (void)h; (void)t; return LT_OK;
}
static LTRESULT T_FlipScreen(uint32 f) {
    (void)f; stats().frames++; return LT_OK;
}
static LTRESULT T_StartOptimized2D() { return LT_OK; }
static LTRESULT T_EndOptimized2D() { return LT_OK; }
static void T_SetCameraRect(HLOCALOBJ h, bool b, int x, int y, int w, int ht) {
    (void)h; (void)b; (void)x; (void)y; (void)w; (void)ht;
}
static void T_SetCameraFOV(HLOCALOBJ h, float fovX, float fovY) {
    (void)h; (void)fovX; (void)fovY;
}
static bool T_IsCommandOn(int c) { return cmdOn(c); }
static void T_RunConsoleString(const char* s) { (void)s; }
static void T_GetAxisOffsets(LTFLOAT* v) {
    float a[3] = {0, 0, 0};
    takeAxes(a);
    if (v) { v[0] = a[0]; v[1] = a[1]; v[2] = a[2]; }
}
static LTRESULT T_ClearInput() { return LT_OK; }

// ---- tuned subclasses ----
static MemStream* loadRealFile(const std::string& full) {
    FILE* f = fopen(full.c_str(), "rb");
    if (!f) return nullptr;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    MemStream* ms = new MemStream();
    ms->data.resize(len > 0 ? (size_t)len : 0);
    if (len > 0) ms->data.resize(fread(ms->data.data(), 1, (size_t)len, f));
    fclose(f);
    return ms;
}
// ---- real texture data (TGA) + registry shared by Tex/DrawPrim/Font ----
struct TgaImage { uint32_t w = 0, h = 0; std::vector<uint8_t> rgba; };
// Minimal TGA reader: uncompressed true-color (type 2), 24/32 bpp.
// Rows are stored top-first to match Vulkan UV (v=0 at top).
static bool loadTGA(const std::string& path, TgaImage& out) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return false;
    uint8_t h[18];
    bool ok = fread(h, 1, 18, f) == 18;
    uint16_t w = (uint16_t)(h[12] | (h[13] << 8));
    uint16_t hh = (uint16_t)(h[14] | (h[15] << 8));
    ok = ok && h[2] == 2 && (h[16] == 24 || h[16] == 32) && w && hh &&
         w <= 4096 && hh <= 4096;
    long skip = h[0]; // ID field
    if (ok && h[1]) { // colormap present: skip it
        uint16_t n = (uint16_t)(h[5] | (h[6] << 8));
        skip += (long)n * ((h[7] + 7) / 8);
    }
    if (ok) ok = fseek(f, skip, SEEK_CUR) == 0;
    int bpp = h[16] / 8;
    size_t n = (size_t)w * hh;
    std::vector<uint8_t> src;
    if (ok) {
        src.resize(n * (size_t)bpp);
        ok = fread(src.data(), 1, n * (size_t)bpp, f) == n * (size_t)bpp;
    }
    fclose(f);
    if (!ok) return false;
    bool topLeft = (h[17] & 0x20) != 0;
    out.w = w; out.h = hh;
    out.rgba.resize(n * 4);
    for (uint16_t y = 0; y < hh; y++) {
        uint16_t sy = topLeft ? y : (uint16_t)(hh - 1 - y);
        for (uint16_t x = 0; x < w; x++) {
            const uint8_t* s = &src[((size_t)sy * w + x) * (size_t)bpp];
            uint8_t* d = &out.rgba[((size_t)y * w + x) * 4];
            d[0] = s[2]; d[1] = s[1]; d[2] = s[0];
            d[3] = (bpp == 4) ? s[3] : 255;
        }
    }
    return true;
}
// Executable directory (double-click runs start deep inside build-msvc/).
static std::string exeDir() {
    static std::string d = [] {
#ifdef _WIN32
        char buf[1024];
        DWORD n = GetModuleFileNameA(nullptr, buf, (DWORD)sizeof(buf));
        std::string s = n ? buf : "";
#else
        char buf[4096];
        ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
        std::string s = n > 0 ? std::string(buf, (size_t)n) : "";
#endif
        size_t pos = s.find_last_of("/\\");
        return pos == std::string::npos ? std::string() : s.substr(0, pos);
    }();
    return d;
}
// Windows .rez semantics are case-insensitive; emulate that on unix-likes.
static bool tryResolveAsset(const std::string& p, std::string& out) {
    FILE* f = fopen(p.c_str(), "rb");
    if (f) { fclose(f); out = p; return true; }
#ifdef _WIN32
    return false;
#else
    std::vector<std::string> parts;
    std::string cur2;
    for (char ch : p) {
        if (ch == '/') { parts.push_back(cur2); cur2.clear(); }
        else cur2 += ch;
    }
    parts.push_back(cur2);
    std::string cur;
    if (!p.empty() && p[0] == '/') cur = "/";
    for (size_t i = 0; i < parts.size(); i++) {
        if (parts[i].empty()) continue;
        // Case-insensitive glob for one path component (iteration lives
        // inside precompiled libc, opaque to the optimizer).
        std::string pat;
        for (char ch : parts[i]) {
            unsigned char u = (unsigned char)ch;
            if (isalpha(u)) {
                pat += '[';
                pat += (char)toupper(u);
                pat += (char)tolower(u);
                pat += ']';
            } else if (ch == '*' || ch == '?' || ch == '[' || ch == '\\') {
                pat += '\\';
                pat += ch;
            } else {
                pat += ch;
            }
        }
        std::string full = cur.empty() ? pat : (cur == "/" ? cur + pat : cur + "/" + pat);
        glob_t g;
        memset(&g, 0, sizeof(g));
        int r = glob(full.c_str(), GLOB_NOSORT, nullptr, &g);
        bool ok = (r == 0 && g.gl_pathc > 0 && g.gl_pathv && g.gl_pathv[0]);
        if (ok) cur = g.gl_pathv[0];
        globfree(&g);
        if (!ok) return false;
    }
    f = fopen(cur.c_str(), "rb");
    if (!f) return false;
    fclose(f);
    out = cur;
    return true;
#endif
}
// Wrapper: try as-is (cwd), then walk up from the exe dir and cwd so
// double-click runs (cwd = deep build output dir) find repo data files.
static bool resolveAsset(const std::string& p, std::string& out) {
    if (tryResolveAsset(p, out)) return true;
    std::error_code ec;
    std::string roots[2] = { exeDir(), std::filesystem::current_path(ec).string() };
    for (const std::string& root : roots) {
        if (root.empty()) continue;
        std::filesystem::path d(root);
        for (int i = 0; i < 8 && !d.empty(); ++i, d = d.parent_path()) {
            std::filesystem::path cand = d / std::filesystem::path(p);
            if (tryResolveAsset(cand.string(), out)) return true;
        }
    }
    return false;
}
struct TexEntry { uint32_t w = 0, h = 0, vkId = 0; std::string name; };
struct TexRegistry {
    std::map<uintptr_t, TexEntry> byHandle;
    uintptr_t next = 0x100;
};
inline TexRegistry& texRegistry() { static TexRegistry r; return r; }
inline HTEXTURE texRegister(const std::string& name, uint32_t w, uint32_t h,
                            uint32_t vkId) {
    TexRegistry& r = texRegistry();
    for (auto& kv : r.byHandle)
        if (kv.second.name == name) {
            kv.second.w = w; kv.second.h = h; kv.second.vkId = vkId;
            return reinterpret_cast<HTEXTURE>(kv.first);
        }
    uintptr_t k = r.next++;
    TexEntry e; e.w = w; e.h = h; e.vkId = vkId; e.name = name;
    r.byHandle[k] = e;
    return reinterpret_cast<HTEXTURE>(k);
}
inline const TexEntry* texEntry(HTEXTURE t) {
    auto& m = texRegistry().byHandle;
    auto it = m.find(reinterpret_cast<uintptr_t>(t));
    return it == m.end() ? nullptr : &it->second;
}
inline HTEXTURE& boundTexture() { static HTEXTURE t = nullptr; return t; }
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
    static std::string& rezDir() {
        static std::string r;
        return r;
    }
    LTRESULT OpenFile(const char* name, ILTStream** s) override {
        if (!s) return LT_ERROR;
        std::string n = name ? name : "";
        for (size_t i = 0; i < n.size(); i++)
            if (n[i] == '\\') n[i] = '/';
        if (n.find(".raw") != std::string::npos) {
            std::string full = rezDir() + "/" + n;
            MemStream* ms = loadRealFile(full);
            if (ms) {
                unsigned long long sum = 0;
                for (size_t i = 0; i < ms->data.size(); i++)
                    sum += ms->data[i];
                stats().terrainBytes = (int)ms->data.size();
                stats().terrainSum = sum;
                *s = ms;
                emit("real terrain %s (%d bytes sum=%llu)",
                     full.c_str(), (int)ms->data.size(), sum);
                return LT_OK;
            }
            *s = synthTerrain(340, 480);
            emit("synth terrain for %s", n.c_str());
            return LT_OK;
        }
        // General VFS fallback: any engine-relative path resolves against
        // the -rez asset root, so real demo files (Tex/*.dtx, Snd/*.wav,
        // Interface/*.pcx, ...) load with true byte content.
        if (!rezDir().empty()) {
            std::string full = rezDir() + "/" + n;
            MemStream* ms = loadRealFile(full);
            if (ms) {
                *s = ms;
                emit("real file %s (%d bytes)", full.c_str(), (int)ms->data.size());
                return LT_OK;
            }
        }
        return LT_ERROR;
    }
    LTRESULT SendToServer(ILTMessage_Read* m, uint32 f) override {
        if (g_routeToServer) g_routeToServer(m);
        (void)f; return LT_OK;
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
        o->InitNetworking = &T_InitNetworking;
        o->AddInternetDriver = &T_AddInternetDriver;
        o->GetSessionList = &T_GetSessionList;
        o->GetServiceList = &T_GetServiceList;
        o->FreeServiceList = &T_FreeServiceList;
        o->SelectService = &T_SelectService;
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

// Optional Vulkan raster backend for DrawPrim (unix-likes real pixels).
// Coordinates arrive in screen pixels; converted to NDC here.
struct VkBridge {
    static VulkanRenderer*& renderer() {
        static VulkanRenderer* r = nullptr;
        return r;
    }
    static int& width() { static int w = 800; return w; }
    static int& height() { static int h = 600; return h; }
    static void push(float x, float y, float r, float g, float b) {
        VkTriVert v;
        v.x = (x / (float)width()) * 2.0f - 1.0f;
        // Vulkan NDC: y=-1 is the TOP of the viewport (unlike GL/DX).
        v.y = (y / (float)height()) * 2.0f - 1.0f;
        v.r = r; v.g = g; v.b = b; v.a = 1.0f;
        if (renderer()) {
            batchSlot()[batchCount()] = v;
            if (++batchCount() == 3) {
                renderer()->PushTri(batchSlot());
                batchCount() = 0;
            }
        }
    }
    static VkTriVert* batchSlot() {
        static VkTriVert b[3];
        return b;
    }
    static int& batchCount() { static int c = 0; return c; }
    static void rgba(float& r, float& g, float& b, const LT_VERTRGBA& c) {
        r = c.r / 255.0f; g = c.g / 255.0f; b = c.b / 255.0f;
    }
    static void quadTex(float x0, float y0, float x1, float y1,
                        float u0, float v0, float u1, float v1,
                        float r, float g, float b, float a, uint32_t tex) {
        if (!renderer() || !tex) return;
        float w = (float)width(), h = (float)height();
        VkTexVert v[4];
        v[0].x = (x0 / w) * 2.0f - 1.0f; v[0].y = (y0 / h) * 2.0f - 1.0f;
        v[0].u = u0; v[0].v = v0; v[0].r = r; v[0].g = g; v[0].b = b; v[0].a = a;
        v[1].x = (x1 / w) * 2.0f - 1.0f; v[1].y = (y0 / h) * 2.0f - 1.0f;
        v[1].u = u1; v[1].v = v0; v[1].r = r; v[1].g = g; v[1].b = b; v[1].a = a;
        v[2].x = (x1 / w) * 2.0f - 1.0f; v[2].y = (y1 / h) * 2.0f - 1.0f;
        v[2].u = u1; v[2].v = v1; v[2].r = r; v[2].g = g; v[2].b = b; v[2].a = a;
        v[3].x = (x0 / w) * 2.0f - 1.0f; v[3].y = (y1 / h) * 2.0f - 1.0f;
        v[3].u = u0; v[3].v = v1; v[3].r = r; v[3].g = g; v[3].b = b; v[3].a = a;
        renderer()->PushTexQuad(tex, v);
    }
    // Textured triangle via a degenerate quad (last vert repeated).
    static void triTex(float x0, float y0, float u0, float v0,
                       float x1, float y1, float u1, float v1,
                       float x2, float y2, float u2, float v2,
                       float r, float g, float b, float a, uint32_t tex) {
        if (!renderer() || !tex) return;
        float w = (float)width(), h = (float)height();
        VkTexVert v[4];
        v[0].x = (x0 / w) * 2.0f - 1.0f; v[0].y = (y0 / h) * 2.0f - 1.0f;
        v[0].u = u0; v[0].v = v0; v[0].r = r; v[0].g = g; v[0].b = b; v[0].a = a;
        v[1].x = (x1 / w) * 2.0f - 1.0f; v[1].y = (y1 / h) * 2.0f - 1.0f;
        v[1].u = u1; v[1].v = v1; v[1].r = r; v[1].g = g; v[1].b = b; v[1].a = a;
        v[2].x = (x2 / w) * 2.0f - 1.0f; v[2].y = (y2 / h) * 2.0f - 1.0f;
        v[2].u = u2; v[2].v = v2; v[2].r = r; v[2].g = g; v[2].b = b; v[2].a = a;
        v[3] = v[2];
        renderer()->PushTexQuad(tex, v);
    }
};

class DrawPrimTuned : public HostDrawPrim {
public:
    LTRESULT SetTexture(const HTEXTURE t) override {
        boundTexture() = t; return LT_OK;
    }
    LTRESULT DrawPrim(LT_POLYGT3* v, const uint32 n) override {
        stats().drawPrimCalls++;
        stats().drawPrimVerts += (int)n;
        const TexEntry* e = texEntry(boundTexture());
        if (v && e && e->vkId && VkBridge::renderer()) {
            for (uint32 i = 0; i < n; i++) {
                float r = 0, g = 0, b = 0;
                for (int k = 0; k < 3; k++) {
                    float cr, cg, cb;
                    VkBridge::rgba(cr, cg, cb, v[i].verts[k].rgba);
                    r += cr; g += cg; b += cb;
                }
                VkBridge::triTex(v[i].verts[0].x, v[i].verts[0].y,
                                 v[i].verts[0].u, v[i].verts[0].v,
                                 v[i].verts[1].x, v[i].verts[1].y,
                                 v[i].verts[1].u, v[i].verts[1].v,
                                 v[i].verts[2].x, v[i].verts[2].y,
                                 v[i].verts[2].u, v[i].verts[2].v,
                                 r / 3.0f, g / 3.0f, b / 3.0f, 1.0f, e->vkId);
            }
            return LT_OK;
        }
        if (v)
            for (uint32 i = 0; i < n; i++)
                for (int k = 0; k < 3; k++) {
                    float r, g, b;
                    VkBridge::rgba(r, g, b, v[i].verts[k].rgba);
                    VkBridge::push(v[i].verts[k].x, v[i].verts[k].y, r, g, b);
                }
        return LT_OK;
    }
    LTRESULT DrawPrim(LT_POLYFT3* v, const uint32 n) override {
        stats().drawPrimCalls++;
        stats().drawPrimVerts += (int)n;
        const TexEntry* e = texEntry(boundTexture());
        if (v && e && e->vkId && VkBridge::renderer()) {
            for (uint32 i = 0; i < n; i++) {
                float r, g, b;
                VkBridge::rgba(r, g, b, v[i].rgba);
                float a = v[i].rgba.a / 255.0f;
                VkBridge::triTex(v[i].verts[0].x, v[i].verts[0].y,
                                 v[i].verts[0].u, v[i].verts[0].v,
                                 v[i].verts[1].x, v[i].verts[1].y,
                                 v[i].verts[1].u, v[i].verts[1].v,
                                 v[i].verts[2].x, v[i].verts[2].y,
                                 v[i].verts[2].u, v[i].verts[2].v,
                                 r, g, b, a, e->vkId);
            }
            return LT_OK;
        }
        if (v)
            for (uint32 i = 0; i < n; i++) {
                float r, g, b;
                VkBridge::rgba(r, g, b, v[i].rgba);
                for (int k = 0; k < 3; k++)
                    VkBridge::push(v[i].verts[k].x, v[i].verts[k].y, r, g, b);
            }
        return LT_OK;
    }
    LTRESULT DrawPrim(LT_POLYG3* v, const uint32 n) override {
        stats().drawPrimCalls++;
        stats().drawPrimVerts += (int)n;
        if (v)
            for (uint32 i = 0; i < n; i++)
                for (int k = 0; k < 3; k++) {
                    float r, g, b;
                    VkBridge::rgba(r, g, b, v[i].verts[k].rgba);
                    VkBridge::push(v[i].verts[k].x, v[i].verts[k].y, r, g, b);
                }
        return LT_OK;
    }
    LTRESULT DrawPrim(LT_POLYF3* v, const uint32 n) override {
        stats().drawPrimCalls++;
        stats().drawPrimVerts += (int)n;
        if (v)
            for (uint32 i = 0; i < n; i++) {
                float r, g, b;
                VkBridge::rgba(r, g, b, v[i].rgba);
                for (int k = 0; k < 3; k++)
                    VkBridge::push(v[i].verts[k].x, v[i].verts[k].y, r, g, b);
            }
        return LT_OK;
    }
    LTRESULT DrawPrim(LT_POLYFT4* v, const uint32 n) override {
        stats().drawPrimCalls += (int)n;
        stats().drawPrimVerts += (int)n * 4;
        const TexEntry* e = texEntry(boundTexture());
        if (!v || !e || !e->vkId || !VkBridge::renderer()) return LT_OK;
        for (uint32 i = 0; i < n; i++) {
            float r, g, b;
            VkBridge::rgba(r, g, b, v[i].rgba);
            float a = v[i].rgba.a / 255.0f;
            VkBridge::quadTex(v[i].verts[0].x, v[i].verts[0].y,
                              v[i].verts[2].x, v[i].verts[2].y,
                              v[i].verts[0].u, v[i].verts[0].v,
                              v[i].verts[2].u, v[i].verts[2].v,
                              r, g, b, a, e->vkId);
        }
        return LT_OK;
    }
    LTRESULT DrawPrim(LT_POLYGT4* v, const uint32 n) override {
        stats().drawPrimCalls += (int)n;
        stats().drawPrimVerts += (int)n * 4;
        const TexEntry* e = texEntry(boundTexture());
        if (!v || !e || !e->vkId || !VkBridge::renderer()) return LT_OK;
        for (uint32 i = 0; i < n; i++) {
            float r = 0, g = 0, b = 0;
            for (int k = 0; k < 4; k++) {
                float cr, cg, cb;
                VkBridge::rgba(cr, cg, cb, v[i].verts[k].rgba);
                r += cr; g += cg; b += cb;
            }
            VkBridge::quadTex(v[i].verts[0].x, v[i].verts[0].y,
                              v[i].verts[2].x, v[i].verts[2].y,
                              v[i].verts[0].u, v[i].verts[0].v,
                              v[i].verts[2].u, v[i].verts[2].v,
                              r / 4.0f, g / 4.0f, b / 4.0f, 1.0f, e->vkId);
        }
        return LT_OK;
    }
};

class CommonTuned : public HostCommon {
public:
    LTRESULT SetupEuler(LTRotation& r, float p, float y, float rl) override {
        // R4: euler REAL (igual que HostILTCommon): el stub en identidad
        // hacia que todo lo cliente (movimiento, camara) mirara a +Z.
        r = LTRotation(p, y, rl);
        return LT_OK;
    }
    LTRESULT CreateMessage(ILTMessage_Write*& m) override {
        if (g_createClientMessage) {
            m = g_createClientMessage();
            return LT_OK;
        }
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
    LTRESULT UpdateMovement(MoveInfo* i) override {
        if (!i) return LT_ERROR;
        ObjState* o = findObj(i->m_hObject);
        if (!o) return LT_ERROR;
        i->m_Offset = o->vel;
        i->m_Offset.x *= i->m_dt; i->m_Offset.y *= i->m_dt; i->m_Offset.z *= i->m_dt;
        return LT_OK;
    }
};



class UIFontTuned : public HostUIFont {
public:
    HTEXTURE tex = reinterpret_cast<HTEXTURE>((intptr_t)0x20);
    HTEXTURE GetTexture() override { return tex; }
    uint32 defColor = 0xFFFFFFFF;
    void SetDefColor(uint32 argb) override { defColor = argb; }
    struct Glyph {
        float u0 = 0, v0 = 0, u1 = 0, v1 = 0;
        float w = 0, h = 0, adv = 8, bx = 0, by = 12;
    };
    Glyph glyphs[256];
    bool hasAtlas = false;
    uint32 lineH = 16;
};


class FontTuned : public HostFontManager {
public:
    CUIFont* makeFont(char const* f) {
        UIFontTuned* fnt = new UIFontTuned();
#ifdef HAS_FREETYPE
        std::string name = f ? f : "";
        for (char& c : name) if (c == '\\') c = '/';
        std::string base = ClientTuned::rezDir();
        std::string path = base.empty() ? name : base + "/" + name;
        MemStream* ms = nullptr;
        // Reuse the tuned file search: try rez root then cwd.
        {
            std::string rp;
            if (!resolveAsset(path, rp) && !base.empty())
                resolveAsset(name, rp);
            FILE* tf = rp.empty() ? nullptr : fopen(rp.c_str(), "rb");
            if (tf) {
                fseek(tf, 0, SEEK_END);
                long len = ftell(tf);
                fseek(tf, 0, SEEK_SET);
                if (len > 0) {
                    std::vector<uint8_t> buf((size_t)len);
                    if (fread(buf.data(), 1, (size_t)len, tf) == (size_t)len) {
                        ms = new MemStream();
                        ms->data = std::move(buf);
                    }
                }
                fclose(tf);
            }
        }
        if (ms && !ms->data.empty() && VkBridge::renderer()) {
            FT_Library lib = nullptr;
            if (FT_Init_FreeType(&lib) == 0) {
                FT_Face face = nullptr;
                if (FT_New_Memory_Face(lib, ms->data.data(),
                                       (FT_Long)ms->data.size(), 0,
                                       &face) == 0) {
                    uint32_t px = 16;
                    if (FT_Set_Pixel_Sizes(face, 0, px) == 0)
                        bakeAtlas(face, px, path, fnt);
                    FT_Done_Face(face);
                }
                FT_Done_FreeType(lib);
            }
            delete ms;
        } else {
            delete ms;
        }
#else
        (void)f;
#endif
        return fnt;
    }
#ifdef HAS_FREETYPE
    void bakeAtlas(FT_Face face, uint32_t px, const std::string& path,
                   UIFontTuned* fnt) {
        const int COLS = 16, ROWS = 16;
        int cell = (int)px + 4;
        uint32_t aw = (uint32_t)(COLS * cell), ah = (uint32_t)(ROWS * cell);
        std::vector<uint8_t> atlas((size_t)aw * ah * 4, 0);
        for (int c = 0; c < 256; c++)
            fnt->glyphs[c].adv = (float)px / 2.0f;
        for (int c = 0; c < 256; c++) {
            if (FT_Load_Char(face, (FT_ULong)c, FT_LOAD_RENDER)) continue;
            FT_Bitmap& bm = face->glyph->bitmap;
            if (!bm.buffer || bm.pitch <= 0) continue;
            int gx = (c % COLS) * cell, gy = (c / COLS) * cell;
            for (unsigned r = 0; r < bm.rows && r < (unsigned)cell; r++)
                for (unsigned q = 0; q < bm.width && q < (unsigned)cell; q++) {
                    uint8_t cov = bm.buffer[r * bm.pitch + q];
                    uint8_t* d = &atlas[((size_t)(gy + r) * aw + gx + q) * 4];
                    d[0] = d[1] = d[2] = 255;
                    d[3] = cov;
                }
            UIFontTuned::Glyph& g = fnt->glyphs[c];
            g.u0 = (float)gx / aw; g.v0 = (float)gy / ah;
            g.u1 = (float)(gx + bm.width) / aw;
            g.v1 = (float)(gy + bm.rows) / ah;
            g.w = (float)bm.width; g.h = (float)bm.rows;
            float adv = (float)(face->glyph->advance.x >> 6);
            g.adv = adv > 0 ? adv : (float)px / 2.0f;
            g.bx = (float)face->glyph->bitmap_left;
            g.by = (float)face->glyph->bitmap_top;
        }
        uint32_t id = VkBridge::renderer()->RegisterTexture(aw, ah,
                                                            atlas.data());
        if (id) {
            fnt->tex = texRegister("font/atlas", aw, ah, id);
            fnt->hasAtlas = true;
            fnt->lineH = px;
            emit("real font atlas %s (%ux%u)", path.c_str(), aw, ah);
        }
    }
#endif
    CUIFont* CreateFont(char const* f, char const* fc, uint32 s,
                        uint8 a, uint8 b, LTFontParams* p) override {
        (void)fc; (void)s; (void)a; (void)b; (void)p;
        return makeFont(f);
    }
    CUIFont* CreateFont(char const* f, char const* fc, uint32 s,
                        char* c, LTFontParams* p) override {
        (void)fc; (void)s; (void)c; (void)p;
        return makeFont(f);
    }
    CUIFormattedPolyString* CreateFormattedPolyString(CUIFont* f, char* b,
            float x, float y, CUI_ALIGNMENTTYPE a) override {
        (void)b; (void)x; (void)y; (void)a;
        PolyTunedAsFormatted* p = new PolyTunedAsFormatted();
        p->font = static_cast<UIFontTuned*>(f);
        return p;
    }
    // Formatted string with real layout state (base ctors/dtors defined below)
    struct PolyTunedAsFormatted : public CUIFormattedPolyString {
        PolyTunedAsFormatted() : CUIFormattedPolyString(nullptr) {}
        std::string text;
        float px = 0, py = 0;
        UIFontTuned* font = nullptr;
        CUI_RESULTTYPE SetText(const char* b) override {
            text = b ? b : ""; return CUIR_OK;
        }
        float lineAdvance() const {
            return font && font->hasAtlas ? (float)font->lineH : 16.0f;
        }
        float textWidth() const {
            float w = 0, best = 0;
            for (char ch : text) {
                if (ch == '\n') { if (w > best) best = w; w = 0; continue; }
                unsigned char c = (unsigned char)ch;
                w += (font && font->hasAtlas) ? font->glyphs[c].adv : 8.0f;
            }
            return w > best ? w : best;
        }
        float GetWidth() override { return textWidth(); }
        float GetHeight() override {
            float lines = 1;
            for (char ch : text) if (ch == '\n') lines++;
            return lines * lineAdvance();
        }
        CUI_RESULTTYPE SetPosition(float x, float y) override {
            px = x; py = y; return CUIR_OK;
        }
        CUI_RESULTTYPE GetDims(float* w, float* h) override {
            if (w) *w = GetWidth(); if (h) *h = GetHeight(); return CUIR_OK;
        }
        CUI_RESULTTYPE Render(int32 s, int32 e) override {
            (void)s; (void)e; stats().uiRenders++;
            if (text.empty() || !VkBridge::renderer()) return CUIR_OK;
            const TexEntry* en =
                (font && font->hasAtlas) ? texEntry(font->tex) : nullptr;
            if (en && en->vkId) {
                // Real glyph quads from the baked atlas.
                uint32_t c = font->defColor;
                float a = ((c >> 24) & 255) / 255.0f;
                float r = ((c >> 16) & 255) / 255.0f;
                float g = ((c >> 8) & 255) / 255.0f;
                float b = (c & 255) / 255.0f;
                if (a <= 0) a = 1.0f;
                float lh = lineAdvance();
                float pen = px, yy = py;
                int quads = 0;
                for (char ch : text) {
                    if (ch == '\n') { pen = px; yy += lh; continue; }
                    const UIFontTuned::Glyph& gl =
                        font->glyphs[(unsigned char)ch];
                    if (gl.w > 0 && gl.h > 0) {
                        float x0 = pen + gl.bx, y0 = yy + (lh - gl.by);
                        VkBridge::quadTex(x0, y0, x0 + gl.w, y0 + gl.h,
                                          gl.u0, gl.v0, gl.u1, gl.v1,
                                          r, g, b, a, en->vkId);
                        quads++;
                    }
                    pen += gl.adv;
                }
                stats().drawPrimCalls += quads * 2;
                return CUIR_OK;
            }
            // Legacy fallback: bounds quad when no atlas is available.
            float w = GetWidth(), h = GetHeight();
            if (w > 0 && h > 0) {
                float x0 = px, y0 = py, x1 = px + w, y1 = py + h;
                float r = 0.75f, g = 0.78f, b = 0.85f;
                VkBridge::push(x0, y0, r, g, b);
                VkBridge::push(x1, y0, r, g, b);
                VkBridge::push(x0, y1, r, g, b);
                VkBridge::push(x1, y0, r, g, b);
                VkBridge::push(x1, y1, r, g, b);
                VkBridge::push(x0, y1, r, g, b);
                stats().drawPrimCalls += 2;
            }
            return CUIR_OK;
        }
    };
};

class TexTuned : public HostTexInterface {
public:
    LTRESULT loadName(const char* n, HTEXTURE& t) {
        std::string name = n ? n : "";
        for (char& c : name) if (c == '\\') c = '/';
        for (auto& kv : texRegistry().byHandle)
            if (kv.second.name == name) {
                t = reinterpret_cast<HTEXTURE>(kv.first);
                return LT_OK;
            }
        std::string base = ClientTuned::rezDir();
        std::string path;
        auto probe = [&](const std::string& rel) {
            if (!path.empty()) return;
            std::string c;
            if (!base.empty() && resolveAsset(base + "/" + rel, c)) path = c;
            else if (resolveAsset(rel, c)) path = c;
        };
        probe(name);
        TgaImage img;
        bool loaded = !path.empty() && loadTGA(path, img);
        if (!loaded && name.size() > 4 &&
            name.compare(name.size() - 4, 4, ".dtx") == 0) {
            // DTX is proprietary; use the shipped .tga twin when present.
            path.clear();
            probe(name.substr(0, name.size() - 4) + ".tga");
            loaded = !path.empty() && loadTGA(path, img);
        }
        if (!loaded) {
            emit("texture %s not found or unreadable, placeholder",
                 name.c_str());
            t = reinterpret_cast<HTEXTURE>((intptr_t)0x30);
            return LT_OK;
        }
        uint32_t id = 0;
        if (VkBridge::renderer())
            id = VkBridge::renderer()->RegisterTexture(img.w, img.h,
                                                       img.rgba.data());
        if (!id) {
            emit("texture %s upload failed, placeholder", path.c_str());
            t = reinterpret_cast<HTEXTURE>((intptr_t)0x30);
            return LT_OK;
        }
        t = texRegister(name, img.w, img.h, id);
        emit("real texture %s (%ux%u)", path.c_str(), img.w, img.h);
        return LT_OK;
    }
    LTRESULT GetTextureDims(const HTEXTURE t, uint32& w, uint32& h) override {
        const TexEntry* e = texEntry(t);
        if (!e) { w = 64; h = 64; return LT_OK; } // legacy placeholder dims
        w = e->w; h = e->h;
        return LT_OK;
    }
    LTRESULT FindTextureFromName(HTEXTURE& t, const char* n) override {
        return loadName(n, t);
    }
    LTRESULT CreateTextureFromName(HTEXTURE& t, const char* n) override {
        return loadName(n, t);
    }
};

} // namespace Host — the UI stubs below define ::CUIPolyString/::CUIFormattedPolyString
  // (global SDK classes); defining them inside the namespace is ill-formed on MSVC.


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
