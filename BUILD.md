# LithTech Jupiter - Build Linux (Arch + gcc/g++ + Vulkan)

## Estado actual (verificado 2026-09-11, tooling ya funciona)

- **Binarios Windows borrados**: 407 archivos (`*.exe/*.dll/*.lib/*.pdb/*.obj` etc) eliminados via `clean_windows_binaries.sh` + `find ... -delete`. Quedan 0 en repo (6 restantes son `distlib/*.exe` dentro de `.venv`).
- **Git init**: `git init` ejecutado en `nolf2_gpl_src/` (branch master, `.gitignore` ya creado). Sin commit inicial (requiere autorización explícita - no hacer `git commit` sin pedir).
- **Indexación graphify**: `graphify-output/nolf2-index.json` generado (fallback Python porque `graphify` no existe en PyPI/cargo/npm). **7888 archivos**, 7215 nodos, 20657 edges. Ver `third_party/vulkan` y `.venv` para detalles de fallback.
  - `Engine:4357 Game:2008 Libs:145 Samples:1266` (ver JSON `stats.by_category`)
  - Top hubs: `GlobalServerMgr.cpp` (125 includes), `PhotoshopSDK.h` (68) etc.
- **CMake + gcc/g++ + Vulkan**: `cmake -B build --toolchain cmake/toolchain-arch.cmake` **configura OK** (GNU 16.1.1, CMake 4.4, SDL2 2.32.70, Vulkan vendoreado 1.4.357, glslang 16.3). `cmake --build build -j$(nproc)` **compila OK**: `liblith_engine.a` (7.3K stub) + `librenderer_vulkan.a` (184K).

## Reproducir

```bash
cd /home/shifty/lithtech-jupiter/Lithtech-Jupiter-System-Enterprise-Edition-Build-69/nolf2_gpl_src
# 1. Limpiar (ya hecho, idempotente)
./clean_windows_binaries.sh   # -> 0 archivos tras segunda corrida

# 2. Git (ya hecho)
git status                    # untracked: Engine/ Game/ Renderer/ Platform/ etc
# git add . && git commit -m "chore: clean windows binaries, add cmake+linux+vulkan scaffolding"  # solo con autorización

# 3. Graphify (fallback Python, no requiere cargo/npm)
.venv/bin/python /tmp/graphify_index.py  # regenera graphify-output/nolf2-index.json (2.1M)
# o: ./run_graphify.sh  # intenta cargo/npm/pip, cae a fallback

# 4. Build Arch gcc/g++
cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON
cmake --build build -j$(nproc)
ls -lh build/Engine/liblith_engine.a build/Renderer/vulkan/librenderer_vulkan.a
```

Sin toolchain explicito también funciona (usa gcc system):
```bash
cmake -B build -DLITH_VULKAN=ON && cmake --build build -j$(nproc)
```

## Que falta para compilar TODO el engine (no solo stub)

El `Engine` completo son **1193 cpp** con 89 `windows.h`, 10 `__asm`, 49 `DirectX/d3d9` hard-deps (MFC, MAX70SDK, PhotoshopSDK). El `Engine/CMakeLists.txt` actual usa **stub** (`LITH_ENGINE_FULL=OFF` por defecto) para que el toolchain se verifique sin portar todo. Para port incremental:

```bash
cmake -B build -DLITH_ENGINE_FULL=ON --toolchain cmake/toolchain-arch.cmake
cmake --build build   # mostrará errores tipo bdefs.h/mfcstub.h/directx - arreglar uno a uno
```

Pasos:
1. Ampliar `Platform/windows.h` y `Platform/platform.h` (ya mapea DWORD/HRESULT/__stdcall/stricmp) con GUID/CRITICAL_SECTION etc.
2. Reemplazar `#include <windows.h>` -> `#include "Platform/platform.h"` en ~200 archivos (`sed -i 's/<windows.h>/"Platform\/platform.h"/'`)
3. Portear `__asm` en `Math/` a `<immintrin.h>` SSE2 (`-msse2` ya puesto)
4. Completar `Renderer/vulkan/vk_renderer.cpp` => swapchain real, renderpass, pipeline, VMA, SPIR-V (`Renderer/vulkan/shaders/*.vert/*.frag` -> `glslangValidator -V`)
5. Añadir includes faltantes `bdefs.h`/`ltmodule.h` (están en `runtime/shared/src` y `sdk/inc`) como `target_include_directories` cuando `LITH_ENGINE_FULL=ON`

## Toolchain gcc/g++ (Arch)

- `cmake/toolchain-arch.cmake`: `gcc`/`g++`, `-march=native -pipe`, `FIND_ROOT BOTH` (no `ONLY`, permitía fallo de `FindVulkan`)
- Flags: `-Wall -Wextra -Wno-multichar -Wno-write-strings -fpermissive -fno-strict-aliasing -fvisibility=hidden -msse2`
- Defines: `_LINUX _RETAIL NOMINMAX STRICT _HAS_EXCEPTIONS=0 PLATFORM_LINUX ENGINE_LINUX RENDERER_VULKAN VK_USE_PLATFORM_XLIB_KHR`
- Vulkan: vendoreado en `third_party/vulkan/include` (Khronos Vulkan-Headers main, incluye `vk_video/`) porque `vulkan-headers` no está instalado sin `sudo` en este container. Si tienes `pacman -S vulkan-headers`, CMake usará system.

## Graphify

`graphify` binario no existe en PyPI (`pip install graphify` -> 404), `cargo` no está, `npm` no está. Se usa fallback Python `.venv/bin/python /tmp/graphify_index.py` que genera el mismo JSON (`stats`, `top_hubs`, `nodes`, `edges`). Ver `run_graphify.sh` para variantes CLI intentadas.
