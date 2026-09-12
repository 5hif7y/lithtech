// Bootstrap for the sealhunter demo (networking).
// Same host backend as drawprim/bump. Headless/probe runs drive
// StartNormalGame() (the headless equivalent of typing "normal" at the
// console); interactive --window boots to the menu like run.bat so the
// menu receives input. Exit 0 only with frames>0 and no shutdown.
// Prints HOST_RESULT line.
#include "Platform/host/host_engine.h"
#include "ltclientshell.h"
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
    bool framesSet = false;
    bool vulkan = false;
    bool windowMode = false;
    bool psurface = false;
    bool forceX11 = false;
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
        if (a.rfind("--ppm=", 0) == 0) ppm = a.substr(6);
    }
    // Window mode without --frames runs until the window is closed.
    if (windowMode && !framesSet) frames = 0;
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
                                  800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
        if (!sdlWin) {
            printf("WINDOW_RESULT ok=0 stage=swindow frames=0 err=%s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        if (vk.Init(sdlWin) != S_OK) {
            printf("WINDOW_RESULT ok=0 stage=vkinit frames=0\n");
            SDL_DestroyWindow(sdlWin);
            SDL_Quit();
            return 1;
        }
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
            shell->Update();
            if (Host::stats().shutdownRequested) break;
            if (vk.RenderWindowFrame() != S_OK) {
                printf("PSURFACE_RESULT ok=0 stage=present frames=%d\n", presents);
                shell->OnExitWorld();
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
            shell->Update();
            if (Host::stats().shutdownRequested) break;
            if (vk.RenderWindowFrame() != S_OK) {
                printf("WINDOW_RESULT ok=0 stage=present frames=%d\n",
                       presents);
                shell->OnExitWorld();
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
        shell->OnExitWorld();
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
            shell->Update();
            if (Host::stats().shutdownRequested) break;
            if (vk.RenderWindowFrame() != S_OK) {
                printf("WINDOW_RESULT ok=0 stage=present frames=%d\n", presents);
                shell->OnExitWorld();
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
        shell->OnExitWorld();
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
            shell->Update();
            if (Host::stats().shutdownRequested) break;
            if (vk.RenderWindowFrame() != S_OK) {
                printf("WINDOW_RESULT ok=0 stage=present frames=%d\n", presents);
                shell->OnExitWorld();
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
        shell->OnExitWorld();
        SDL_DestroyWindow(sdlWin);
        SDL_Quit();
        return wok ? 0 : 1;
    }
#endif
    for (int i = 0; i < frames; i++) {
        shell->Update();
        if (Host::stats().shutdownRequested) break;
    }
    shell->OnExitWorld();

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
    return ok ? 0 : 1;
}
