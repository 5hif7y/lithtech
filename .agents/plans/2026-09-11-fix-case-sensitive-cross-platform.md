# Plan: Fix case-sensitive vs case-insensitive para compilable en Windows y Unix-likes

## Goal
Hacer que `nolf2_gpl_src` compile idénticamente en Windows (NTFS case-insensitive) y en Unix-likes (ext4/APFS case-sensitive) corrigiendo los 2155 mismatches ` #include "Foo.h"` vs archivo en disco `foo.h` detectados vía graphify, sin romper builds existentes.

## Success Criteria
- `cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON && cmake --build build -j` compila en Linux (Arch ext4) y en Windows (VS NTFS) sin `fatal error: Foo.h: No such file` por case.
- `cmake --build build --target graphics_drawprim -j4 && ./build/Samples/Samples/graphics/drawprim/graphics_drawprim` produce ELF runnable en Linux y .exe en Windows.
- No quedan mismatches case en los 2155 reportados: `python3 /tmp/audit_case2.py` → `Total mismatches: 0` tras fixes.
- `graphify-output/nolf2-index.json` regenerado sigue 7888 files, `windows_tokens` estable, `Muse-Docs/` y `DemoVulkan` siguen OK.

## Context And Current Facts
**Auditoría graphify (ahorro tokens, no leer 7888 raw):** `graphify-output/nolf2-index.json` 7888 files, 7215 nodos, ahorro 94% (7215 vs 7888). `python3 /tmp/audit_case2.py` escaneando solo nodos `ext .h/.hpp/.cpp` con `#include` regex encontró **2155 mismatches case**: `Engine 2033`, `Samples 120`, `Libs 2`. Top: `dedit.h 87`, `edithelpers.h 53`, `iFnPub.h 49`, `Max.h 39` etc. Ejemplos: `Libs/ButeMgr/butemgr.cpp "#include \"Stdafx.h\""` vs disco `stdafx.h` (stdlith), `Engine/sdk/inc/LTEulerAngles.cpp "#include \"lteulerangles.h\""` vs `LTEulerAngles.h`, `Engine/libs/externalesd/... "RngInterstitialClient.h"` vs `rnginterstitialclient.h`.

**Filesystem:** Windows NTFS case-insensitive ( `Stdafx.h` == `stdafx.h` ), Linux ext4 case-sensitive (distinto). Ya hay symlinks creados manualmente para `DynamicMesh.h → dynamicmesh.h`, `WorldProperties.h → worldproperties.h`, `BaseFX.h → BaseFx.h`, `ltes` etc, pero faltan 2155.

**Build actual:** `Engine/CMakeLists.txt`, `Samples/*/CMakeLists.txt` ya tienen `Platform/math_shim.h` `-include`, `Platform/windows.h` shims, `ltinteger.h` dual `_LINUX`, pero `graphics_drawprim` aún falla en `ltclientshell.cpp:22 ClientFXDB.h` y luego `BaseFX.h` y luego `LinkList.h` todos por case.

**Regla sesión:** siempre usar `graphify` para auditoría (session-rules.md).

## Constraints And Non-goals
- Debe compilar en **ambos** Windows y Unix sin `ifdef _WIN32` por include: el mismo `#include "Stdafx.h"` debe funcionar en ambos si el fix es correcto.
- No romper `vulkan_demo` 604K ni `DemoVulkan` headless 800x600.
- No mover archivos en disco a nombres nuevos que rompan historial git más de lo necesario; preferir fix de `#include` + symlinks como fallback, no rename masivo de 2155 archivos.
- No añadir case-insensitive FS en Linux (no `ciopfs`).

## Key Decisions
### 1. Fix de includes vs symlinks vs FS wrapper
- **Recomendado: híbrido Fix includes (primario) + symlinks (fallback).** Fix ` #include "Foo.h"` para que use el case exacto del archivo en disco (ej. `Stdafx.h` → `stdafx.h` si el archivo es `stdafx.h`). Esto funciona en ambos OS. Symlinks (`ln -s stdafx.h Stdafx.h`) como red de seguridad para includes que vienen de SDKs externos (MAX SDK) donde no queremos editar. Rechazado: solo symlinks (2155 symlinks, fragile, git no trackea bien en Windows), solo FS wrapper (ciopfs, no portable).

### 2. Herramienta de fix
- **Recomendado: `python3 /tmp/fix_case.py` que usa `graphify` `basename_map` para mapear cada `inc` a `actual` y hace `sed -i` del include.** Rechazado: `sed` manual por archivo (lento), `git mv` de archivos (rompe blame).

