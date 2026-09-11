# Plan: Hacer compilables todas las piezas de LithTech Jupiter (Linux gcc/g++)

## Goal
Hacer que **cada librería y cada demo** de `nolf2_gpl_src` sea compilable con `gcc 16.1.1 / g++ 16.1.1` en Arch Linux, desacoplando librerías viejas (1998-2003) por versiones modernas donde el usuario lo autoriza. El proyecto principal ya compila (`liblith_engine.a` stub + `librenderer_vulkan.a`); este plan lleva eso a **todas** las piezas.

Usuario pidió: *siempre usar graphify para ahorrar tokens (regla de sesión), hacer commit del trabajo actual, auditar en profundidad y planear reparación compilable; desacoplar librerías viejas está permitido*.

## Success Criteria
- `cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON && cmake --build build -j` sigue compilando el proyecto principal (no regresión).
- Cada una de las **17 librerías** (`Libs/Libs/*` + `Engine/Engine/libs/*`) tiene un `CMakeLists.txt` y compila aislada con `cmake --build build --target <lib>` o al menos `g++ -c` con `-IPlatform` sin errores `windows.h`/`__int64`/`iostream.h`.
- Cada una de las **16+ demos** (`Samples/Samples/<cat>/<demo>`) compila al menos `cshell`+`sshell` con el mismo toolchain (Vulkan/SDL2 en vez de DirectX/MFC).
- `graphify-output/nolf2-index.json` regenerado post-fix muestra `windows_tokens` → 0 en libs portables; `cmake --build` no emite warnings `__declspec(x)=` dropped (ya fix).
- Docs en `Muse-Docs/` actualizados con nueva auditoría; `git log` muestra commits por fase.

## Context And Current Facts
**Toolchain verificado (exec 76-95):** `gcc 16.1.1`, `cmake 4.4.0`, `SDL2 2.32.70 (sdl2-compat)`, `glslang 16.3`, `Vulkan Khronos 1.4.357` vendoreado en `third_party/vulkan` (porque `vulkan-headers` requiere sudo, ya vendoreado), `zlib 1.3.2`, `openal 1.25.2`, `libpng`, `libjpeg-turbo` instalados (`pacman -Qs`).

**Graphify (ahorro tokens, regla de sesión en `session-rules.md`):** `fallback Python .venv/bin/python /tmp/graphify_index.py` → `graphify-output/nolf2-index.json` 2.03 MB, **7888 files, 7215 nodos, 20657 edges, by_category Engine:4357 Game:2008 Libs:145 Samples:1266**, `by_ext .h:4491 .cpp:2599`. Índice ya genera `windows_tokens` por nodo. Uso en esta auditoría: `json.load(index)` + `filter` en vez de leer 7888 archivos → **9.4M → 0.56M (94%)** triage vulns y **9.7M → 25k (99.7%)** incremental libs/demos. Regeneración: `.venv/bin/python /tmp/graphify_index.py`.

**Estado actual build (exec 95):** `cmake --build build -j` OK → `liblith_engine.a 7.3K` (stub `LITH_ENGINE_FULL=OFF`) + `librenderer_vulkan.a 185K` (VulkanRenderer con `SDL_Vulkan_CreateSurface`). `Engine` completo son 1193 cpp con 89 `windows.h`, 10 `__asm`, 49 `DirectX` — por eso stub.

**Auditoría previa (exec 79-85, `Muse-Docs/06-Auditoria-Librerias-y-Demos.md`):**
- **Libs 16 testeadas con `g++ -D_LINUX -IPlatform` todas FAIL:** `ButeMgr FAIL_WINDOWS Stdafx.h:1`, `CryptMgr FAIL_HEADER iostream.h:7`, `MFCStub FAIL_WINDOWS mfcs_point:4`, `RegMgr FAIL_HEADER`, `stdlith FAIL __int64`, `zlib NO_CPP`, `ltmem FAIL ltheap:3`, `rezmgr FAIL Platform/security.h` (con `-I.` sí resuelve, fallo real es `Stdafx.h`/`iostream.h`), etc. `windows_tokens` por lib: `Engine/externalesd 34`, `Lib_DShow 13`, `zlib 8` peores; `ButeMgr 1`, `CryptMgr 0`, `rezmgr 0` mejores.
- **Demos 5 representativas FAIL:** `graphics/drawprim` `ltinteger.h __int64→int64` (parcheado a `_LINUX`/`__LINUX`+`cstdint`, luego `ltmatrix.h ltsinf` falta), `graphics/shaders d3d9.h` (stub `Platform/d3d9.h` creado), `nettest WorldProperties.h`, etc.
- **Fixes ya aplicados (commits 9677131, 093f45a):** `Platform/security.h` (LITH_STRCPY, lith_make_buffer RAII, lith_memcpy_checked), `butemgr.cpp` leak RAII, `rezmgr.cpp` `lith_safe_copy`, `ltinteger.h` dual `_LINUX`/`__LINUX`, `Platform/d3d9.h` stub, `Platform/windows.h` GUID/CRITICAL_SECTION.

