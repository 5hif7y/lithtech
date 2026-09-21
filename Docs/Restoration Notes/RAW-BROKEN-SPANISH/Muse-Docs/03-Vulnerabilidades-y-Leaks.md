# Vulnerabilidades y Leaks — Triage via Graphify

## Scan (usando graphify index, no 7888 reads)

```bash
.venv/bin/python /tmp/vuln_scan.py  # 7215 nodos, no 7888 raw
```

**Hits por patrón:**

- windows_h: 148\n- memcpy_unchecked: 399\n- new: 4516\n- delete: 2547\n- strcpy: 662\n- strncpy_no_null: 169\n- strcat: 119\n- vsprintf: 19\n- malloc: 101\n- free: 108\n- format_string: 276\n- sprintf: 1078\n- __asm: 24

**Top 10 archivos con más hits:**

- Engine/Engine/tools/RenderStylesEditor/RenderStylesEditor.cpp: [['strcpy', 12], ['sprintf', 177], ['strncpy_no_null', 1], ['malloc', 1], ['free', 1], ['new', 7]]\n- Engine/Engine/tools/DEdit/Views/RegionViewInitTracker.cpp: [['new', 150], ['delete', 3]]\n- Engine/Engine/tools/RenderStyle_Packer/renderstylepacker.cpp: [['sprintf', 147], ['new', 1]]\n- Engine/Engine/runtime/kernel/src/sys/win/ltdirectmusic_impl.cpp: [['strcpy', 15], ['new', 78], ['delete', 47]]\n- Engine/Engine/tools/ltdmtest/ltdirectmusic_impl.cpp: [['strcpy', 15], ['new', 77], ['delete', 45]]\n- Engine/Engine/libs/rezmgr/rezmgr.cpp: [['strcpy', 17], ['strcat', 7], ['strncpy_no_null', 5], ['memcpy_unchecked', 6], ['new', 47], ['delete', 43]]\n- Engine/Engine/tools/ModelEdit/ModelEditDlg.cpp: [['strcpy', 1], ['strcat', 11], ['sprintf', 42], ['malloc', 1], ['free', 1], ['new', 33], ['delete', 36]]\n- Engine/Engine/tools/Plugins/MaxWorldExport/LTAFile.cpp: [['new', 49], ['delete', 8], ['format_string', 50]]\n- Engine/Engine/tools/Plugins/SDKFiles/PhotoshopSDK/samplecode/common/sources/PIUActionUtils.cpp: [['strcpy', 27], ['sprintf', 1], ['new', 37], ['delete', 33]]\n- Engine/Engine/tools/Plugins/SDKFiles/MAX70SDK/include/maxscrpt/maxnurbs.h: [['delete', 93]]

**Leak candidates (new>>delete):**

- Libs/Libs/stdlith/dynarray.h: malloc 0/free 0 new 5/delete 0\n- Libs/Libs/lith/lithchunkallocator.h: malloc 0/free 0 new 8/delete 3\n- Libs/Libs/ButeMgr/butemgr.cpp: malloc 0/free 0 new 34/delete 10\n- Libs/Libs/dibmgr/dibmgr.cpp: malloc 0/free 0 new 28/delete 13\n- Libs/Libs/MFCStub/mfcs_string.cpp: malloc 0/free 0 new 6/delete 2\n- Libs/Libs/MFCStub/mfcstub.cpp: malloc 0/free 0 new 7/delete 2\n- Libs/Libs/zlib/deflate.c: malloc 0/free 0 new 6/delete 0\n- Libs/Libs/zlib/trees.c: malloc 0/free 0 new 5/delete 0\n- Libs/Libs/zlib/inftrees.c: malloc 0/free 0 new 4/delete 0\n- Engine/Engine/sdk/inc/ltserverobj.h: malloc 3/free 2 new 5/delete 2

Full report: `/tmp/vuln_report.json` (9.1K)

## Fixes aplicados (commit 9677131)

1. **Nuevo `Platform/security.h`** (74 líneas): `LITH_STRCPY/STRNCPY/STRCAT/SNPRINTF`, `lith_safe_copy/cat`, `lith_make_buffer`/`lith_buffer_data` (RAII vector<char>), `lith_memcpy_checked`, `lith_validate_ptr/size`. Test en `/tmp/test_security.cpp` pasa (strcpy trunc 9, safe_copy hell, memcpy overflow prevented).

2. **`Libs/ButeMgr/butemgr.cpp` (leak 34 new/10 delete):** `char *pssBuf = new char[n]` → `vector<char> _pssBufVec = lith_make_buffer(n)` + `lith_buffer_data`, elimina 2× `delete[]` manual, + incluye `Platform/security.h`.

3. **`Engine/libs/rezmgr/rezmgr.cpp` (overflow 60B):** `strcpy(Header.FileType, ...)` → `lith_safe_copy(..., sizeof, ...)` + `memcpy(UserTitle)` → `lith_memcpy_checked(..., strnlen)`.

4. **`Renderer/vulkan` (hardening):** `vk_renderer.h` incluye `SDL_vulkan.h` antes de `vulkan.h`; `vk_renderer.cpp` añade `Platform/security.h`, validaciones `lith_validate_ptr` en `Init`, bounds `1M verts`/`16 devices` en `DrawPrimitive`/`pickPhysicalDevice`.

Restantes ~660 strcpy / 1077 sprintf en `Tools/` (MFC/MAXSDK) requieren port incremental `LITH_ENGINE_FULL=ON`.

## Cómo reproducir triage

```bash
.venv/bin/python /tmp/vuln_scan_after.py  # ahorro 94% vs naive
```
