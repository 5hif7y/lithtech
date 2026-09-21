# 02 — Port MSVC de la demo sealhunter

Entorno: MSVC 19.50 (VS18), CMake 4.3.2, vcpkg en
`C:\Users\Shifty\DEV\vcpkg` (SDL2 2.32.10, freetype; toolchain file de vcpkg
obligatorio por `find_package(SDL2 REQUIRED)`), VulkanSDK 1.4.357.0,
`build-msvc/` (ignorado por git).

## Fixes de configure

- `CMakeLists.txt` (root): en WIN32 prefiere `find_package(Vulkan)` real
  (vulkan-1.lib) antes que el hack de headers vendoreados; `NOMINMAX` en
  MSVC; `PLATFORM_WINDOWS`; rutas absolutas `LITH_REAL_WINDOWS_H` (um) y
  `LITH_REAL_CRTDBG_H` (ucrt) para los dispatchers.
- `Renderer/vulkan/CMakeLists.txt`: `VK_USE_PLATFORM_WIN32_KHR` en Windows
  (XLIB rompía `vulkan.h` en MSVC).

## Patrón dispatcher (sombras en include path)

`Platform/` va primero en `-I`, así que `Platform/windows.h` y
`Platform/crtdbg.h` sombreaban a los headers reales y el `#include`
recursivo se anulaba por `#pragma once`. Solución: dispatcher que en
`_WIN32` incluye el header real por ruta absoluta (macro del CMake) y en
Linux usa el shim (`windows_linux.h`, contenido original intacto).

## Fixes de compilación

- `sdk/inc/ltinteger.h`: `#define __int64` solo fuera de Windows
  (redefinía un keyword y rompía `<cstdint>` e `intrin.h`).
- `sdk/inc/physics/math_phys.h`: `rdtsc()` con `__rdtsc()` en x64/ARM64,
  asm solo x86.
- `clientfx/Shared/BaseFx.h`: `bool` faltante en `operator==`.
- `Platform/host/host_engine.h`: 9 stubs `T_*` con firmas distintas al SDK
  real (eran warnings con `-fpermissive`); definiciones `::CUIPolyString`
  movidas fuera de `namespace Host` (C2888).
- `host_sealhunter.cpp`: fallbacks `ILTPhysics::` (11 virtuales no-puros;
  MSVC emite vtables base y exige definición); `SDL_MAIN_HANDLED` en el
  target (SDL renombraba `main`).
- `sealhunter/CMakeLists.txt`: `SDL_MAIN_HANDLED` en WIN32.

## Modo ventana (nativo Win32)

El SDL2 de vcpkg **no trae backend Vulkan** (`SDL_Vulkan_GetInstanceExtensions`
→ "Vulkan is not loaded"; `SDL_Vulkan_*` crashea). Reparación sin tocar SDL:
ventana plain + `SDL_GetWindowWMInfo` → HWND →
`VulkanRenderer::InitNativeWin32` (`vkCreateWin32SurfaceKHR`, espejo del path
X11). Verificado: `WINDOW_RESULT ok=1 frames=60 presents=60 fps=137` (RX 580).
Alternativa futura: `vcpkg install sdl2[vulkan]` (no necesaria).

## Verificación

- Configure limpio (todas las demos), link limpio, `HOST_RESULT ok=1`,
  `WINDOW_RESULT ok=1`, exit 0. Sin regresiones headless.
