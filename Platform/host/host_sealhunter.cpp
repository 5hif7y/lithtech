// Bootstrap for the sealhunter demo (networking).
// Same host backend as drawprim/bump. Headless/probe runs drive
// StartNormalGame() (the headless equivalent of typing "normal" at the
// console); interactive --window boots to the menu like run.bat so the
// menu receives input. Exit 0 only with frames>0 and no shutdown.
// Prints HOST_RESULT line.
#include "Platform/host/host_engine.h"
#include "Platform/host/host_server.h"
#include "Platform/host/host_world.h"
#include "Platform/host/host_model.h"
// Clases del juego para el melee R4 (cast estatico: la fabrica las crea por
// nombre y el host las identifica por ServerObj::name).
#include "seal.h"
#include "playersrvr.h"
#include "ltclientshell.h"
#include "ltservershell.h"
#ifdef _LINUX
#ifdef HAS_SDL3
// Via principal SDL3 (ver host_sdl3.h; ese TU es el unico que incluye
// headers SDL3: SDL2 y SDL3 no pueden mezclarse en un mismo .cpp).
// Los keycodes SDL2/SDL3 coinciden en valor para las teclas mapeadas
// (diseno estable de SDL), asi que SDLK_* via SDL2 sirve para ambos.
#include <SDL2/SDL.h>
#include "Platform/host/host_sdl3.h"
// SDL keycode -> (Windows VK, engine command); mirrors Host::mapKeysym.
static void mapSDLKey(int sym, int& vk, int& cmd) {
    vk = -1; cmd = -1;
    switch (sym) {
    case SDLK_UP: vk = Host::HVK_UP; cmd = 1; break;
    case SDLK_DOWN: vk = Host::HVK_DOWN; cmd = 2; break;
    case SDLK_LEFT: vk = Host::HVK_LEFT; cmd = 3; break;
    case SDLK_RIGHT: vk = Host::HVK_RIGHT; cmd = 4; break;
    case SDLK_RETURN: case SDLK_KP_ENTER: vk = Host::HVK_RETURN; cmd = 18; break;
    case SDLK_SPACE: vk = Host::HVK_SPACE; cmd = 16; break;
    case SDLK_TAB: vk = Host::HVK_TAB; cmd = 17; break;
    case SDLK_ESCAPE: vk = Host::HVK_ESCAPE; cmd = 250; break;
    case SDLK_F12: vk = Host::HVK_F12; cmd = -1; break;
    default:
        if ((sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <= 'Z')) {
            int u = toupper(sym);
            vk = u;
            if (u == 'W') cmd = 1;
            else if (u == 'S') cmd = 2;
            else if (u == 'A') cmd = 3;
            else if (u == 'D') cmd = 4;
            else if (u == 'T') cmd = 19;
        } else if (sym >= '0' && sym <= '9') {
            vk = sym;
        }
        break;
    }
}
#endif
// Fallback (y --x11): ventana nativa X11.
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <cstdlib>
#include <ctime>
#else
#include <SDL2/SDL.h>
// SDL keycode -> (Windows VK, engine command); mirrors Host::mapKeysym.
static void mapSDLKey(int sym, int& vk, int& cmd) {
    vk = -1; cmd = -1;
    switch (sym) {
    case SDLK_UP: vk = Host::HVK_UP; cmd = 1; break;
    case SDLK_DOWN: vk = Host::HVK_DOWN; cmd = 2; break;
    case SDLK_LEFT: vk = Host::HVK_LEFT; cmd = 3; break;
    case SDLK_RIGHT: vk = Host::HVK_RIGHT; cmd = 4; break;
    case SDLK_RETURN: case SDLK_KP_ENTER: vk = Host::HVK_RETURN; cmd = 18; break;
    case SDLK_SPACE: vk = Host::HVK_SPACE; cmd = 16; break;
    case SDLK_TAB: vk = Host::HVK_TAB; cmd = 17; break;
    case SDLK_ESCAPE: vk = Host::HVK_ESCAPE; cmd = 250; break;
    case SDLK_F12: vk = Host::HVK_F12; cmd = -1; break;
    default:
        if ((sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <= 'Z')) {
            int u = toupper(sym);
            vk = u;
            if (u == 'W') cmd = 1;
            else if (u == 'S') cmd = 2;
            else if (u == 'A') cmd = 3;
            else if (u == 'D') cmd = 4;
            else if (u == 'T') cmd = 19;
        } else if (sym >= '0' && sym <= '9') {
            vk = sym;
        }
        break;
    }
}
#endif
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#ifdef _WIN32
#include <SDL2/SDL_syswm.h>
// HWND de una ventana SDL sin pasar por SDL_Vulkan_* (ausente en el
// SDL2 de vcpkg). Solo Windows.
static void* SdlHwnd(SDL_Window* w) {
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (!w || !SDL_GetWindowWMInfo(w, &info)) return nullptr;
    if (info.subsystem != SDL_SYSWM_WINDOWS) return nullptr;
    return info.info.win.window;
}
#endif

extern ILTClient* g_pLTClient;
extern ILTDrawPrim* g_pLTCDrawPrim;
extern ILTCommon* g_pLTCCommon;
extern ILTPhysics* g_pLTCPhysics;
extern ILTClientSoundMgr* g_pLTCSoundMgr;
extern ILTFontManager* g_pLTCFontManager;
extern ILTTexInterface* g_pLTCTexInterface;
extern ILTModelClient* g_pLTCModel;
extern ILTWidgetManager* g_pLTCWidgetManager;

// ---- ILTPhysics base fallbacks ----
// HostPhysics/PhysicsTuned override every ILTPhysics virtual, but MSVC still
// needs definitions for the non-pure base virtuals to emit the base vtables
// (their real bodies live in the full engine,
// Engine/Engine/runtime/shared/src/shared_iltphysics.cpp, which demos don't
// build). These are never called through a base subobject; LT_ERROR signals
// misuse. Single TU => no ODR risk.
LTRESULT ILTPhysics::IsWorldObject(HOBJECT) { return LT_ERROR; }
LTRESULT ILTPhysics::GetMass(HOBJECT, float*) { return LT_ERROR; }
LTRESULT ILTPhysics::SetMass(HOBJECT, float) { return LT_ERROR; }
LTRESULT ILTPhysics::GetFrictionCoefficient(HOBJECT, float*) { return LT_ERROR; }
LTRESULT ILTPhysics::SetFrictionCoefficient(HOBJECT, float) { return LT_ERROR; }
LTRESULT ILTPhysics::GetObjectDims(HOBJECT, LTVector*) { return LT_ERROR; }
LTRESULT ILTPhysics::GetVelocity(HOBJECT, LTVector*) { return LT_ERROR; }
LTRESULT ILTPhysics::GetForceIgnoreLimit(HOBJECT, float&) { return LT_ERROR; }
LTRESULT ILTPhysics::SetForceIgnoreLimit(HOBJECT, float) { return LT_ERROR; }
LTRESULT ILTPhysics::GetAcceleration(HOBJECT, LTVector*) { return LT_ERROR; }
LTRESULT ILTPhysics::GetStandingOn(HOBJECT, CollisionInfo*) { return LT_ERROR; }

