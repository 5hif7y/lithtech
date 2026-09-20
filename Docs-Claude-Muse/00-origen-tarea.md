# 00 — Origen: sesión scaffolding con sandbox roto (tarea.txt)

Trancripción literal de la sesión 0 en `tarea.txt` (= `Tarea.txt`,
idénticos): Muse Code 0.1.0 sobre Arch, con el sandbox roto
(`sandbox enforcement unavailable: /proc/self/exe`, más errores
`invalid type: string "false", expected a boolean` en search/read). Sin
poder ejecutar nada, la sesión produjo solo scaffolding vía write_file.

## Pedido original del usuario

Borrar binarios Windows → `git init` → indexar con graphify → portar a
CMake + gcc/g++ en ArchLinux + soporte Vulkan al renderer.

## Lo que dejó (archivos, para ejecutar en 1 min en Arch)

- `clean_windows_binaries.sh` (dry-run a `/tmp/windows-binaries.txt`,
  borra `.exe/.dll/.lib/.pdb/.obj/.exp/.ilk/.idb/.pch/.bsc` + dirs
  `Debug/Release/.vs/ipch`). No borrar `.vcxproj/.sln` aún: referencia
  para el CMake.
- `init_git.sh` + `.gitignore` (binarios, `build/`, CMakeCache, `*.spv`,
  `graphify-output/`).
- `run_graphify.sh` (autodetecta CLI: cargo/npm/pip; genera
  `graphify-output/nolf2-index.json`).
- `CMakeLists.txt` root (3.27, C++17, `LITH_VULKAN=ON`,
  `-Wall -fpermissive -fno-strict-aliasing`, `find_package(Vulkan SDL2)`),
  `cmake/toolchain-arch.cmake` (gcc/g++, `-march=native`),
  `Engine/CMakeLists.txt` (glob + excluir `d3d_*/dx*/win_*`, `-msse2`),
  `Renderer/vulkan/` (stub `VulkanRenderer::Init` + SPIR-V vía
  glslangValidator), `Platform/platform.h` (shim `windows.h` →
  `DWORD/HRESULT/__stdcall/stricmp→strcasecmp`), `BUILD.md`.

## Decisiones que nacieron acá y siguen vigentes

- Todo lo Windows-específico centralizado en `Platform/` (shims), nunca
  `#ifdef` dispersos.
- D3D8/9 no se porta: backend Vulkan nuevo desde cero (`VK_USE_PLATFORM_XLIB_KHR`,
  SDL2 para `VkSurfaceKHR`, HLSL→GLSL→SPIR-V).
- `__asm` de Math/Renderer → intrínsecos SSE2 (`-msse2`).
- Preguntas que quedaron al usuario: alcance del borrado, dónde va el
  `git init`, qué binario graphify usa (resueltas en sesión 1).
