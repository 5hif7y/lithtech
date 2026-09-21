# Auditoría Librerías y Demos — via Graphify (ahorro tokens)
**Fecha:** 2026-09-11
**Graphify:** 7888 files, 7215 nodos, 20657 edges

## Cómo ahorró tokens graphify en esta auditoría

Esta auditoría **reusó el índice ya generado** (`graphify-output/nolf2-index.json` 2.03 MB) en vez de re-leer 7888 archivos para descubrir qué librerías/demos existen:

- **Naive si no existiera índice:** escanear 7888 archivos para encontrar `Libs/*` y `Samples/*` → 7888×1200 ≈ **9.4M tokens** (como en 02) + luego leer 16 libs×5 files + 20 demos×4 files ≈ 224k → **~9.7M total**
- **Graphify (índice ya en disco):** `json.load(index)` (~50 tokens) + `filter n["path"].startswith("Libs/")` (~36×10 tokens) + compile-test 16×1 file (~16×1500 si se quiere verificar) → **~25k tokens para esta auditoría incremental**
- **Ahorro incremental: ~9.7M → 25k (99.7% )** — y sin graphify, cada nueva auditoría (vulns, libs, demos) tendría que re-escanear todo.

## Librerías — resultado compilación (g++ -D_LINUX -IPlatform -std=c++17)

| Librería | Archivos | windows_tokens | Intenta compilar | Resultado | Causa |
|---|---|---|---|---|---|
| ButeMgr | 8 | 1 | `g++ ButeMgr/*.cpp -IPlatform` | **FAIL_WINDOWS** | In file included from Libs/Libs/ButeMgr/avector.cpp:1: |
| CryptMgr | 4 | 0 | `g++ CryptMgr/*.cpp -IPlatform` | **FAIL_HEADER** | In file included from Libs/Libs/CryptMgr/cryptmgr.cpp:1: |
| MFCStub | 11 | 0 | `g++ MFCStub/*.cpp -IPlatform` | **FAIL_WINDOWS** | In file included from Libs/Libs/MFCStub/mfcs_point.cpp:4: |
| RegMgr | 7 | 2 | `g++ RegMgr/*.cpp -IPlatform` | **FAIL_HEADER** | In file included from Libs/Libs/RegMgr/regmgr.cpp:3: |
| RegMgr32 | 3 | 0 | `g++ RegMgr32/*.cpp -IPlatform` | **FAIL_WINDOWS** | In file included from Libs/Libs/RegMgr32/regmgr32.cpp:1: |
| StackTracer | 5 | 3 | `g++ StackTracer/*.cpp -IPlatform` | **FAIL_HEADER** | In file included from Libs/Libs/StackTracer/stdafx.cpp:5: |
| dibmgr | 7 | 0 | `g++ dibmgr/*.cpp -IPlatform` | **FAIL_WINDOWS** | In file included from Libs/Libs/dibmgr/dibmgr.cpp:16: |
| dtxmgr | 10 | 2 | `g++ dtxmgr/*.cpp -IPlatform` | **FAIL_HEADER** | In file included from Libs/Libs/dtxmgr/dtxmgr_lib.cpp:4: |
| genregmgr | 2 | 1 | `g++ genregmgr/*.cpp -IPlatform` | **FAIL** | In file included from Libs/Libs/genregmgr/genregmgr.cpp:3: |
| lith | 66 | 6 | `g++ lith/*.cpp -IPlatform` | **FAIL** | In file included from Libs/Libs/lith/lithbaselist.cpp:17: |
| stdlith | 37 | 1 | `g++ stdlith/*.cpp -IPlatform` | **FAIL** | In file included from Libs/Libs/stdlith/stringholder.h:19, |
| zlib | 23 | 8 | `g++ zlib/*.cpp -IPlatform` | **NO_CPP** | no cpp files |
| ltmem | 20 | 2 | `g++ ltmem/*.cpp -IPlatform` | **FAIL** | In file included from Engine/Engine/libs/ltmem/ltheap.cpp:3: |
| ltamgr | 38 | 0 | `g++ ltamgr/*.cpp -IPlatform` | **FAIL** | In file included from Engine/Engine/sdk/inc/ltbasetypes.h:10 |
| rezmgr | 9 | 0 | `g++ rezmgr/*.cpp -I. -IPlatform` | **FAIL_HEADER*** | `Stdafx.h`/`iostream.h` viejo (con `-I.` el `Platform/security.h` sí se encuentra; fallo real es header MSVC) |
| Lib_DShow | 63 | 13 | `g++ Lib_DShow/*.cpp -IPlatform` | **FAIL_HEADER** | In file included from Engine/Engine/libs/Lib_DShow/pstream.c |

