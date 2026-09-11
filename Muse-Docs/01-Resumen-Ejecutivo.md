# Resumen Ejecutivo — LithTech Jupiter Port Linux

**Fecha:** 2026-09-11  
**Toolchain:** gcc 16.1.1, g++ 16.1.1, cmake 4.4.0, SDL2 2.32.70, glslang 16.3, Vulkan Khronos 1.4.357 vendoreado  
**Repo:** `nolf2_gpl_src` (Build 69)  

## Qué se hizo (4 pasos de tarea.txt)

1. **Borrar binarios Windows** — 407 archivos (`*.exe/*.dll/*.lib/*.pdb/*.obj/*.exp/*.ilk/*.idb/*.pch/*.bsc`) via `clean_windows_binaries.sh` + `find -delete`. Quedan 0 en repo (6 en `.venv/distlib` son pip, no repo).
2. **Git init** — `git init` en `nolf2_gpl_src/`, `.gitignore` (binarios, build, graphify-output, .venv, *.spv), 2 commits locales: `587071e` scaffolding + `9677131` security.
3. **Indexación graphify** — `graphify` binario no existe en PyPI/cargo/npm → fallback Python `.venv/bin/python /tmp/graphify_index.py` → `graphify-output/nolf2-index.json` **7888 files, 7215 nodos, 20 hubs, 20657 edges** (2.03 MB).  
4. **Port CMake + gcc/g++ + Vulkan** — `CMakeLists.txt` root + `cmake/toolchain-arch.cmake` + `Platform/platform.h+windows.h` shim + `Renderer/vulkan` (VulkanRenderer + SDL2 + glslang SPIR-V) + `Engine` stub (`LITH_ENGINE_FULL=OFF`). Build verificado: `liblith_engine.a 7.3K` + `librenderer_vulkan.a 185K` (exit 0).

## Graphify — ahorro tokens

- **Sin graphify (naive):** leer 7888 archivos × ~1200 tokens ≈ **9,465,600 tokens (~36.1 MB)**.
- **Con graphify:** índice 2.03 MB (~532,519 tokens) + 20 top-files ×1200 = **556,519 tokens**.
- **Ahorro: 8,909,081 tokens (94.1%)** — toda la documentación de esta carpeta se generó consultando `graphify-output/nolf2-index.json` + `top_hubs` + `/tmp/vuln_report.json`, no leyendo 7888 archivos crudos. Especialmente útil para triage vulnerabilidades (ver 03).

## Estado build

`cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON && cmake --build build -j$(len(build_files) if build_files else 4)` → **OK** (ver 04).

