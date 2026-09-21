# Git e Historial

## Commits

```
9677131 security: fix leaks + overflows via graphify triage, add Platform/security.h
 Engine/Engine/libs/rezmgr/rezmgr.cpp | 2338 ++++++++++++++++++++++++++++++++++
 Libs/Libs/ButeMgr/butemgr.cpp        | 2315 +++++++++++++++++++++++++++++++++
 Platform/security.h                  |   74 ++
 Renderer/vulkan/vk_renderer.cpp      |   20 +-
 4 files changed, 4742 insertions(+), 5 deletions(-)
587071e chore: clean windows binaries (407), add cmake+gcc/vulkan arch toolchain + graphify index
 .gitignore                                         |     43 +
 BUILD.md                                           |     63 +
 CMakeLists.txt                                     |     77 +
 Engine/CMakeLists.txt                              |     40 +
 Platform/platform.h                                |     76 +
 Platform/windows.h                                 |     40 +
 Renderer/vulkan/CMakeLists.txt                     |     55 +
 Renderer/vulkan/vk_renderer.cpp                    |     90 +
 Renderer/vulkan/vk_renderer.h                      |     46 +
 clean_windows_binaries.sh                          |     27 +
 cmake/toolchain-arch.cmake                         |     16 +
 graphify-output/nolf2-index.json                   |  95121 ++++++++
 init_git.sh                                        |     55 +
 run_graphify.sh                                    |     30 +
 .../include/vk_video/vulkan_video_codec_av1std.h   |    394 +
 .../vk_video/vulkan_video_codec_av1std_decode.h    |    109 +
 .../vk_video/vulkan_video_codec_av1std_encode.h    |    143 +
 .../include/vk_video/vulkan_video_codec_h264std.h  |    314 +
 .../vk_video/vulkan_video_codec_h264std_decode.h   |     77 +
 .../vk_video/vulkan_video_codec_h264std_encode.h   |    147 +
 .../include/vk_video/vulkan_video_codec_h265std.h  |    446 +
 .../vk_video/vulkan_video_codec_h265std_decode.h   |     67 +
 .../vk_video/vulkan_video_codec_h265std_encode.h   |    157 +
 .../include/vk_video/vulkan_video_codec_vp9std.h   |    151 +
 .../vk_video/vulkan_video_codec_vp9std_decode.h    |     68 +
 .../include/vk_video/vulkan_video_codecs_common.h  |     36 +
 third_party/vulkan/include/vulkan/vk_icd.h         |    244 +
 third_party/vulkan/include/vulkan/vk_layer.h       |    191 +
 third_party/vulkan/include/vulkan/vk_platform.h    |     83 +
 third_party/vulkan/include/vulkan/vulkan.cppm      |   1349 +
 third_party/vulkan/include/vulkan/vulkan.h         |    102 +
 third_party/vulkan/include/vulkan/vulkan.hpp       |  27928 +++
 third_party/vulkan/include/vulkan/vulkan_android.h |    159 +
 third_party/vulkan/include/vulkan/vulkan_beta.h    |    375 +
 third_party/vulkan/include/vulkan/vulkan_core.h    |  27186 +++
 .../vulkan/include/vulkan/vulkan_directfb.h        |     59 +
 third_party/vulkan/include/vulkan/vulkan_enums.hpp |  10861 +
 .../include/vulkan/vulkan_extension_inspection.hpp |   4466 +
 .../vulkan/include/vulkan/vulkan_format_traits.hpp |  10654 +
 third_party/vulkan/include/vulkan/vulkan_fuchsia.h |    282 +
 third_party/vulkan/include/vulkan/vulkan_funcs.hpp |  43803 ++++
 third_party/vulkan/include/vulkan/vulkan_ggp.h     |     62 +
 .../vulkan/include/vulkan/vulkan_handles.hpp       |  33623 +++
 third_party/vulkan/include/vulkan/vulkan_hash.hpp  |  22981 ++
 .../vulkan/include/vulkan/vulkan_hpp_macros.hpp    |    355 +
 third_party/vulkan/include/vulkan/vulkan_ios.h     |     50 +
 third_party/vulkan/include/vulkan/vulkan_macos.h   |     50 +
 third_party/vulkan/include/vulkan/vulkan_metal.h   |    244 +
 third_party/vulkan/include/vulkan/vulkan_ohos.h    |    120 +
 third_party/vulkan/include/vulkan/vulkan_raii.hpp  |  30255 +++
 third_party/vulkan/include/vulkan/vulkan_screen.h  |    114 +
 .../vulkan/include/vulkan/vulkan_shared.hpp        |   1217 +
 .../include/vulkan/vulkan_static_assertions.hpp    |  10627 +
 .../vulkan/include/vulkan/vulkan_structs.hpp       | 220597 ++++++++++++++++++
 .../vulkan/include/vulkan/vulkan_to_string.hpp     |  12091 +
 third_party/vulkan/include/vulkan/vulkan_ubm.h     |     59 +
 third_party/vulkan/include/vulkan/vulkan_vi.h      |     50 +
 .../vulkan/include/vulkan/vulkan_video.cppm        |     73 +
 third_party/vulkan/include/vulkan/vulkan_video.hpp |   5529 +
 third_party/vulkan/include/vulkan/vulkan_wayland.h |     59 +
 third_party/vulkan/include/vulkan/vulkan_win32.h   |    372 +
 third_party/vulkan/include/vulkan/vulkan_xcb.h     |     60 +
 third_party/vulkan/include/vulkan/vulkan_xlib.h    |     60 +
 .../vulkan/include/vulkan/vulkan_xlib_xrandr.h     |     50 +
 64 files changed, 564328 insertions(+)

```

## Qué se trackea

- **Trackeado (commit 587071e):** `.gitignore`, `BUILD.md`, `CMakeLists.txt`, `Platform/`, `Renderer/vulkan/`, `cmake/toolchain-arch.cmake`, `Engine/CMakeLists.txt`, `third_party/vulkan/` (vendoreado), `graphify-output/nolf2-index.json` (2.1M, con `git add -f` porque `.gitignore` ignora `graphify-output/` y `build/`), `clean_windows_binaries.sh`, `init_git.sh`, `run_graphify.sh`
- **Commit 9677131:** `Platform/security.h`, `Libs/ButeMgr/butemgr.cpp` (RAII), `Engine/libs/rezmgr/rezmgr.cpp` (safe copy), `Renderer/vulkan/` hardening
- **No trackeado (intentional, .gitignore):** `build/` (CMakeCache, .a), `.venv/` (pip distlib *.exe son 6 *.exe restantes, no repo), `Development/ TO2 .rez/.bmp` (assets), `Game/ Libs/ Linux/ Samples/` (fuentes completas aún no porteadas, 7888 files totales en índice pero no todos commiteados para no bloat)
- **Para trackear todo:** `git add Engine/ Game/ Libs/ ...` (7888 files) → commit enorme, se dejó fuera a propósito hasta port incremental.

## .gitignore

```
graphify-output/  # se trackea con -f solo el índice
build/ build-*/ out/
*.exe *.dll *.lib *.pdb *.obj
.venv/ __pycache__/
*.spv
!third_party/vulkan/
```

## Reproducir

```bash
git status --short
git log --oneline -3
git show HEAD --stat
```

## Recovery

Antes de commits se intentó `workspace-recovery.sh save` → `unborn HEAD is unsupported` (esperado en repo nuevo).