### 3. Validación case en CI
- **Recomendado: añadir `cmake -P cmake/check_case.cmake` que re-ejecuta `audit_case2.py` y falla si mismatches >0.** Rechazado: no check (reintroduce bugs).

## Recommended Approach
En un solo paso, usar `graphify` para no re-leer 7888: `audit_case2.py` ya tiene `mismatches` lista (2155). Iterar esa lista y para cada `(src, inc, actual, found)` hacer `sed -i "s|#include \"inc\"|#include \"actual\"|"` en `src`. Luego crear symlinks inversos (`actual -> inc` y `inc -> actual`) para que ambos cases funcionen en Linux y Windows siga igual. Finalmente `cmake -P` check.

## Work Plan
**Unidad 1 — Fix includes (1 commit)**
- Input: `/tmp/case_audit.json` (2155 mismatches) via graphify.
- Script `python3 /tmp/fix_case_includes.py` que para cada mismatch hace `pathlib.Path(src).read_text().replace(f'#include "{inc}"', f'#include "{actual}"')` y escribe. Usa `inc` exacto, no lower, para no tocar otros includes.
- Validación: `python3 /tmp/audit_case2.py` → `Total mismatches: 0`.

**Unidad 2 — Symlinks fallback (1 commit, mismo PR)**
- Para cada `actual` en `basename_map`, crear `lower` y `Cap` symlinks si no existen: `ln -s actual lower` y `ln -s actual Cap` (como ya se hizo para `DynamicMesh.h`, `WorldProperties.h`, `BaseFX.h`). Esto cubre SDKs que no queremos editar.
- Validación: `find . -type l | wc -l` crece ~2155, `ls Engine/sdk/inc/lteulerangles.h` y `LTEulerAngles.h` ambos existen.

**Unidad 3 — CI check (1 commit)**
- Crear `cmake/check_case.cmake` que ejecuta `python3 /tmp/audit_case2.py` y `if(mismatches>0) message(FATAL_ERROR)`.
- Añadir a `CMakeLists.txt` `add_custom_target(check_case COMMAND python3 ...)`.
- Validación: `cmake --build build --target check_case` pasa.

## Validation Plan
- **Por unidad 1:** `python3 /tmp/audit_case2.py 2>&1 | grep "Total mismatches"` debe ser `0` (antes 2155). `grep -r '#include "Stdafx.h"' Libs/` debe ser 0, solo `stdafx.h`.
- **Por unidad 2:** `find Engine -type l | grep -i Stdafx` muestra `Stdafx.h -> stdafx.h`. `ls Engine/sdk/inc/LTEulerAngles.h` y `lteulerangles.h` ambos existen.
- **E2E:** `rm -rf build; cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON && cmake --build build -j && cmake --build build --target graphics_drawprim -j4 && file build/Samples/Samples/graphics/drawprim/graphics_drawprim | grep ELF` debe ser `ELF 64-bit` en Linux y `cmake --build build --target graphics_drawprim` en Windows con `msbuild` debe producir `.exe`.
- **Regresión:** `./build/DemoVulkan/vulkan_demo --headless` → `HEADLESS: Saved /tmp/vulkan_demo.ppm 800x600` y `build/Engine/liblith_engine.a` 8.2K siguen.

## Risks / Rollback
- **Riesgo:** fix de includes toca 2155 líneas, puede romper `git blame` y `Engine/libs/externalesd/realconsole` (SDK externo). Mitigación: symlinks cubren caso, y `externalesd` ya está desacoplado (`INTERFACE`) en `Engine/CMakeLists.txt` (`EXCLUDE REGEX ".*externalesd.*"`), no compila.
- **Riesgo:** symlinks no funcionan en Windows `git clone` con `core.symlinks=false`. Mitigación: el fix primario es includes, symlinks son fallback solo para Linux; Windows no los necesita por ser case-insensitive.
- **Rollback:** cada unidad es commit separado; `git revert <hash>` por unidad. `rm find . -type l -delete` limpia symlinks.

## Open Questions
- None. Asunciones: `Engine` 2033 mismatches son todos fixables via `sed` (verificado top 20 son `dedit.h` etc en `Engine/libs/externalesd` que ya está excluido, no afecta build principal).

## Sources
- No hay fuentes externas nuevas; el plan usa workspace facts `graphify-output/nolf2-index.json` (7888 files) y `audit_case2.py` (2155 mismatches) inspeccionados en esta corrida. El comportamiento NTFS case-insensitive vs ext4 case-sensitive es hecho del OS, no requiere fetch externo.

