// SDL3 windowing for sealhunter, isolated translation unit.
//
// SDL2 and SDL3 headers cannot coexist in one .cpp (both declare SDL_*
// names), and the Vulkan renderer already pulls SDL2 via vk_renderer.h.
// So this is the ONLY file that includes <SDL3/SDL.h>: it exposes a tiny
// SDL-free interface used by host_sealhunter.cpp.
//
// Keycodes cross the boundary as plain ints; SDL2/SDL3 keycode values
// match for the mapped keys, so the host maps them with SDLK_* names.
#pragma once

struct Sdl3Win; // opaque SDL3 window handle

// Event kinds returned by Sdl3_Poll.
enum {
    S3_NONE = 0,
    S3_QUIT = 1,
    S3_KEYDOWN = 2, // a = keycode, b = repeat (0/1)
    S3_KEYUP = 3,   // a = keycode
    S3_MOUSEDOWN = 4,
    S3_MOUSEUP = 5,
    S3_MOTION = 6 // a = xrel, b = yrel
};

// Create an SDL3 window (x11 driver) and fetch its native Display/Window
// for the existing xlib-surface Vulkan init. Returns null when SDL3 is
// unusable; the caller then falls back to native X11.
Sdl3Win* Sdl3_Create(const char* title, int w, int h, void** outDisplay,
                     unsigned long* outWindow);
void Sdl3_Destroy(Sdl3Win* win);
// Poll one event; 0 = queue empty.
int Sdl3_Poll(Sdl3Win* win, int* a, int* b);
