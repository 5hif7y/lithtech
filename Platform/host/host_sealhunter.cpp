// Real headless bootstrap for the sealhunter demo (networking).
// Same host backend as drawprim/bump; drives StartNormalGame() (the
// headless equivalent of typing "normal" at the console). Exit 0 only
// with frames>0 and no shutdown. Prints HOST_RESULT line.
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

    r = shell->StartNormalGame();
    printf("HOST: StartNormalGame -> %d shutdown=%d\n",
           (int)r, (int)Host::stats().shutdownRequested);
    if (r != LT_OK || Host::stats().shutdownRequested) {
        printf("HOST_RESULT ok=0 stage=start frames=0\n");
        return 1;
    }

    shell->OnEnterWorld();
    for (int i = 0; i < frames; i++) {
        shell->Update();
        if (Host::stats().shutdownRequested) break;
    }
    shell->OnExitWorld();

    Host::Stats& s = Host::stats();
    printf("HOST: frames=%d draws=%d objects=%d cprint=%d\n",
           s.frames, s.drawPrimCalls, s.objectsCreated, s.cprintLines);
    bool ok = s.frames > 0 && !s.shutdownRequested;
    printf("HOST_RESULT ok=%d stage=run frames=%d draws=%d\n",
           ok ? 1 : 0, s.frames, s.drawPrimCalls);
    return ok ? 0 : 1;
}