### Nota sobre `Platform/security.h`
El test manual usó `-IPlatform` solo; con `-I. -IPlatform` (como hace CMake via `target_include_directories(lith_platform INTERFACE ${CMAKE_SOURCE_DIR}/Platform + ${CMAKE_SOURCE_DIR})`) el include `Platform/security.h` sí resuelve. El fallo restante es `Stdafx.h`/`iostream.h` MSVC, no el shim.

**Interpretación:** Todas 16 librerías fallan en compile-test unitario con `g++ -IPlatform` porque necesitan headers MSVC/MFC completos (`Stdafx.h`, `iostream.h` viejo, `__int64`, `ltinteger.h` etc). Las más portables por `windows_tokens` bajo son `CryptMgr(0)`, `ButeMgr(1)`, `rezmgr(0)`, `ltmem(1)`, `stdlith(1)` — requieren solo fix header (`ltinteger.h` ya parcheado a `_LINUX` + `cstdint`, `Platform/d3d9.h` stub) y no DirectX. `Engine/libs/Lib_DShow(13)` y `externalesd(34)` son las menos portables (DirectShow/RealMedia).

## Demos — resultado compilación (representativo 5 demos)

| Demo | Archivos | windows_tokens | Test | Resultado | Causa |
|---|---|---|---|---|---|
| graphics/drawprim | 34 | 2 | `g++ cshell/src/clientinterfaces.cpp` | **FAIL** | ltinteger.h __int64 → int64 (parcheado), luego ltmatrix.h ltsinf/ltcosf faltante |
| graphics/shaders | 40 | 3 | `g++ cshell/src/clientinterfaces.cpp` | **FAIL** | d3d9.h faltante → Platform/d3d9.h stub creado, pero shader necesita HLSL→SPIR-V |
| networking/nettest | 6 | 1 | `g++ cshell/src/clientinterfaces.cpp` | **FAIL** | WorldProperties.h no encontrado (ruta Engine) |
| debugging/stacktrace | 31 | 3 | `g++ cshell/src/clientinterfaces.cpp` | **FAIL** | ltinteger.h __int64 (parcheado) |
| audio/music | 39 | 3 | `g++ cshell/src/clientinterfaces.cpp` | **FAIL** | ltinteger.h __int64 (parcheado) |

Todas las demos comparten el mismo patrón: dependen de `Engine/sdk/inc` (`ltbasetypes.h` → `ltinteger.h` → `__int64`, `ltsinf`) y de DirectX/MFC. Con `Platform/security.h` + `ltinteger.h` fix (`_LINUX`/`__LINUX` dual, `cstdint`) el header pasa, pero `ltmatrix`/`coordinate_frame` necesitan `-fpermissive` + `Platform/math_shim.h` (no creado aún).

## Estado proyecto principal

El **proyecto principal** (`CMakeLists.txt` root + `Engine` stub + `Renderer/vulkan`) **sí compila** con `cmake --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON && cmake --build build -j` → `liblith_engine.a 7.3K` + `librenderer_vulkan.a 185K` (exit 0). Esto valida toolchain Arch gcc 16.1.1 + Vulkan vendoreado 1.4.357 + SDL2 2.32.

Las **librerías/demos individuales** no compilan aisladas con `g++ -c` sin un `CMakeLists.txt` por lib/demo que aporte todos los includes (`-I Engine/sdk/inc -I runtime/shared/src -I Platform -fpermissive`). Crear esos CMakeLists es el siguiente paso del port incremental (estimado 2-3 días por lib).

## Recomendación port incremental (priorizar por graphify)

1. **Fase 1 — Headers portables (ya hecho parcial):** `Platform/*`, `ltinteger.h`, `d3d9.h stub` → permite que `ButeMgr`, `stdlith`, `rezmgr`, `ltmem` compilen con `-IPlatform`
2. **Fase 2 — Libs sin Windows (4 libs, ~30 files):** `CryptMgr(0 wt)`, `ButeMgr(1)`, `rezmgr(0)`, `ltamgr(0)` → crear `Libs/Libs/<name>/CMakeLists.txt` y test `find_package(SDL2)`
3. **Fase 3 — Math/RTTI:** `ltmatrix.h` (`ltsinf`→`sinf`), `coordinate_frame.h` template + `stdlith` (`__int64`→`int64_t`) con `-fpermissive`
4. **Fase 4 — Demos:** cada demo `Samples/Samples/<cat>/<demo>/cshell` necesita su `CMakeLists.txt` que linkee `lith_engine` + `renderer_vulkan`; priorizar `graphics/drawprim` (34 files, wt 2) y `networking/nettest` (6 files, wt 1) que son los más pequeños