namespace Host {
// Definicion unica de notePlayer (declarada en host_engine.h).
void notePlayer(float x, float y, float z, float yaw, float pitch) {
    PlayerPub& p = playerPub();
    p.x = x; p.y = y; p.z = z; p.yaw = yaw; p.pitch = pitch;
    p.valid = true;
}
// R1 probe: synthetic 3D scene through the mesh pipeline (perspective +
// depth). Builds its own headless renderer, snapshots, and counts pixels:
// ok requires non-clear coverage, some red (rear box cap above the front
// box), and red strictly inside the non-clear set (depth resolved).
namespace {
void meshTri(VulkanRenderer& vk, uint32_t tex, float ax, float ay, float az,
             float au, float av, float bx, float by, float bz, float bu,
             float bv, float cx, float cy, float cz, float cu, float cv,
             float r, float g, float b) {
    VkMeshVert v[3];
    v[0].x = ax; v[0].y = ay; v[0].z = az; v[0].u = au; v[0].v = av;
    v[1].x = bx; v[1].y = by; v[1].z = bz; v[1].u = bu; v[1].v = bv;
    v[2].x = cx; v[2].y = cy; v[2].z = cz; v[2].u = cu; v[2].v = cv;
    for (int k = 0; k < 3; k++) {
        v[k].r = r; v[k].g = g; v[k].b = b; v[k].a = 1.0f;
    }
    vk.PushTri3D(tex, v);
}
void meshQuad(VulkanRenderer& vk, uint32_t tex, float ax, float ay, float az,
              float bx, float by, float bz, float cx, float cy, float cz,
              float dx, float dy, float dz, float r, float g, float b) {
    meshTri(vk, tex, ax, ay, az, 0, 0, bx, by, bz, 1, 0, cx, cy, cz, 1, 1,
            r, g, b);
    meshTri(vk, tex, ax, ay, az, 0, 0, cx, cy, cz, 1, 1, dx, dy, dz, 0, 1,
            r, g, b);
}
void meshCube(VulkanRenderer& vk, uint32_t tex, float cx, float cy, float cz,
              float sx, float sy, float sz, float r, float g, float b) {
    const float x0 = cx - sx / 2, x1 = cx + sx / 2;
    const float y0 = cy - sy / 2, y1 = cy + sy / 2;
    const float z0 = cz - sz / 2, z1 = cz + sz / 2;
    meshQuad(vk, tex, x0, y0, z0, x1, y0, z0, x1, y1, z0, x0, y1, z0, r, g, b);
    meshQuad(vk, tex, x0, y0, z1, x0, y1, z1, x1, y1, z1, x1, y0, z1, r, g, b);
    meshQuad(vk, tex, x0, y0, z0, x0, y0, z1, x1, y0, z1, x1, y0, z0, r, g, b);
    meshQuad(vk, tex, x0, y1, z0, x1, y1, z0, x1, y1, z1, x0, y1, z1, r, g, b);
    meshQuad(vk, tex, x0, y0, z0, x0, y1, z0, x0, y1, z1, x0, y0, z1, r, g, b);
    meshQuad(vk, tex, x1, y0, z0, x1, y0, z1, x1, y1, z1, x1, y1, z0, r, g, b);
}
} // namespace

int runMeshTest(const std::string& ppm) {
    VulkanRenderer vk;
    if (!vk.InitHeadless(800, 600)) {
        printf("MESH_RESULT ok=0 stage=vkinit\n");
        return 1;
    }
    // Checker 64x64 + white 1x1.
    std::vector<uint8_t> chk(64 * 64 * 4);
    for (int y = 0; y < 64; y++)
        for (int x = 0; x < 64; x++) {
            const bool w = (((x / 8) + (y / 8)) & 1) == 0;
            uint8_t* p = &chk[(size_t)(y * 64 + x) * 4];
            p[0] = p[1] = p[2] = w ? 235 : 40;
            p[3] = 255;
        }
    const uint8_t white[4] = {255, 255, 255, 255};
    const uint32_t tChk = vk.RegisterTexture(64, 64, chk.data());
    const uint32_t tWhite = vk.RegisterTexture(1, 1, white);
    if (!tChk || !tWhite) {
        printf("MESH_RESULT ok=0 stage=tex\n");
        return 1;
    }
    // Camera (0,3,-8) looking at (0,1,0), fovX=90 (shared math, host_world.h).
    const Vec3 tgt{0, 1, 0}, pos{0, 3, -8};
    float VP[16];
    lookAt4(pos, tgt, 90.0f, 800.0f / 600.0f, 0.5f, 100.0f, VP);
    vk.SetViewProj(VP);
    vk.Clear(0xFF334455);
    // Ground grid x[-6,6] z[-4,8] at y=0.
    for (int ix = 0; ix < 12; ix++)
        for (int iz = 0; iz < 12; iz++) {
            const float x0 = -6.0f + (float)ix, z0 = -4.0f + (float)iz;
            meshQuad(vk, tChk, x0, 0, z0, x0 + 1, 0, z0, x0 + 1, 0, z0 + 1,
                     x0, 0, z0 + 1, 1, 1, 1);
        }
    // Front box (textured) + taller rear red box (body occluded by front).
    meshCube(vk, tChk, 0, 1.2f, 2, 2, 2, 2, 1, 1, 1);
    meshCube(vk, tWhite, 0.5f, 1.4f, 4.5f, 1.6f, 2.8f, 1.6f, 1, 0.15f, 0.1f);
    const size_t tris = vk.PendingMeshTris();
    if (!vk.SnapshotPPM(ppm.c_str())) {
        printf("MESH_RESULT ok=0 stage=snap tris=%u\n", (unsigned)tris);
        return 1;
    }
    // Verify pixels: clear is 0x334455 -> PPM stores RGBA bytes (B,G,R order
    // on disk? No: R8G8B8A8 -> bytes R,G,B,A; snapshot writes first 3 = R,G,B).
    FILE* fp = fopen(ppm.c_str(), "rb");
    bool ok = false;
    size_t nonClear = 0, red = 0;
    if (fp) {
        char magic[3] = {0, 0, 0};
        unsigned w = 0, h = 0, mx = 0;
        if (fscanf(fp, "%2s %u %u %u", magic, &w, &h, &mx) == 4 &&
            w == 800 && h == 600 && mx == 255) {
            fgetc(fp);
            std::vector<uint8_t> px((size_t)w * h * 3);
            if (fread(px.data(), 1, px.size(), fp) == px.size()) {
                for (size_t i = 0; i < (size_t)w * h; i++) {
                    const unsigned R = px[i * 3], G = px[i * 3 + 1],
                                   B = px[i * 3 + 2];
                    const bool isClear =
                        (R >= 0x2B && R <= 0x3B) && (G >= 0x3C && G <= 0x4C) &&
                        (B >= 0x4D && B <= 0x5D);
                    if (!isClear) {
                        nonClear++;
                        if (R > 150 && G < 90 && B < 90) red++;
                    }
                }
                ok = nonClear > 50000 && red > 100 && red < nonClear / 2;
            }
        }
        fclose(fp);
    }
    printf("MESH: tris=%u nonClear=%u red=%u\n", (unsigned)tris,
           (unsigned)nonClear, (unsigned)red);
    printf("MESH_RESULT ok=%d tris=%u\n", ok ? 1 : 0, (unsigned)tris);
    return ok ? 0 : 1;
}

struct WorldTexCache {
    std::map<std::string, uint32_t> id;
    unsigned hits = 0, miss = 0, sprites = 0;
};
static bool endsWithI(const std::string& s, const char* suf) {
    const size_t n = strlen(suf);
    if (s.size() < n) return false;
    for (size_t i = 0; i < n; i++)
        if (tolower(s[s.size() - n + i]) != suf[i]) return false;
    return true;
}
uint32_t resolveWorldTex(const std::string& name, VulkanRenderer& vk,
                         WorldTexCache& c) {
    std::map<std::string, uint32_t>::iterator it = c.id.find(name);
    if (it != c.id.end()) {
        if (it->second) c.hits++;
        return it->second;
    }
    uint32_t id = 0;
    std::string n = name;
    for (size_t i = 0; i < n.size(); i++)
        if (n[i] == '\\') n[i] = '/';
    if (endsWithI(n, ".spr")) {
        c.sprites++;
        c.id[name] = 0;
        return 0;
    }
    std::string base = ClientTuned::rezDir();
    if (base.empty()) base = "demo-sealhunter/sealhunter/rez";
    std::string cands[4];
    cands[0] = base + "/" + n;
    cands[1] = cands[0] + ".dtx";
    std::string bn = n;
    {
        const size_t sl = bn.find_last_of('/');
        if (sl != std::string::npos) bn = bn.substr(sl + 1);
    }
    cands[2] = base + "/Tex/" + bn;
    cands[3] = cands[2] + ".dtx";
    std::string tried;
    for (int i = 0; i < 4 && !id; i++) {
        std::string rp;
        // twin .tga primero (legible sin decode), luego el candidato tal cual
        std::string tga = cands[i];
        {
            const size_t dot = tga.find_last_of('.');
            const size_t sl = tga.find_last_of('/');
            if (dot != std::string::npos &&
                (sl == std::string::npos || dot > sl))
                tga = tga.substr(0, dot);
            tga += ".tga";
        }
        if (resolveAsset(tga, rp)) {
            TgaImage img;
            if (loadTGA(rp, img) && img.w && img.h)
                id = vk.RegisterTexture(img.w, img.h, img.rgba.data());
        }
        if (!id && resolveAsset(cands[i], rp)) {
            if (endsWithI(rp, ".tga")) {
                TgaImage img;
                if (loadTGA(rp, img) && img.w && img.h)
                    id = vk.RegisterTexture(img.w, img.h, img.rgba.data());
            } else {
                uint32_t w = 0, h = 0;
                std::vector<uint8_t> rgba;
                if (decodeDTX(rp, w, h, rgba) && w && h)
                    id = vk.RegisterTexture(w, h, rgba.data());
            }
        }
        tried += (i ? "," : "") + cands[i];
    }
    if (id)
        c.hits++;
    else {
        c.miss++;
        if (c.miss <= 6)
            printf("[world] tex miss: '%s' tried %s\n", n.c_str(),
                   tried.c_str());
    }
    c.id[name] = id;
    return id;
}

static bool isSkyTex(const std::string& n) {
    // Secciones del skybox (TexFX\Sky\...): el box vive en coords fijas de
    // autor; el engine lo centra en la camara (SkyDef). Detectamos por prefijo.
    if (n.size() < 5) return false;
    return (tolower(n[0]) == 't' && tolower(n[1]) == 'e' &&
            tolower(n[2]) == 'x' && tolower(n[3]) == 'f' &&
            tolower(n[4]) == 'x');
}
size_t drawWorldMesh(VulkanRenderer& vk, const WorldData& w, WorldTexCache& c,
                     unsigned& secDrawn, unsigned& secSkip,
                     bool fullBright = false, const float* campos = nullptr) {
    size_t tris = 0;
    VkMeshVert v[3];
    // Aislamiento de bloques para diagnostico: SEAL_BLOCKS="0,3,5".
    bool only[256];
    bool useOnly = false;
    for (int i = 0; i < 256; i++) only[i] = false;
    if (const char* env = getenv("SEAL_BLOCKS")) {
        useOnly = true;
        int a = -1, b = -1;
        for (const char* p = env;; p++) {
            if (*p >= '0' && *p <= '9') {
                if (a < 0) a = 0;
                a = a * 10 + (*p - '0');
            } else {
                if (a >= 0 && a < 256) only[a] = true;
                a = -1;
                if (!*p) break;
            }
        }
        (void)b;
    }
    for (size_t bi = 0; bi < w.blocks.size(); bi++) {
        if (useOnly && !only[bi]) continue;
        const WorldBlock& b = w.blocks[bi];
        // Skybox sigue a la camara (ver isSkyTex).
        float ox = 0, oy = 0, oz = 0;
        if (campos && !b.sections.empty() && isSkyTex(b.sections[0].tex0)) {
            ox = campos[0] - b.cx;
            oy = campos[1] - b.cy;
            oz = campos[2] - b.cz;
        }
        for (size_t si = 0; si < b.sections.size(); si++) {
            const WorldSection& s = b.sections[si];
            uint32_t tex = 0;
            if (!s.tex0.empty()) tex = resolveWorldTex(s.tex0, vk, c);
            if (!tex) {
                secSkip++;
                continue;
            }
            secDrawn++;
            for (uint32_t t = 0; t < s.triCount; t++) {
                for (int k = 0; k < 3; k++) {
                    const WorldVert& sv =
                        b.verts[b.idx[(size_t)(s.triStart + t) * 3 + (size_t)k]];
                    v[k].x = sv.x + ox; v[k].y = sv.y + oy; v[k].z = sv.z + oz;
                    v[k].u = sv.u; v[k].v = sv.v;
                    if (fullBright) {
                        v[k].r = v[k].g = v[k].b = v[k].a = 1.0f;
                    } else {
                        v[k].r = sv.r; v[k].g = sv.g; v[k].b = sv.b;
                        v[k].a = sv.a;
                    }
                }
                vk.PushTri3D(tex, v);
                tris++;
            }
        }
    }
    return tris;
}

// Frame en vivo: mundo persistente (carga una vez) + VP de la camara cliente.
void drawWorldFrame(VulkanRenderer& vk) {
    if (!ServerWorld::instance().clientEntered) return;
    if (!worldTried()) {
        worldTried() = true;
        std::string base = ClientTuned::rezDir();
        if (base.empty()) base = "demo-sealhunter/sealhunter/rez";
        std::string rp;
        WorldData& wd = worldStore();
        if ((resolveAsset(base + "/Worlds/World.dat", rp) ||
             resolveAsset("Worlds/World.dat", rp)) &&
            loadWorldDat(rp, wd) && !wd.blocks.empty()) {
            worldReady() = true;
            printf("[world] live: blocks=%u tris=%u (%s)\n",
                   (unsigned)wd.blocks.size(), wd.totalTris, rp.c_str());
        } else {
            printf("[world] live load FAILED\n");
        }
        fflush(stdout);
    }
    if (!worldReady()) return;
    WorldData* w = &worldStore();
    static WorldTexCache tc;
    const ObjState* cam = nullptr;
    for (std::map<HLOCALOBJ, ObjState>::iterator it = objects().begin();
         it != objects().end(); ++it) {
        if (it->second.type == 5 /*OT_CAMERA*/) {
            cam = &it->second;
            break;
        }
    }
    if (!cam) return;
    const float cp[3] = {cam->pos.x, cam->pos.y, cam->pos.z};
    const LTVector F = cam->rot.Forward(), R = cam->rot.Right(),
                   U = cam->rot.Up();
    const float rf[3] = {R.x, R.y, R.z}, uf[3] = {U.x, U.y, U.z},
                ff[3] = {F.x, F.y, F.z};
    float VP[16];
    vpFromBasis4(rf, uf, ff, cp, 90.0f,
                 (float)VkBridge::width() / (float)VkBridge::height(), 1.0f,
                 20000.0f, VP);
    vk.SetViewProj(VP);
    unsigned sd = 0, ss = 0;
    drawWorldMesh(vk, *w, tc, sd, ss, true, cp);
}

int runWorldTest(const std::string& ppm) {
    std::string base = ClientTuned::rezDir();
    if (base.empty()) base = "demo-sealhunter/sealhunter/rez";
    std::string rp;
    if (!resolveAsset(base + "/Worlds/World.dat", rp) &&
        !resolveAsset("Worlds/World.dat", rp)) {
        printf("WORLD_RESULT ok=0 stage=find\n");
        return 1;
    }
    WorldData w;
    if (!loadWorldDat(rp, w) || w.blocks.empty()) {
        printf("WORLD_RESULT ok=0 stage=parse\n");
        return 1;
    }
    size_t nsec = 0;
    std::map<std::string, unsigned> texUse;
    float mnx = 1e30f, mny = 1e30f, mnz = 1e30f;
    float mxx = -1e30f, mxy = -1e30f, mxz = -1e30f;
    for (size_t i = 0; i < w.blocks.size(); i++) {
        const WorldBlock& b = w.blocks[i];
        nsec += b.sections.size();
        for (size_t j = 0; j < b.sections.size(); j++)
            texUse[b.sections[j].tex0]++;
        if (b.cx - b.hx < mnx) mnx = b.cx - b.hx;
        if (b.cy - b.hy < mny) mny = b.cy - b.hy;
        if (b.cz - b.hz < mnz) mnz = b.cz - b.hz;
        if (b.cx + b.hx > mxx) mxx = b.cx + b.hx;
        if (b.cy + b.hy > mxy) mxy = b.cy + b.hy;
        if (b.cz + b.hz > mxz) mxz = b.cz + b.hz;
    }
    printf("WORLD: blocks=%u sections=%u tris=%u\n", (unsigned)w.blocks.size(),
           (unsigned)nsec, w.totalTris);
    printf("WORLD: bbox=(%.0f,%.0f,%.0f)-(%.0f,%.0f,%.0f)\n", mnx, mny, mnz,
           mxx, mxy, mxz);
    {
        const float spots[8][2] = {{120, -140}, {80, -100}, {160, -60},
                                   {200, -120}, {190, -40},  {120, -100},
                                   {100, -120}, {140, -80}};
        for (int i = 0; i < 8; i++)
            printf("WORLD: ground(%.0f,%.0f)=%.0f\n", spots[i][0],
                   spots[i][1], groundHeight(w, spots[i][0], spots[i][1]));
        // Diagnostico de cobertura del query en un punto.
        {
            const float x = 120, z = -140;
            unsigned nb = 0, ns = 0;
            size_t nt = 0;
            for (size_t bi = 0; bi < w.blocks.size(); bi++) {
                const WorldBlock& b = w.blocks[bi];
                if (x < b.cx - b.hx || x > b.cx + b.hx || z < b.cz - b.hz ||
                    z > b.cz + b.hz)
                    continue;
                nb++;
                ns += (unsigned)b.sections.size();
                for (size_t si = 0; si < b.sections.size(); si++)
                    nt += b.sections[si].triCount;
            }
            printf("WORLD: dbg blocks=%u sections=%u tris=%u\n", nb, ns,
                   (unsigned)nt);
        }
    }
    {
        unsigned shown = 0;
        for (std::map<std::string, unsigned>::iterator it = texUse.begin();
             it != texUse.end() && shown < 12; ++it, ++shown)
            printf("WORLD: tex '%s' x%u\n", it->first.c_str(), it->second);
    }
    VulkanRenderer vk;
    if (!vk.InitHeadless(800, 600)) {
        printf("WORLD_RESULT ok=0 stage=vkinit\n");
        return 1;
    }
    WorldTexCache tc;
    const Vec3 bboxCtr{(mnx + mxx) / 2, (mny + mxy) / 2, (mnz + mxz) / 2};
    float size = mxx - mnx;
    if (mxy - mny > size) size = mxy - mny;
    if (mxz - mnz > size) size = mxz - mnz;
    Vec3 pos{bboxCtr.x, bboxCtr.y + size * 0.45f,
             bboxCtr.z - size * 1.05f};
    Vec3 tgt = bboxCtr;
    // Override para inspeccionar puntos concretos:
    // SEAL_CAM="px,py,pz,tx,ty,tz".
    if (const char* env = getenv("SEAL_CAM")) {
        float v[6] = {0, 0, 0, 0, 0, 0};
        if (sscanf(env, "%f,%f,%f,%f,%f,%f", &v[0], &v[1], &v[2], &v[3],
                   &v[4], &v[5]) == 6) {
            pos = Vec3{v[0], v[1], v[2]};
            tgt = Vec3{v[3], v[4], v[5]};
        }
    }
    printf("WORLD: cam=(%.0f,%.0f,%.0f) tgt=(%.0f,%.0f,%.0f)\n", pos.x, pos.y,
           pos.z, tgt.x, tgt.y, tgt.z);
    float VP[16];
    lookAt4(pos, tgt, 90.0f, 800.0f / 600.0f, 1.0f, size * 8.0f + 500.0f, VP);
    vk.SetViewProj(VP);
    vk.Clear(0xFF87A0C0);
    unsigned secDrawn = 0, secSkip = 0;
    const size_t tris = drawWorldMesh(vk, w, tc, secDrawn, secSkip);
    printf("WORLD: drawn tris=%u secDrawn=%u secSkip=%u texOk=%u texMiss=%u "
           "sprites=%u\n",
           (unsigned)tris, secDrawn, secSkip, tc.hits, tc.miss, tc.sprites);
    if (!vk.SnapshotPPM(ppm.c_str())) {
        printf("WORLD_RESULT ok=0 stage=snap\n");
        return 1;
    }
    FILE* fp = fopen(ppm.c_str(), "rb");
    bool ok = false;
    if (fp) {
        char magic[3] = {0, 0, 0};
        unsigned sw = 0, sh = 0, mx = 0;
        if (fscanf(fp, "%2s %u %u %u", magic, &sw, &sh, &mx) == 4) {
            fgetc(fp);
            std::vector<uint8_t> px((size_t)sw * sh * 3);
            if (fread(px.data(), 1, px.size(), fp) == px.size()) {
                size_t nonClear = 0;
                for (size_t i = 0; i < (size_t)sw * sh; i++) {
                    const unsigned R = px[i * 3], G = px[i * 3 + 1],
                                       B = px[i * 3 + 2];
                    if (R < 0x7F || R > 0x8F || G < 0x98 || G > 0xA8 ||
                        B < 0xB8 || B > 0xC8)
                        nonClear++;
                }
                ok = tris > 500 && nonClear > (size_t)sw * sh / 33;
                printf("WORLD: nonClear=%u\n", (unsigned)nonClear);
            }
        }
        fclose(fp);
    }
    printf("WORLD_RESULT ok=%d tris=%u\n", ok ? 1 : 0, (unsigned)tris);
    return ok ? 0 : 1;
}

// Instancia de modelo: rota yaw (+pitch optativo) + traslada verts cocidos.
void drawModelInstance(VulkanRenderer& vk, const ModelMesh& mesh, uint32_t tex,
                       float px, float py, float pz, float yaw,
                       float pitch = 0.0f) {
    if (!tex || mesh.verts.empty() || mesh.idx.empty()) return;
    const float c = cosf(yaw), s = sinf(yaw);
    const float cp = cosf(pitch), sp = sinf(pitch);
    VkMeshVert v[3];
    for (size_t t = 0; t + 2 < mesh.idx.size(); t += 3) {
        for (int k = 0; k < 3; k++) {
            const ModelVert& sv = mesh.verts[mesh.idx[t + (size_t)k]];
            const float x1 = sv.x * c + sv.z * s;
            const float z1 = -sv.x * s + sv.z * c;
            v[k].x = x1 + px;
            v[k].y = sv.y * cp - z1 * sp + py;
            v[k].z = sv.y * sp + z1 * cp + pz;
            v[k].u = sv.u;
            v[k].v = sv.v; // igual que el mundo: pass-through directo
            v[k].r = v[k].g = v[k].b = v[k].a = 1.0f;
        }
        vk.PushTri3D(tex, v);
    }
}

// R4 melee: flanco de ataque (cmd 15 = click/espacio) + cooldown 0.6s.
// Dispara el CheckForHit() REAL del CPlayerSrvr (raycast 35 +
// OBJ_MID_DAMAGE 5). Con force=true el headless auto-ataque lo dispara.
void pollAttack(bool force) {
    if (!ServerWorld::instance().clientEntered) return;
    LPBASECLASS pp = serverPlayerObj();
    if (!pp) return;
    // R4: el server no recibe la rot del cliente (net caída); se espeja el
    // yaw publicado para que el raycast del melee mire a donde el jugador.
    {
        const PlayerPub& rp = playerPub();
        if (rp.valid) {
            for (std::vector<std::unique_ptr<ServerObj>>::iterator it =
                     ServerWorld::instance().objs.begin();
                 it != ServerWorld::instance().objs.end(); ++it) {
                if (it->get() && (*it)->obj == pp) {
                    LTRotation rr(rp.pitch, rp.yaw, 0.0f);
                    S_SetObjectRotation(ServerWorld::toH(it->get()), &rr);
                    break;
                }
            }
        }
    }
    static bool prev = false;
    static float lastSwing = -10.0f;
    const bool now = cmdOn(15);
    const bool edge = force || (now && !prev);
    prev = now;
    if (!edge) return;
    const float t = ServerWorld::instance().time;
    if (!force && t - lastSwing < 0.6f) return;
    lastSwing = t;
    static_cast<CPlayerSrvr*>(pp)->CheckForHit();
    printf("[host] swing t=%.2f\n", t);
    fflush(stdout);
}

// R4 score: lee score/money del CPlayerSrvr (KILLSCORE ya lo actualizo)
// y lo empuja a la GUI del cliente + detecta victoria (sin AIVolume0: el
// conteo es del host).
void pushScore(CLTClientShell* shell) {
    if (!shell || !ServerWorld::instance().clientEntered) return;
    LPBASECLASS pp = serverPlayerObj();
    if (!pp) return;
    CPlayerSrvr* ps = static_cast<CPlayerSrvr*>(pp);
    const unsigned s = ps->GetScore();
    const float m = ps->GetMoney();
    static unsigned lastS = 0;
    static float lastM = -1.0f;
    static bool won = false;
    if (s != lastS || m != lastM) {
        lastS = s;
        lastM = m;
        shell->SetHudStats(s, m);
        printf("[host] score=%u money=%.2f\n", s, m);
        fflush(stdout);
    }
    if (!won && s > 0) {
        unsigned alive = 0;
        for (std::vector<std::unique_ptr<ServerObj>>::iterator it =
                 ServerWorld::instance().objs.begin();
             it != ServerWorld::instance().objs.end(); ++it) {
            const ServerObj* o = it->get();
            if (!o || !o->active || !o->obj) continue;
            if (o->name.compare(0, 4, "Seal") != 0) continue;
            const Seal* seal = static_cast<const Seal*>(o->obj);
            if (!seal->IsDead()) alive++;
        }
        if (alive == 0) {
            won = true;
            printf("[host] VICTORY: all seals hunted! score=%u money=%.2f\n",
                   s, m);
            g_pLTClient->CPrint("VICTORY! All seals hunted. Score %u  $%.2f",
                                s, m);
            fflush(stdout);
        }
    }
}

// Modelos en vivo: focas del server + snowman + jugador + mazo.
void drawModelsFrame(VulkanRenderer& vk) {
    if (!ServerWorld::instance().clientEntered) return;
    static bool tried = false;
    static ModelFile sealM, snowM, guardM, malM;
    static bool haveSeal = false, haveSnow = false, haveBruno = false,
                haveMal = false;
    static WorldTexCache mtc;
    static uint32_t tSeal = 0, tSnow = 0, tBruno = 0, tMal = 0;
    static uint32_t tSeal1 = 0, tSnow1 = 0, tBruno1 = 0, tMal1 = 0;
    if (!tried) {
        tried = true;
        std::string base = ClientTuned::rezDir();
        if (base.empty()) base = "demo-sealhunter/sealhunter/rez";
        struct Def {
            const char* file;
            const char* skin0;
            const char* skin1; // segundo slot o NULL
            ModelFile* m;
            bool* have;
            uint32_t* tex0;
            uint32_t* tex1;
        };
        Def defs[4] = {
            {"seal", "ModelTextures/seal.dtx", nullptr, &sealM, &haveSeal,
             &tSeal, &tSeal1},
            {"SnowMan", "ModelTextures/snowMan.dtx", nullptr, &snowM,
             &haveSnow, &tSnow, &tSnow1},
            // El jugador del demo es HARMGuard (CreatePlayer): cuerpo +
            // ojos con dos skins.
            {"HARMGuard", "ModelTextures/HARMPurple.dtx",
             "ModelTextures/HARMHeadW1.dtx", &guardM, &haveBruno, &tBruno,
             &tBruno1},
            {"Mallet", "ModelTextures/Mallet.dtx", nullptr, &malM, &haveMal,
             &tMal, &tMal1},
        };
        for (int i = 0; i < 4; i++) {
            std::string rp;
            if (!resolveAsset(base + "/Models/" + defs[i].file + ".ltb", rp) ||
                !loadLTB(rp, *defs[i].m) || defs[i].m->meshes.empty()) {
                printf("[model] live %s FAILED\n", defs[i].file);
                continue;
            }
            *defs[i].tex0 = resolveWorldTex(defs[i].skin0, vk, mtc);
            *defs[i].tex1 =
                defs[i].skin1 ? resolveWorldTex(defs[i].skin1, vk, mtc) : 0;
            if (!*defs[i].tex0) {
                printf("[model] live %s skin MISSING\n", defs[i].file);
                continue;
            }
            *defs[i].have = true;
            size_t tr = 0;
            for (size_t j = 0; j < defs[i].m->meshes.size(); j++)
                tr += defs[i].m->meshes[j].idx.size() / 3;
            printf("[model] live %s tris=%u\n", defs[i].file, (unsigned)tr);
        }
        modelsLiveFlag() = haveSeal && haveBruno;
        fflush(stdout);
    }
    const float t = ServerWorld::instance().time;
    if (haveSeal) {
        int si = 0;
        for (std::vector<std::unique_ptr<ServerObj>>::iterator it =
                 ServerWorld::instance().objs.begin();
             it != ServerWorld::instance().objs.end(); ++it) {
            const ServerObj* o = it->get();
            if (!o || !o->active || !o->obj) continue;
            if (o->name.compare(0, 4, "Seal") != 0) continue;
            // R4: las muertas no se dibujan (Die() las oculta en el engine).
            const Seal* seal = static_cast<const Seal*>(o->obj);
            if (seal && seal->IsDead()) {
                si++;
                continue;
            }
            const float pitch = sinf(t * 5.0f + (float)si * 2.1f) * 0.3f;
            for (size_t j = 0; j < sealM.meshes.size(); j++)
                drawModelInstance(vk, sealM.meshes[j], tSeal, o->pos.x,
                                  o->pos.y, o->pos.z, (float)si * 2.1f,
                                  pitch);
            si++;
        }
    }
    if (haveSnow) {
        // Junto al campo de juego (el Snowman0 disenado esta a 1km; este
        // compone la escena como la captura original).
        for (size_t j = 0; j < snowM.meshes.size(); j++)
            drawModelInstance(vk, snowM.meshes[j], tSnow, -640.0f, -515.0f,
                              415.0f, 0.8f);
    }
    // Diagnostico una sola vez: suelo bajo focas/jugador/snowman.
    {
        static bool diag = false;
        if (!diag && worldReady()) {
            diag = true;
            const WorldData& w = worldStore();
            for (std::vector<std::unique_ptr<ServerObj>>::iterator it =
                     ServerWorld::instance().objs.begin();
                 it != ServerWorld::instance().objs.end(); ++it) {
                const ServerObj* o = it->get();
                if (!o || !o->active) continue;
                printf("[model] %s at (%.0f,%.0f,%.0f) ground=%.0f\n",
                       o->name.c_str(), o->pos.x, o->pos.y, o->pos.z,
                       groundHeight(w, o->pos.x, o->pos.z));
            }
            const PlayerPub& ppd = playerPub();
            if (ppd.valid)
                printf("[model] player at (%.0f,%.0f,%.0f) ground=%.0f\n",
                       ppd.x, ppd.y, ppd.z,
                       groundHeight(worldStore(), ppd.x, ppd.z));
            printf("[model] snowman ground=%.0f\n",
                   groundHeight(worldStore(), 150.0f, -120.0f));
            fflush(stdout);
        }
    }
    const PlayerPub& pp = playerPub();
    if (pp.valid) {
        if (haveBruno) {
            for (size_t j = 0; j < guardM.meshes.size(); j++) {
                const uint32_t tx =
                    guardM.meshes[j].texSlot == 1 && tBruno1 ? tBruno1
                                                             : tBruno;
                drawModelInstance(vk, guardM.meshes[j], tx, pp.x, pp.y, pp.z,
                                  pp.yaw);
            }
        }
        if (haveMal) {
            const float fx = sinf(pp.yaw), fz = cosf(pp.yaw);
            const float rx = fz, rz = -fx;
            const float hx = pp.x + rx * 20.0f + fx * 18.0f;
            const float hy = pp.y + 32.0f;
            const float hz = pp.z + rz * 20.0f + fz * 18.0f;
            for (size_t j = 0; j < malM.meshes.size(); j++)
                drawModelInstance(vk, malM.meshes[j], tMal, hx, hy, hz,
                                  pp.yaw, 0.35f);
        }
    }
}

int runModelTest(const std::string& ppm) {
    std::string base = ClientTuned::rezDir();
    if (base.empty()) base = "demo-sealhunter/sealhunter/rez";
    const char* names[6] = {"seal", "bruno", "SnowMan", "Mallet", "HARMGuard",
                             "playerbase"};
    const char* skins[6] = {"ModelTextures/seal.dtx",
                            "ModelTextures/Bruno.dtx",
                            "ModelTextures/snowMan.dtx",
                            "ModelTextures/Mallet.dtx",
                            "ModelTextures/HARMPurple.dtx",
                            "ModelTextures/HARMHeadW1.dtx"};
    const float posX[6] = {-250.0f, -150.0f, -50.0f, 50.0f, 150.0f, 250.0f};
    VulkanRenderer vk;
    if (!vk.InitHeadless(800, 600)) {
        printf("MODEL_RESULT ok=0 stage=vkinit\n");
        return 1;
    }
    WorldTexCache tc;
    size_t totalTris = 0;
    bool allOk = true;
    const Vec3 campos{0, 90, -290}, camtgt{0, 40, 0};
    float VP[16];
    lookAt4(campos, camtgt, 90.0f, 800.0f / 600.0f, 1.0f, 3000.0f, VP);
    vk.SetViewProj(VP);
    vk.Clear(0xFF203038);
    for (int mi = 0; mi < 6; mi++) {
        std::string rp;
        std::string cand = base + "/Models/" + names[mi] + ".ltb";
        if (!resolveAsset(cand, rp)) {
            printf("MODEL: %s NOT FOUND\n", names[mi]);
            allOk = false;
            continue;
        }
        ModelFile mf;
        if (!loadLTB(rp, mf)) {
            printf("MODEL: %s parse FAILED\n", names[mi]);
            allOk = false;
            continue;
        }
        if (mf.meshes.empty())
            printf("MODEL: %s sin geometria propia (ok)\n", names[mi]);
        size_t mtris = 0, mverts = 0;
        for (size_t i = 0; i < mf.meshes.size(); i++) {
            mtris += mf.meshes[i].idx.size() / 3;
            mverts += mf.meshes[i].verts.size();
        }
        printf("MODEL: %s meshes=%u verts=%u tris=%u nodes=%u "
               "children=%u slots=",
               names[mi], (unsigned)mf.meshes.size(), (unsigned)mverts,
               (unsigned)mtris, (unsigned)mf.nodes.size(),
               (unsigned)mf.childFiles.size());
        for (size_t i = 0; i < mf.meshes.size(); i++)
            printf("%d ", mf.meshes[i].texSlot);
        printf("\n");
        const uint32_t tex = resolveWorldTex(skins[mi], vk, tc);
        if (!tex) {
            printf("MODEL: %s skin MISSING\n", names[mi]);
            allOk = false;
            continue;
        }
        for (size_t i = 0; i < mf.meshes.size(); i++) {
            drawModelInstance(vk, mf.meshes[i], tex, posX[mi], 0, 0, 0.4f);
            totalTris += mf.meshes[i].idx.size() / 3;
        }
    }
    printf("MODEL: totalTris=%u texOk=%u texMiss=%u\n", (unsigned)totalTris,
           tc.hits, tc.miss);
    if (!vk.SnapshotPPM(ppm.c_str())) {
        printf("MODEL_RESULT ok=0 stage=snap\n");
        return 1;
    }
    const bool ok = allOk && totalTris > 500;
    printf("MODEL_RESULT ok=%d tris=%u\n", ok ? 1 : 0, (unsigned)totalTris);
    return ok ? 0 : 1;
}
} // namespace Host

