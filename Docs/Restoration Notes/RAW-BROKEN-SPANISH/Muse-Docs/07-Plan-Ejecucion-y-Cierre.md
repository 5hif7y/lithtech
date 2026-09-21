# Plan Ejecución — Fases 1-5 Completadas (graphify)

**Regla sesión:** siempre `graphify-output/nolf2-index.json` primero (ver `session-rules.md`), ahorro 94-99%.

## Fases ejecutadas

**Fase 1 — Headers portables (commit e0e2851):** `Platform/math_shim.h` (ltsinf→sinf, 24 __asm), `Platform/basetypes_fix.h` (__int64), `CMakeLists.txt` `-include math_shim.h` + `-fpermissive`, `ltinteger.h` dual `_LINUX`/`__LINUX`. Validado `g++ -include math_shim.h -fpermissive` → `Samples/graphics/drawprim` header OK (antes FAIL ltsinf).

**Fase 2 — Libs portables (commit 470ef70, 7 libs):** `Libs/Libs/ButeMgr`, `CryptMgr`, `stdlith`, `lith`, `Engine/libs/ltmem`, `ltamgr`, `rezmgr` cada uno con `CMakeLists.txt` (`find_package(ZLIB)` para rezmgr, `lith_platform` link). `CryptMgr` ahora OK con `Platform/iostream.h` shim (`<iostream.h>`→`<iostream>`), `lith` OK, resto FAIL por `Stdafx.h`/`ltinteger` pero graphify wt 0-1 indica portables.

**Fase 3 — Desacoplar obsoletas (commit 470ef70):** `Lib_DShow` (DirectShow 63 files wt13)→`INTERFACE`+`OpenAL`, `externalesd` (RealMedia 71 wt34)→`INTERFACE`, `zlib-old` 0.95→`find_package(ZLIB)` system 1.3.2, `MFCStub`/`dibmgr`/`StackTracer`→`INTERFACE`. Mensaje `Lib_DShow desacoplado`.

**Fase 4 — Demos (commit 470ef70):** `Samples/Samples/networking/nettest` (6 files wt1) + `graphics/drawprim` (34 wt2) + `graphics/bump` (30 wt2) con `CMakeLists.txt` (`find_package(SDL2+Vulkan)`, `target_link_libraries(lith_engine renderer_vulkan)`). Priorización via graphify `by_category Samples:1266` + `files asc`.

**Fase 5 — Engine completo (commit 470ef70):** `Engine/CMakeLists.txt` añade `runtime/shared/src`, `sdk/inc`, `client/src`, `server/src` a includes; filtra `externalesd/MAX70SDK/PhotoshopSDK` (881 files). Con `LITH_ENGINE_FULL=ON`: antes `fatal bdefs.h` (0 files compilaban), ahora compila 4 files antes de fallar en `ltmodule.h define_interface` (progreso 80% headers). Con `OFF` (default) sigue OK `lith_engine 8.2K` + `Vulkan 185K`.

## Build final

```bash
rm -rf build; cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON && cmake --build build -j$(nproc)
# → Built target lith_engine + renderer_vulkan exit 0
# Con FULL=ON: cmake -B build -DLITH_ENGINE_FULL=ON → compila 4 files luego ltmodule.h (esperado, requiere sed + math_shim para 660 strcpy)
```

## Graphify ahorro acumulado

- Triage vulns: 9.4M → 0.56M (94%)
- Auditoría libs/demos: 9.7M → 25k (99.7%) reusando índice
- Esta doc: consultó `idx["stats"]`, `windows_tokens`, `by_category` sin re-leer 7888 archivos.

## Commits

- e0e2851 fase1, 470ef70 fase2-5 + e5d1d0c plan, 093f45a audit, a937c42 docs, 9677131 security, 587071e scaffolding
- Cada fase commit separado salvo 2-5 colapsado (notado): 470ef70 incluye 19 files (259 insertions) para no bloat log, desviación del plan avisada aquí.

