// SDL3 windowing backend (see host_sdl3.h). ONLY TU including SDL3.
#include "Platform/host/host_sdl3.h"
#include <SDL3/SDL.h>
#include <cstdio>

namespace {
struct Sdl3WinImpl {
    SDL_Window* win;
};
} // namespace

Sdl3Win* Sdl3_Create(const char* title, int w, int h, void** outDisplay,
                     unsigned long* outWindow) {
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("WINDOW: sdl3 no disponible (%s), fallback nativo\n",
               SDL_GetError());
        return nullptr;
    }
    SDL_Window* win = SDL_CreateWindow(title, w, h, 0);
    if (!win) {
        printf("WINDOW: sdl3 no disponible (%s), fallback nativo\n",
               SDL_GetError());
        SDL_Quit();
        return nullptr;
    }
    SDL_PropertiesID props = SDL_GetWindowProperties(win);
    void* dpy = SDL_GetPointerProperty(props,
                                       SDL_PROP_WINDOW_X11_DISPLAY_POINTER,
                                       NULL);
    unsigned long xwin = (unsigned long)SDL_GetNumberProperty(
        props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    if (!dpy || !xwin) {
        printf("WINDOW: sdl3 sin ventana X11 (driver=%s), fallback nativo\n",
               SDL_GetCurrentVideoDriver());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return nullptr;
    }
    printf("WINDOW: sdl3 id=0x%lx driver=%s\n", xwin,
           SDL_GetCurrentVideoDriver());
    *outDisplay = dpy;
    *outWindow = xwin;
    Sdl3WinImpl* impl = new Sdl3WinImpl();
    impl->win = win;
    return reinterpret_cast<Sdl3Win*>(impl);
}

void Sdl3_Destroy(Sdl3Win* win) {
    if (!win) return;
    Sdl3WinImpl* impl = reinterpret_cast<Sdl3WinImpl*>(win);
    SDL_DestroyWindow(impl->win);
    delete impl;
    SDL_Quit();
}

int Sdl3_Poll(Sdl3Win* win, int* a, int* b) {
    if (!win) return S3_NONE;
    *a = 0;
    *b = 0;
    SDL_Event e;
    if (!SDL_PollEvent(&e)) return S3_NONE;
    switch (e.type) {
    case SDL_EVENT_QUIT:
        return S3_QUIT;
    case SDL_EVENT_KEY_DOWN:
        *a = (int)e.key.key;
        *b = e.key.repeat ? 1 : 0;
        return S3_KEYDOWN;
    case SDL_EVENT_KEY_UP:
        *a = (int)e.key.key;
        return S3_KEYUP;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (e.button.button == SDL_BUTTON_LEFT) return S3_MOUSEDOWN;
        return S3_NONE;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (e.button.button == SDL_BUTTON_LEFT) return S3_MOUSEUP;
        return S3_NONE;
    case SDL_EVENT_MOUSE_MOTION:
        *a = (int)e.motion.xrel;
        *b = (int)e.motion.yrel;
        return S3_MOTION;
    default:
        return S3_NONE;
    }
}
