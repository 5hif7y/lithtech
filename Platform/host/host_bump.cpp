// Real headless bootstrap for the bump demo.
// Like drawprim, plus scripted forward input (frames 10-50) to exercise
// movement/physics for real. Exit 0 only with frames>0 and no shutdown.
// Prints machine-checkable HOST_RESULT line.
#include "Platform/host/host_engine.h"
#include "ltclientshell.h"
#include <cstdio>
#include <cstring>
#include <string>

extern ILTClient* g_pLTClient;
extern ILTDrawPrim* g_pLTCDrawPrim;
extern ILTCommon* g_pLTCCommon;
extern ILTPhysics* g_pLTCPhysics;
extern ILTClientSoundMgr* g_pLTCSoundMgr;
extern ILTFontManager* g_pLTCFontManager;
extern ILTTexInterface* g_pLTCTexInterface;
extern ILTModelClient* g_pLTCModel;
extern ILTWidgetManager* g_pLTCWidgetManager;

static bool scriptForward(int cmd) {
    int f = Host::stats().frames;
    return cmd == 1 && f >= 10 && f < 50;
}

int main(int argc, char* argv[]) {
    int frames = 60;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a.rfind("--frames=", 0) == 0) frames = atoi(a.c_str() + 9);
    }

    static Host::ClientTuned client;
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
    // scripted input replaces the default no-op IsCommandOn
    client.IsCommandOn = [](int cmd) { return scriptForward(cmd); };

    g_pLTClient = &client;
    g_pLTCDrawPrim = &drawprim;
    g_pLTCCommon = &common;
    g_pLTCPhysics = &physics;
    g_pLTCSoundMgr = &sound;
    g_pLTCFontManager = &font;
    g_pLTCTexInterface = &tex;
    g_pLTCModel = &model;
    g_pLTCWidgetManager = &widget;

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

    shell->OnEnterWorld();
    for (int i = 0; i < frames; i++) {
        shell->Update();
        if (Host::stats().shutdownRequested) break;
    }
    shell->OnExitWorld();

    Host::Stats& s = Host::stats();
    float dist = 0;
    if (!Host::objects().empty()) {
        HOBJECT h = Host::objects().begin()->first;
        Host::ObjState* o = Host::findObj(h);
        if (o) {
            float dx = o->pos.x, dy = o->pos.y, dz = o->pos.z;
            dist = sqrtf(dx * dx + dy * dy + dz * dz);
        }
    }
    printf("HOST: frames=%d moves=%d dist=%.1f objects=%d cprint=%d\n",
           s.frames, s.physMoves, dist, s.objectsCreated, s.cprintLines);
    bool ok = s.frames > 0 && s.physMoves > 0 && dist > 0
        && !s.shutdownRequested;
    printf("HOST_RESULT ok=%d stage=run frames=%d moves=%d dist=%.1f\n",
           ok ? 1 : 0, s.frames, s.physMoves, dist);
    return ok ? 0 : 1;
}