int main(int argc, char* argv[]) {
    int frames = 60;
    bool framesSet = false;
    bool vulkan = false;
    bool windowMode = false;
    bool psurface = false;
    bool forceX11 = false;
    bool meshTest = false;
    bool worldTest = false;
    bool modelTest = false;
    bool autoattack = false;
    bool autowalk = false;
    std::string ppm = "/tmp/sealhunter_vk.ppm";
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a.rfind("--frames=", 0) == 0) {
            frames = atoi(a.c_str() + 9);
            framesSet = true;
        }
        if (a == "--vulkan") vulkan = true;
        if (a == "--window") windowMode = true;
        if (a == "--psurface") psurface = true;
        if (a == "--x11") forceX11 = true;
        if (a == "--mesh-test") meshTest = true;
        if (a == "--world-test") worldTest = true;
        if (a == "--model-test") modelTest = true;
        if (a == "--autoattack") autoattack = true;
        if (a == "--autowalk") autowalk = true;
        if (a.rfind("--ppm=", 0) == 0) ppm = a.substr(6);
    }
    if (meshTest) return Host::runMeshTest(ppm);
    if (worldTest) return Host::runWorldTest(ppm);
    if (modelTest) return Host::runModelTest(ppm);
    // Window mode without --frames runs until the window is closed.
    if (windowMode && !framesSet) frames = 0;
