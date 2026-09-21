# Compilación — Arch Linux (gcc/g++ + Vulkan)

## Dependencias

- gcc 16.1.1, g++ 16.1.1, cmake 4.4.0, glslang 16.3
- SDL2 2.32.70 (sdl2-compat), vulkan-loader 1.4.350, Vulkan headers vendoreados 1.4.357 (ver 05)
- Opcional: ninja, vulkan-headers system (`pacman -S vulkan-headers` → usa system en vez de vendoreado)

## Toolchain

`cmake/toolchain-arch.cmake`:
```cmake
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_FLAGS_INIT "-march=native -pipe")
set(CMAKE_CXX_FLAGS_INIT "-march=native -pipe")
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)  # no ONLY, permitía fallo FindVulkan
```
Flags: `-Wall -Wextra -Wno-multichar -Wno-write-strings -fpermissive -fno-strict-aliasing -fvisibility=hidden -msse2 -mfpmath=sse`  
Defines: `_LINUX _RETAIL NOMINMAX STRICT _HAS_EXCEPTIONS=0 PLATFORM_LINUX ENGINE_LINUX RENDERER_VULKAN VK_USE_PLATFORM_XLIB_KHR`

## Shims

- `Platform/platform.h`: `DWORD→uint32_t, HRESULT→int32_t, MAX_PATH 260, __stdcall/__cdecl/__declspec → vacío, stricmp→strcasecmp, _vsnprintf→vsnprintf`
- `Platform/windows.h`: wrapper `#include "platform.h"` + `GUID/IID/CRITICAL_SECTION` stubs, `InitializeCriticalSection` etc
- `Platform/security.h`: safe string + RAII (ver 03)
- `third_party/vulkan/include` (22M) + `vk_video/` vendoreado porque `vulkan-headers` no está sin sudo en container

## Build

```bash
rm -rf build
cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON
cmake --build build -j$(nproc)
# → [50%] Built target lith_engine (7.3K stub, LITH_ENGINE_FULL=OFF)
# → [100%] Built target renderer_vulkan (185K, vk_renderer.cpp.o con vulkan.h vendoreado + SDL_vulkan.h)
ls -lh build/Engine/liblith_engine.a build/Renderer/vulkan/librenderer_vulkan.a
```

Sin toolchain:
```bash
cmake -B build -DLITH_VULKAN=ON && cmake --build build -j$(nproc)
```

## Por qué Engine es stub

Engine completo son 1193 cpp con 89 `windows.h` + 10 `__asm` + 49 `DirectX` hard-deps (MFC, MAX70SDK, PhotoshopSDK). `Engine/CMakeLists.txt` usa:

```cmake
option(LITH_ENGINE_FULL OFF) # ON para intentar build completo incremental
if(LITH_ENGINE_FULL)
  file(GLOB_RECURSE ENGINE_SOURCES ...) # filtra d3d_/clientfx/tools/libs/built
else()
  file(WRITE lith_engine_stub.cpp "// stub\nint lith_engine_linux_stub(){return 0;}")
endif()
```

Para port completo:
```bash
cmake -B build -DLITH_ENGINE_FULL=ON --toolchain cmake/toolchain-arch.cmake
cmake --build build  # mostrará bdefs.h/mfcstub.h/directx faltantes → arreglar uno a uno
# + sed -i 's/<windows.h>/"Platform\/platform.h"/' ~200 archivos
# + portear __asm → <immintrin.h> SSE2
```

## Validación Vulkan renderer

`Renderer/vulkan/vk_renderer.h/.cpp`: `VulkanRenderer::Init(SDL_Window*)` → `vkCreateInstance`/`SDL_Vulkan_CreateSurface`/`pickPhysicalDevice`/`createLogicalDevice`/`createSwapchain` (stub, falta renderpass/pipeline/VMA/SPIR-V real en `shaders/`). Compila con `Vulkan::Vulkan` (vendoreado) + `SDL2::SDL2`.