**Repo git:** 4 commits (`587071e` scaffolding, `9677131` security, `a937c42` docs, `093f45a` audit). `.gitignore` ignora `build/`, `graphify-output/`, `.venv/`, `*.spv`, pero `graphify-output/nolf2-index.json` y `third_party/vulkan` se trackean con `git add -f`. `Development/`, `Engine/Engine/runtime/`, `Engine/Engine/sdk/` etc siguen untracked (intencional, no bloat hasta port incremental) — `git status --porcelain` muestra ~40 entradas untracked.

**Dependencias obsoletas vía graphify paths (exec 99):** `MFC 18 files`, `DirectX/DShow 267`, `RealMedia 71`, `zlib-old 23`, `MAX/Photoshop SDK 881`, `Portable 78`. Modernas disponibles: `zlib 1.3.2`, `sdl2-compat`, `openal 1.25`, `freetype2 2.14`, `vulkan-headers 1.4.357` (inspeccionado `pacman -Si` + `https://zlib.net/` + `https://www.libsdl.org/` + `https://github.com/KhronosGroup/Vulkan-Headers`).

## Constraints And Non-goals
- **Regla sesión:** siempre consultar `graphify-output/nolf2-index.json` antes de leer 7888 archivos; regenerar solo tras cambios grandes.
- **No romper proyecto principal:** cualquier decoupling debe mantener `cmake --build build` verde; `Engine` stub sigue default `LITH_ENGINE_FULL=OFF` hasta que incremental lo reemplace.
- **Autorizado:** desacoplar librerías viejas por modernas (MFC→SDL2, zlib-old→system zlib, DirectShow/RealMedia→OpenAL/FFmpeg, dibmgr→libpng, StackTracer→backward-cpp, MAX/Photoshop SDK→Tools/legacy). No se requiere preservar binarios 1999.
- **No-goals:** no portar `Development/Development/TO2` assets (`.rez/.bmp`), no recompilar `MFC70/71 Runtime` binarios, no hacer `git push` sin pedir, no re-introducir `windows.h` en código nuevo.

## Key Decisions
### 1. Desacoplar vs parchear
- **Recomendado:** desacoplar 6 libs viejas, mantener 6 portables, reescribir 2 shims. Rechazado: parchear todo in-place (coste 881 files MAX SDK sin uso en Linux, `RealMedia` 1999 sin codecs). Evidencia: `graphify` `881 MAX/Photoshop SDK` vs `0 wt` en `ButeMgr/CryptMgr` → priorizar portables.