#ifndef HAS_SDL3
    (void)forceX11; // only meaningful with the SDL3 backend compiled in
#endif
    VulkanRenderer vk;
#ifdef _LINUX
    Display* xDpy = nullptr;
    Window xWin = 0;
    Atom xWmDelete = None;
#ifdef HAS_SDL3
    Sdl3Win* sdlWin = nullptr;
#endif
    if (psurface) {
        if (vk.InitHeadlessPresent(800, 600) != S_OK) {
            printf("PSURFACE_RESULT ok=0 stage=vkinit frames=0\n");
            return 1;
        }
        Host::VkBridge::renderer() = &vk;
        Host::VkBridge::width() = 800;
        Host::VkBridge::height() = 600;
    } else if (windowMode) {
#ifdef HAS_SDL3
        if (!forceX11) {
            // Prefer SDL3 (real x11/wayland backends); native handles feed
            // the existing xlib-surface Vulkan init below.
            void* sdlDpy = nullptr;
            unsigned long sdlXWin = 0;
            sdlWin = Sdl3_Create("Sealhunter - LithTech Jupiter (Vulkan)",
                                 800, 600, &sdlDpy, &sdlXWin);
            if (sdlWin) {
                xDpy = (Display*)sdlDpy;
                xWin = (Window)sdlXWin;
            }
        }
        if (!sdlWin) {
#endif
        xDpy = XOpenDisplay(nullptr);
        if (!xDpy) {
            const char* dd = getenv("DISPLAY");
            printf("WINDOW_RESULT ok=0 stage=xdisplay frames=0 display=%s\n",
                   dd ? dd : "(unset)");
            return 1;
        }
        int scr = DefaultScreen(xDpy);
        xWin = XCreateSimpleWindow(xDpy, RootWindow(xDpy, scr), 0, 0, 800, 600,
                                  0, BlackPixel(xDpy, scr), BlackPixel(xDpy, scr));
        if (!xWin) {
            printf("WINDOW_RESULT ok=0 stage=xwindow frames=0\n");
            XCloseDisplay(xDpy);
            return 1;
        }
        XStoreName(xDpy, xWin, "Sealhunter - LithTech Jupiter (Vulkan)");
        xWmDelete = XInternAtom(xDpy, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(xDpy, xWin, &xWmDelete, 1);
        XSelectInput(xDpy, xWin, ExposureMask | KeyPressMask | KeyReleaseMask |
                   ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                   StructureNotifyMask);
        XMapWindow(xDpy, xWin);
        // Ask the WM for keyboard focus: without this, key events go to
        // whatever window was focused and the menu looks dead.
        XWMHints* wmHints = XAllocWMHints();
        if (wmHints) {
            wmHints->flags = InputHint;
            wmHints->input = True;
            XSetWMHints(xDpy, xWin, wmHints);
            XFree(wmHints);
        }
        // NOTE: no XSetInputFocus here: the window is not viewable yet and
        // the server kills us with BadMatch. Focus is taken on MapNotify
        // and on ButtonPress below.
        XFlush(xDpy);
        printf("WINDOW: id=0x%lx\n", (unsigned long)xWin);
#ifdef HAS_SDL3
        } // if (!sdlWin): native X11 creation
#endif
        if (vk.InitNative(xDpy, (unsigned long)xWin, 800, 600) != S_OK) {
            printf("WINDOW_RESULT ok=0 stage=vkinit frames=0\n");
#ifdef HAS_SDL3
            if (sdlWin) { Sdl3_Destroy(sdlWin); }
            else
#endif
            { XDestroyWindow(xDpy, xWin); XCloseDisplay(xDpy); }
            return 1;
        }
        Host::VkBridge::renderer() = &vk;
        Host::VkBridge::width() = 800;
        Host::VkBridge::height() = 600;
    } else if (vulkan) {
#else
    SDL_Window* sdlWin = nullptr;
    if (psurface) {
        if (vk.InitHeadlessPresent(800, 600) != S_OK) {
            printf("PSURFACE_RESULT ok=0 stage=vkinit frames=0\n");
            return 1;
        }
        Host::VkBridge::renderer() = &vk;
        Host::VkBridge::width() = 800;
        Host::VkBridge::height() = 600;
    } else if (windowMode) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            printf("WINDOW_RESULT ok=0 stage=sdl frames=0 err=%s\n", SDL_GetError());
            return 1;
        }
        sdlWin = SDL_CreateWindow("Sealhunter - LithTech Jupiter (Vulkan)",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  800, 600, SDL_WINDOW_SHOWN
#ifdef _WIN32
                                  /* sin SDL_WINDOW_VULKAN: superficie nativa Win32 abajo */
#else
                                  | SDL_WINDOW_VULKAN
#endif
                                  );
        if (!sdlWin) {
            printf("WINDOW_RESULT ok=0 stage=swindow frames=0 err=%s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
#ifdef _WIN32
        {
            void* hwnd = SdlHwnd(sdlWin);
            if (!hwnd || vk.InitNativeWin32(hwnd, 800, 600) != S_OK) {
                printf("WINDOW_RESULT ok=0 stage=vkinit frames=0\n");
                SDL_DestroyWindow(sdlWin);
                SDL_Quit();
                return 1;
            }
        }
#else
        if (vk.Init(sdlWin) != S_OK) {
            printf("WINDOW_RESULT ok=0 stage=vkinit frames=0\n");
            SDL_DestroyWindow(sdlWin);
            SDL_Quit();
            return 1;
        }
#endif
        Host::VkBridge::renderer() = &vk;
        Host::VkBridge::width() = 800;
        Host::VkBridge::height() = 600;
    } else if (vulkan) {
#endif
        if (!vk.InitHeadless(800, 600)) {
            printf("HOST_RESULT ok=0 stage=vkinit frames=0\n");
            return 1;
        }
        Host::VkBridge::renderer() = &vk;
    }

    static Host::ClientTuned client;
    // run.bat style: lithtech -rez engine.rez -rez ..\rez — usable -rez dir wins
    std::string rez = Host::ParseRezArgs(argc, argv);
    if (rez.empty()) rez = "demo-sealhunter/sealhunter/rez";
    Host::ClientTuned::rezDir() = rez;
    printf("HOST: rezdir=%s\n", rez.c_str());
    static Host::DrawPrimTuned drawprim;
    static Host::CommonTuned common;
    static Host::PhysicsTuned physics;
    static HostSoundMgr sound;
    static Host::FontTuned font;
    static Host::TexTuned tex;
    static HostModelClient model;
    static HostWidgetManager widget;

    HostClient_Fill(&client);
    client.apply(&client);

    g_pLTClient = &client;
    g_pLTCDrawPrim = &drawprim;
    g_pLTCCommon = &common;
    g_pLTCPhysics = &physics;
    g_pLTCSoundMgr = &sound;
    g_pLTCFontManager = &font;
    g_pLTCTexInterface = &tex;
    g_pLTCModel = &model;
    g_pLTCWidgetManager = &widget;

    // ---- dedicated in-process server (Fase B): interfaces, shell, seals ----
    Host::bindServerInterfaces();
    static CLTServerShell serverShell;
    Host::setServerShell(&serverShell);
    if (serverShell.OnServerInitialized() != LT_OK) {
        printf("HOST_RESULT ok=0 stage=srvinit frames=0\n");
        return 1;
    }
    serverShell.PreStartWorld(false);
    {
        // Area de juego: repisa de nieve del GameStartPoint disenado
        // (GameStartPoint0 (-752,575,33) y Snowman0 del World.lta).
        std::map<std::string, std::string> sealProps;
        sealProps["Filename"] = "";
        sealProps["Texture"] = "";
        sealProps["RenderStyle"] = "";
        sealProps["SealValue"] = "10.0";
        LTVector p;
        p.Init(-670.0f, -512.0f, 400.0f);
        Host::spawnServerObject("Seal", p, "Seal0", sealProps);
        p.Init(-600.0f, -528.0f, 420.0f);
        Host::spawnServerObject("Seal", p, "Seal1", sealProps);
        p.Init(-750.0f, -528.0f, 430.0f);
        Host::spawnServerObject("Seal", p, "Seal2", sealProps);
    }
    printf("SERVER: initialized objects=%u\n", Host::serverObjectCount());

    CLTClientShell* shell = new CLTClientShell();

    RMode mode;
    memset(&mode, 0, sizeof(mode));
    mode.m_Width = 800;
    mode.m_Height = 600;
    mode.m_BitDepth = 32;
    LTGUID guid;
    memset(&guid, 0, sizeof(guid));

    LTRESULT r = shell->OnEngineInitialized(&mode, &guid);
    printf("HOST: OnEngineInitialized -> %d shutdown=%d\n",
           (int)r, (int)Host::stats().shutdownRequested);
    if (r != LT_OK || Host::stats().shutdownRequested) {
        printf("HOST_RESULT ok=0 stage=init frames=0\n");
        return 1;
    }

    // Interactive window (unbounded frames) boots to the menu in
    // LOCAL_GAMEMODE_NONE, exactly like run.bat: OnCommandOn only forwards
    // keys to the menu in NONE mode, so auto-starting a game here would eat
    // all menu input. Bounded/probe runs keep the old auto-start path.
    bool menuBoot = windowMode && !psurface && frames <= 0;
    if (!menuBoot) {
        r = shell->StartNormalGame();
        printf("HOST: StartNormalGame -> %d shutdown=%d\n",
               (int)r, (int)Host::stats().shutdownRequested);
        if (r != LT_OK || Host::stats().shutdownRequested) {
            printf("HOST_RESULT ok=0 stage=start frames=0\n");
            return 1;
        }
        shell->OnEnterWorld();
        shell->EnterGameUI();
        Host::ServerOnClientEnterWorld(&serverShell);
        Host::consumeWorldStart();
    } else {
        printf("HOST: menu boot (StartNormalGame deferred to menu choice)\n");
    }
    if (psurface) {
        int presents = 0;
#ifdef _LINUX
        struct timespec pt0, pt1;
        clock_gettime(CLOCK_MONOTONIC, &pt0);
#else
        Uint64 pfreq = SDL_GetPerformanceFrequency();
        Uint64 pt0 = SDL_GetPerformanceCounter();
#endif
        for (int i = 0; i < frames; i++) {
            if (Host::consumeWorldStart()) {
                shell->OnEnterWorld();
                Host::ServerOnClientEnterWorld(&serverShell);
                printf("HOST: entered world\n");
            }
            shell->Update();
            Host::tickServerWorld(1.0f / 60.0f);
        Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::pollAttack(false);
            Host::pushScore(shell);
            if (Host::stats().shutdownRequested) break;
            if (vk.RenderWindowFrame() != S_OK) {
                printf("PSURFACE_RESULT ok=0 stage=present frames=%d\n", presents);
                shell->OnExitWorld();
                Host::ServerOnClientExitWorld(&serverShell);
                return 1;
            }
            presents++;
        }
#ifdef _LINUX
        clock_gettime(CLOCK_MONOTONIC, &pt1);
        double pel = (double)(pt1.tv_sec - pt0.tv_sec) +
                     (double)(pt1.tv_nsec - pt0.tv_nsec) / 1e9;
#else
        Uint64 pt1 = SDL_GetPerformanceCounter();
        double pel = (double)(pt1 - pt0) / (double)pfreq;
#endif
        if (pel <= 0) pel = 1e-9;
        Host::Stats& ps = Host::stats();
        bool pok = presents > 0 && !ps.shutdownRequested;
        printf("PSURFACE: frames=%d draws=%d presents=%d fps=%.1f\n",
               presents, ps.drawPrimCalls, presents, presents / pel);
        printf("PSURFACE_RESULT ok=%d frames=%d draws=%d presents=%d\n",
               pok ? 1 : 0, presents, ps.drawPrimCalls, presents);
        shell->OnExitWorld();
        Host::ServerOnClientExitWorld(&serverShell);
        return pok ? 0 : 1;
    }
#ifdef _LINUX
#ifdef HAS_SDL3
    if (sdlWin) {
        // SDL3 event loop (runs after shell creation; mirrors X11 loop).
        bool quit = false;
        int presents = 0;
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        while (!quit && (frames <= 0 || presents < frames)) {
            int a = 0, b = 0, ev = S3_NONE;
            while (!quit && (ev = Sdl3_Poll(sdlWin, &a, &b)) != S3_NONE) {
                if (ev == S3_QUIT) { quit = true; continue; }
                if (ev == S3_KEYDOWN && !b) {
                    if (a == SDLK_q) { quit = true; continue; }
                    int vkc, cmd;
                    mapSDLKey(a, vkc, cmd);
                    printf("HOST: key sym=0x%x vk=%d cmd=%d\n",
                           (unsigned)a, vkc, cmd);
                    if (vkc >= 0) {
                        shell->OnKeyDown(vkc, 0);
                        if (Host::noteKey(vkc, cmd, true) && cmd >= 0)
                            shell->OnCommandOn(cmd);
                    }
                } else if (ev == S3_KEYUP) {
                    int vkc, cmd;
                    mapSDLKey(a, vkc, cmd);
                    if (vkc >= 0) {
                        shell->OnKeyUp(vkc);
                        Host::noteKey(vkc, cmd, false);
                    }
                } else if (ev == S3_MOUSEDOWN) {
                    shell->OnKeyDown(Host::HVK_LBUTTON, 0);
                    if (Host::noteKey(Host::HVK_LBUTTON, 15, true))
                        shell->OnCommandOn(15);
                } else if (ev == S3_MOUSEUP) {
                    shell->OnKeyUp(Host::HVK_LBUTTON);
                    Host::noteKey(Host::HVK_LBUTTON, 15, false);
                } else if (ev == S3_MOTION) {
                    Host::addAxes((float)a * 0.01f, (float)b * 0.01f, 0);
                }
            }
            if (Host::consumeWorldStart()) {
                shell->OnEnterWorld();
                Host::ServerOnClientEnterWorld(&serverShell);
                printf("HOST: entered world\n");
            }
            shell->Update();
            Host::tickServerWorld(1.0f / 60.0f);
        Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::pollAttack(false);
            Host::pushScore(shell);
            if (Host::stats().shutdownRequested) break;
            if (vk.RenderWindowFrame() != S_OK) {
                printf("WINDOW_RESULT ok=0 stage=present frames=%d\n",
                       presents);
                shell->OnExitWorld();
                Host::ServerOnClientExitWorld(&serverShell);
                Sdl3_Destroy(sdlWin);
                return 1;
            }
            presents++;
            if (presents % 60 == 0) printf("WINDOW: frame %d\n", presents);
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double el = (double)(t1.tv_sec - t0.tv_sec) +
                    (double)(t1.tv_nsec - t0.tv_nsec) / 1e9;
        if (el <= 0) el = 1e-9;
        double fps = presents / el;
        Host::Stats& ws = Host::stats();
        bool wok = presents > 0 && !ws.shutdownRequested;
        printf("WINDOW: frames=%d draws=%d uirenders=%d objects=%d fps=%.1f\n",
               presents, ws.drawPrimCalls, ws.uiRenders, ws.objectsCreated,
               fps);
        printf("WINDOW_RESULT ok=%d frames=%d draws=%d presents=%d fps=%.1f\n",
               wok ? 1 : 0, presents, ws.drawPrimCalls, presents, fps);
        printf("SERVER: objects=%u updateticks=%u time=%.2f\n",
               Host::serverObjectCount(), Host::ServerWorld::instance().totalTicks,
               Host::ServerWorld::instance().time);
        shell->OnExitWorld();
        Host::ServerOnClientExitWorld(&serverShell);
        Sdl3_Destroy(sdlWin);
        return wok ? 0 : 1;
    }
#endif
    if (windowMode) {
        XEvent e;
        bool quit = false;
        int presents = 0;
        int lastMx = 0, lastMy = 0;
        bool haveMouse = false;
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        while (!quit && (frames <= 0 || presents < frames)) {
            while (XPending(xDpy)) {
                XNextEvent(xDpy, &e);
                if (e.type == ClientMessage &&
                    (Atom)e.xclient.data.l[0] == xWmDelete) {
                    quit = true;
                } else if (e.type == MapNotify) {
                    XSetInputFocus(xDpy, xWin, RevertToParent, CurrentTime);
                } else if (e.type == KeyPress) {
                    int idx = (e.xkey.state & ShiftMask) ? 1 : 0;
                    KeySym k = XLookupKeysym(&e.xkey, idx);
                    if (k == XK_q) { quit = true; continue; }
                    int vk, cmd;
                    Host::mapKeysym((unsigned long)k, vk, cmd);
                    printf("HOST: key sym=0x%lx vk=%d cmd=%d\n",
                           (unsigned long)k, vk, cmd);
                    if (vk >= 0) {
                        shell->OnKeyDown(vk, 0);
                        if (Host::noteKey(vk, cmd, true) && cmd >= 0)
                            shell->OnCommandOn(cmd);
                    }
                } else if (e.type == KeyRelease) {
                    KeySym k = XLookupKeysym(&e.xkey, 0);
                    int vk, cmd;
                    Host::mapKeysym((unsigned long)k, vk, cmd);
                    if (vk >= 0) {
                        shell->OnKeyUp(vk);
                        Host::noteKey(vk, cmd, false);
                    }
                } else if (e.type == ButtonPress &&
                           e.xbutton.button == Button1) {
                    XSetInputFocus(xDpy, xWin, RevertToParent, CurrentTime);
                    shell->OnKeyDown(Host::HVK_LBUTTON, 0);
                    if (Host::noteKey(Host::HVK_LBUTTON, 15, true))
                        shell->OnCommandOn(15); // Shoot (autoexec Button0)
                } else if (e.type == ButtonRelease &&
                           e.xbutton.button == Button1) {
                    shell->OnKeyUp(Host::HVK_LBUTTON);
                    Host::noteKey(Host::HVK_LBUTTON, 15, false);
                } else if (e.type == MotionNotify) {
                    int mx = e.xmotion.x, my = e.xmotion.y;
                    if (haveMouse)
                        Host::addAxes((float)(mx - lastMx) * 0.01f,
                                      (float)(my - lastMy) * 0.01f, 0);
                    lastMx = mx; lastMy = my; haveMouse = true;
                }
            }
            if (Host::consumeWorldStart()) {
                shell->OnEnterWorld();
                Host::ServerOnClientEnterWorld(&serverShell);
                printf("HOST: entered world\n");
            }
            shell->Update();
            Host::tickServerWorld(1.0f / 60.0f);
        Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::pollAttack(false);
            Host::pushScore(shell);
            if (Host::stats().shutdownRequested) break;
            if (vk.RenderWindowFrame() != S_OK) {
                printf("WINDOW_RESULT ok=0 stage=present frames=%d\n", presents);
                shell->OnExitWorld();
                Host::ServerOnClientExitWorld(&serverShell);
                XDestroyWindow(xDpy, xWin);
                XCloseDisplay(xDpy);
                return 1;
            }
            presents++;
            if (presents % 60 == 0) printf("WINDOW: frame %d\n", presents);
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double el = (double)(t1.tv_sec - t0.tv_sec) +
                    (double)(t1.tv_nsec - t0.tv_nsec) / 1e9;
        if (el <= 0) el = 1e-9;
        double fps = presents / el;
        Host::Stats& ws = Host::stats();
        bool wok = presents > 0 && !ws.shutdownRequested;
        printf("WINDOW: frames=%d draws=%d uirenders=%d objects=%d fps=%.1f\n",
               presents, ws.drawPrimCalls, ws.uiRenders, ws.objectsCreated, fps);
        printf("WINDOW_RESULT ok=%d frames=%d draws=%d presents=%d fps=%.1f\n",
               wok ? 1 : 0, presents, ws.drawPrimCalls, presents, fps);
        printf("SERVER: objects=%u updateticks=%u time=%.2f\n",
               Host::serverObjectCount(), Host::ServerWorld::instance().totalTicks,
               Host::ServerWorld::instance().time);
        shell->OnExitWorld();
        Host::ServerOnClientExitWorld(&serverShell);
        XDestroyWindow(xDpy, xWin);
        XCloseDisplay(xDpy);
        return wok ? 0 : 1;
    }
#else
    if (windowMode) {
        SDL_Event e;
        bool quit = false;
        int presents = 0;
        Uint64 freq = SDL_GetPerformanceFrequency();
        Uint64 t0 = SDL_GetPerformanceCounter();
        while (!quit && (frames <= 0 || presents < frames)) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) { quit = true; continue; }
                if (e.type == SDL_KEYDOWN && !e.key.repeat) {
                    int vk, cmd;
                    mapSDLKey((int)e.key.keysym.sym, vk, cmd);
                    if (vk >= 0) {
                        shell->OnKeyDown(vk, 0);
                        if (Host::noteKey(vk, cmd, true) && cmd >= 0)
                            shell->OnCommandOn(cmd);
                    }
                } else if (e.type == SDL_KEYUP) {
                    int vk, cmd;
                    mapSDLKey((int)e.key.keysym.sym, vk, cmd);
                    if (vk >= 0) {
                        shell->OnKeyUp(vk);
                        Host::noteKey(vk, cmd, false);
                    }
                } else if (e.type == SDL_MOUSEBUTTONDOWN &&
                           e.button.button == SDL_BUTTON_LEFT) {
                    shell->OnKeyDown(Host::HVK_LBUTTON, 0);
                    if (Host::noteKey(Host::HVK_LBUTTON, 15, true))
                        shell->OnCommandOn(15);
                } else if (e.type == SDL_MOUSEBUTTONUP &&
                           e.button.button == SDL_BUTTON_LEFT) {
                    shell->OnKeyUp(Host::HVK_LBUTTON);
                    Host::noteKey(Host::HVK_LBUTTON, 15, false);
                } else if (e.type == SDL_MOUSEMOTION) {
                    Host::addAxes((float)e.motion.xrel * 0.01f,
                                  (float)e.motion.yrel * 0.01f, 0);
                }
            }
            if (Host::consumeWorldStart()) {
                shell->OnEnterWorld();
                Host::ServerOnClientEnterWorld(&serverShell);
                printf("HOST: entered world\n");
            }
            shell->Update();
            Host::tickServerWorld(1.0f / 60.0f);
        Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::drawModelsFrame(vk);
            Host::pollAttack(false);
        Host::pushScore(shell);
            Host::pollAttack(false);
            Host::pushScore(shell);
            if (Host::stats().shutdownRequested) break;
            if (vk.RenderWindowFrame() != S_OK) {
                printf("WINDOW_RESULT ok=0 stage=present frames=%d\n", presents);
                shell->OnExitWorld();
                Host::ServerOnClientExitWorld(&serverShell);
                SDL_DestroyWindow(sdlWin);
                SDL_Quit();
                return 1;
            }
            presents++;
            if (presents % 60 == 0) printf("WINDOW: frame %d\n", presents);
        }
        Uint64 t1 = SDL_GetPerformanceCounter();
        double fps = presents / ((double)(t1 - t0) / (double)freq);
        Host::Stats& ws = Host::stats();
        bool wok = presents > 0 && !ws.shutdownRequested;
        printf("WINDOW: frames=%d draws=%d uirenders=%d objects=%d fps=%.1f\n",
               presents, ws.drawPrimCalls, ws.uiRenders, ws.objectsCreated, fps);
        printf("WINDOW_RESULT ok=%d frames=%d draws=%d presents=%d fps=%.1f\n",
               wok ? 1 : 0, presents, ws.drawPrimCalls, presents, fps);
        printf("SERVER: objects=%u updateticks=%u time=%.2f\n",
               Host::serverObjectCount(), Host::ServerWorld::instance().totalTicks,
               Host::ServerWorld::instance().time);
        shell->OnExitWorld();
        Host::ServerOnClientExitWorld(&serverShell);
        SDL_DestroyWindow(sdlWin);
        SDL_Quit();
        return wok ? 0 : 1;
    }
#endif
    for (int i = 0; i < frames; i++) {
        if (Host::consumeWorldStart()) {
            shell->OnEnterWorld();
            Host::ServerOnClientEnterWorld(&serverShell);
            printf("HOST: entered world\n");
        }
        // R4-verificacion: caminar adelante sin input real.
        if (autowalk) Host::inputState().cmds.insert(1);
        if (autowalk && (i % 120 == 0)) {
            const Host::PlayerPub& wpp = Host::playerPub();
            printf("[host] walk f=%d pos=(%.0f,%.0f,%.0f)\n", i, wpp.x, wpp.y,
                   wpp.z);
            fflush(stdout);
        }
        shell->Update();
        Host::tickServerWorld(1.0f / 60.0f);
        Host::drawWorldFrame(vk);
        Host::drawModelsFrame(vk);
        Host::pollAttack(autoattack && (i % 45 == 0) && (i > 0));
        Host::pushScore(shell);
        if (Host::stats().shutdownRequested) break;
    }
    shell->OnExitWorld();
    Host::ServerOnClientExitWorld(&serverShell);

    Host::Stats& s = Host::stats();
    size_t vkTris = 0;
    bool vkOk = true;
    if (vulkan) {
        vkTris = vk.PendingTris() + vk.PendingTexQuads() * 2;
        vkOk = vk.SnapshotPPM(ppm.c_str());
        printf("HOST: vulkan tris=%d uirenders=%d snapshot=%s\n",
               (int)vkTris, s.uiRenders, vkOk ? ppm.c_str() : "FAILED");
    }
    printf("HOST: frames=%d draws=%d uirenders=%d objects=%d cprint=%d\n",
           s.frames, s.drawPrimCalls, s.uiRenders,
           s.objectsCreated, s.cprintLines);
    bool ok = s.frames > 0 && !s.shutdownRequested;
    if (vulkan) ok = ok && vkTris > 0 && vkOk;
    printf("HOST_RESULT ok=%d stage=run frames=%d draws=%d vktris=%d\n",
           ok ? 1 : 0, s.frames, s.drawPrimCalls, (int)vkTris);
    printf("SERVER: objects=%u updateticks=%u time=%.2f\n",
           Host::serverObjectCount(), Host::ServerWorld::instance().totalTicks,
           Host::ServerWorld::instance().time);
    return ok ? 0 : 1;
}