### 2. Reemplazos modernos
| Vieja | Moderna | Por qué | Fuente |
|---|---|---|---|
| `Libs/Libs/zlib` 0.95 (23 files, wt8) | `find_package(ZLIB)` system 1.3.2 | Audit 7ASecurity fixes (feb 2026) | [zlib.net](https://zlib.net/) verificado 1.3.2 + `pacman -Si zlib` 1:1.3.2-3 |
| `MFCStub`/`MFC70` + `RegMgr` Win32 | `sdl2-compat` + `std::filesystem`/`std::string` | SDL soporta Linux/Win/Mac, zlib license | [libsdl.org](https://www.libsdl.org/) SDL 3.4.16 + `pacman -Qs sdl2-compat` INSTALLED |
| `Lib_DShow` (DirectShow 63 files, wt13) | `OpenAL 1.25` + `FFmpeg` (no en repo, `pacman -Si openal` ok) | DirectShow no existe Linux | `pacman -Si openal` 1.25.2 + `libsdl.org` audio |
| `externalesd/real` (71 files, wt34) | Eliminar, usar `FFmpeg` si se necesita Real codec | SDK 1999 obsoleto | `graphify` 71 files, no usado en build principal |
| `dibmgr` (libpng viejo) | `libpng` + `SDL2_image` system | `pacman -Qs libpng` INSTALLED | `pacman` |
| `StackTracer` (dbghelp) | `backward-cpp` o `std::stacktrace` C++23 | `wt3` windows-only | `graphify` wt3 |
| `MAX70SDK/PhotoshopSDK` (881) | Mover a `Tools/legacy/` y no compilar en Linux | Solo exporters 3ds Max 7 | `graphify` 881 |

Rechazado: mantener `zlib-old` (vuln CVE), mantener `MFC` (no portable).

### 3. Estrategia Engine
- **Stub primero, incremental después:** mantener `Engine/CMakeLists.txt` con `option(LITH_ENGINE_FULL OFF)` → fase final lo pone `ON` y filtra `d3d_*/dx*/win_*` + `clientfx/tools/libs` como ya hace. Evita romper `lith_engine` mientras libs portables se validan.

### 4. Demos
- **Recomendado:** cada demo `Samples/Samples/<cat>/<demo>` obtiene su `CMakeLists.txt` que `find_package(SDL2)` + `target_link_libraries(lith_engine renderer_vulkan)`, linka `Vulkan::Vulkan` no `d3d9`. Rechazado: parchear cada `.vcproj` (220 .vcproj en `graphify` `by_ext .vcproj:220`).

## Recommended Approach
En 5 fases, cada fase usa `graphify` para seleccionar siguiente trabajo (consultar `windows_tokens`, `by_category`, `top_hubs`) sin re-leer 7888 archivos. Cada fase añade un `CMakeLists.txt` por lib/demo y `find_package` moderno.

## Work Plan
**Fase 0 — Commit y regla (hoy, sin código)**
- Commit trabajo actual si hay `git diff --stat` (hoy solo `.agents/plans` nuevo, `ltinteger.h` ya commiteado) → `git add .agents/plans/2026-09-11-lithtech-compilable-plan.md` + `session-rules.md` si no está trackeado. Validación: `git log --oneline -4` muestra plan.
- Regla `session-rules.md` ya escrita (`personal_project`) → todos los siguientes pasos hacen `json.load(graphify-output/nolf2-index.json)` primero.

**Fase 1 — Headers portables (1 día)**
- `Platform/math_shim.h` (mapea `ltsinf→sinf`, `ltcosf→cosf`), `Platform/basetypes_fix.h` (centraliza `__int64`/`__int32` ya en `ltinteger.h`), completar `Platform/windows.h` GUID.
- `Engine/sdk/inc/ltinteger.h` ya fix dual `_LINUX`; verificar `ltmatrix.h` y `coordinate_frame.h` con `-fpermissive`.
- Validación: `g++ -std=c++17 -D_LINUX -IPlatform -I Engine/sdk/inc -c Samples/graphics/drawprim/cshell/src/clientinterfaces.cpp -o /tmp/test.o` pasa header.

**Fase 2 — Libs portables (2 días, 6 libs)**
- Para `ButeMgr`, `CryptMgr`, `stdlith`, `lith`, `ltmem`, `ltamgr`, `rezmgr` (graphify `wt 0-1`, `by_category Libs:145`):
  - Crear `Libs/Libs/<name>/CMakeLists.txt` con `add_library(<name> STATIC)`, `find_package(ZLIB)` para `zlib`, `target_link_libraries(lith_platform)`, `target_include_directories(BEFORE Platform)`
  - Reemplazar `Libs/Libs/zlib` por system: `find_package(ZLIB REQUIRED)` + `target_link_libraries(ZLIB::ZLIB)`, borrar vendored o dejar `EXCLUDE`
  - `MFCStub` → stub mínimo `mfc_shim.h` con `CString→std::string`, no MFC.
- Validación por lib: `cmake -B build -DLITH_<LIB>=ON && cmake --build build --target <lib> -j`

**Fase 3 — Desacoplar libs obsoletas (2 días)**
- `Engine/libs/Lib_DShow` → `add_library(Lib_DShow INTERFACE)` + `find_package(OpenAL)` o `OFF` con `message(WARNING "Lib_DShow desacoplado")`.
- `Engine/libs/externalesd` → mover a `Engine/libs/externalesd_legacy/` y `EXCLUDE` en `Engine/CMakeLists.txt` (ya filtra `*/libs/*` pero afinar).
- `Engine/libs/enginemodellib`, `Libs/zlib` → ya en Fase 2.
- `Libs/dibmgr` → `find_package(PNG)` + `SDL2_image`.
- `StackTracer` → `find_package(Backward)` o `INTERFACE` vacío.
- Validación: `graphify` `windows_tokens` en esas libs debe ser ignorado; `cmake -B build` no intenta compilarles si `LITH_ENGINE_FULL=OFF`; con `ON` deben filtrarse.

**Fase 4 — Demos (3 días, priorizar por graphify)**
- Orden por `files` y `wt` bajo: `networking/nettest` (6 files, wt1) → `graphics/drawprim` (34, wt2) → `graphics/bump` (30, wt2) → etc (740 nodes Samples). Cada demo: crear `Samples/Samples/<cat>/<demo>/CMakeLists.txt` con `add_executable`, `find_package(SDL2)`, `find_package(Vulkan)`, `target_link_libraries(lith_engine renderer_vulkan)`.
- Reemplazar `#include <d3d9.h>` → `#include "Platform/d3d9.h"` stub (ya existe) y luego渐 port a `vulkan` (`ShaderMgr.cpp` 19)
- Validación: `cmake --build build --target drawprim -j` al menos header-compila; full run requiere `Engine` no stub, pero header check basta para plan.

**Fase 5 — Engine completo y cierre (2 días)**
- `Engine/CMakeLists.txt` con `LITH_ENGINE_FULL=ON` ya filtra `d3d_*`/`clientfx`/`tools`/`built`; añadir `list(FILTER EXCLUDE REGEX ".*externalesd.*")` y `*MAX70SDK*`.
- `Platform/security.h` ya cubre `strcpy/sprintf` (662/1078) → aplicar `LITH_STRCPY` a restantes 660 via `sed` masivo `sed -i 's/strcpy(a,b)/lith_safe_copy(a,sizeof(a),b)/'`.
- Validación final: `rm -rf build && cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON -DLITH_ENGINE_FULL=ON && cmake --build build -j` debe llegar al menos 80% objetos sin `fatal error: windows.h` (ya `Platform/windows.h` cubre 148).

## Validation Plan
- **Por fase:** `cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON && cmake --build build -j` → `liblith_engine.a` + `librenderer_vulkan.a` siempre verde (no regresión).
- **Por lib:** `cmake --build build --target <lib> --verbose` + `nm -u build/Libs/<lib>/lib<lib>.a | grep -i windows` debe ser 0.
- **Por demo:** `g++ -std=c++17 -D_LINUX -IPlatform -I Engine/sdk/inc -c Samples/<demo>/cshell/src/clientinterfaces.cpp -o /tmp/test.o` debe pasar header (graphify `windows_tokens` → 0).
- **Graphify:** tras cada fase ` .venv/bin/python /tmp/graphify_index.py && .venv/bin/python /tmp/vuln_scan.py` → `windows_tokens` bajando, `strcpy` 662→~0 en libs portables.
- **Manual:** `Muse-Docs/06-Auditoria` actualizar con tabla nueva; `pacman -Qs zlib/sdl2/openal` sigue INSTALLED.

## Risks / Rollback
- **Riesgo:** `Engine` completo con `LITH_ENGINE_FULL=ON` rompe 1193 cpp (ya visto `ltsinf`, `coordinate_frame` template). Mitigación: Fase 5 al final, con `-fpermissive` + `math_shim.h`, y `EXCLUDE` para `externalesd`/`MAXSDK` (881 files).
- **Riesgo:** `zlib` system 1.3.2 API cambia vs 0.95 → `find_package(ZLIB)` puede necesitar `ZLIB::ZLIB` vs `z`. Mitigación: `target_link_libraries(ZLIB::ZLIB)` con fallback `z`.
- **Riesgo:** `RealMedia` decoupling deja sin codec para `.rez` viejos → no usado en demos modernas, aceptable (usuario autorizó).
- **Rollback:** cada fase es commit separado (`git log --oneline`); `git revert <hash>` por fase. `third_party/vulkan` vendoreado es 22M → no borrar sin `git rm`. `build/` y `.venv/` ignorados, no afectan historial.

## Open Questions
- **Ninguna bloqueante.** Dos decisiones asumidas (reversibles):
  - `OpenAL + FFmpeg` para `Lib_DShow` vs `SDL2_audio` solo → asumido OpenAL por `pacman -Qs openal` INSTALLED; si se prefiere solo SDL2, cambiar `find_package(OpenAL)` por `SDL2::SDL2` en Fase 3 sin rework mayor.
  - `backward-cpp` vs `std::stacktrace` para `StackTracer` → asumido `backward-cpp` (C++17 compatible), `std::stacktrace` es C++23 y requiere gcc 14+ (disponible 16.1.1, pero menos probado).

## Sources
- [Vulkan-Headers — KhronosGroup GitHub](https://github.com/KhronosGroup/Vulkan-Headers) — inspeccionado 2026-09-11, usado para vendorear `third_party/vulkan` y `find_package(Vulkan)` (ya en repo).
- [SDL — Simple DirectMedia Layer](https://www.libsdl.org/) — inspeccionado, SDL 3.4.16, cross-platform reemplazo MFC/Win32, usado para `sdl2-compat` + `SDL_Vulkan_CreateSurface` en `Renderer/vulkan`.
- [zlib Home Site](https://zlib.net/) — inspeccionado, zlib 1.3.2 (feb 2026) con audit 7ASecurity fixes, usado para reemplazar `Libs/Libs/zlib` 0.95.

