# 05 — Inventario de la sesión 1 rota (diff 289d5607 + chat)

Reconstrucción de lo que hizo la sesión `289d5607-…` (11–14 sep 2026) a
partir de `gitdiff-sesion1-289d5607-….diff` (5049 archivos, +805.936/−5214)
y `Muse-Docs/CONVERSACION.md` (esqueleto de turnos U1–U91 abajo). Lo
narrativo vive en `Muse-Docs/01–09`; acá el inventario verificable.

## El diff por áreas (5049 archivos)

| Área | Archivos | Qué es |
|---|---|---|
| `Engine/Engine/tools` | 3469 | headers restaurados (faltaban del árbol) |
| `Engine/Engine/runtime` | 510 | headers restaurados del runtime |
| `Engine/Engine/libs` | 133 | DirectShow/RealMedia/externals |
| `Engine/Engine/sdk` | 127 | headers del SDK |
| `Engine/Engine/clientfx` | 71 | los `M Engine/...` del `git status` actual |
| `Samples/Samples/graphics` | 290 | copias de demos gráficas |
| `Samples/Samples/networking` | 96 | incluye el árbol sealhunter que compila |
| `Samples/Samples/models/objects/base/audio/debugging` | 214 | resto de demos |
| `Libs/Libs` | 75 | dtxmgr, stdlith, etc. |
| `Renderer/vulkan` | 4 | `CMakeLists + Vk_renderer.h + vk_renderer.{h,cpp}` |
| `Platform/` | 5 | `Platform.h, windows.h, crtdbg.h` + `host/host_engine.h, host_sealhunter.cpp` |
| raíz | 1 | `CMakeLists.txt` |

Lectura: la sesión 1 fue **restauración masiva del árbol** (headers que
faltaban) + **copias de demos** + el esqueleto que la sesión 2 extendió
(host Vulkan, renderer 2D, CMake MSVC). Nótese `Vk_renderer.h` vs
`vk_renderer.h`: el par case-variante que motivó la auditoría
case-sensitive/case-insensitive (U35) para compilar igual en Windows y
Unix-likes.

## Mapa de la conversación (U = turno usuario en CONVERSACION.md)

- **U1–U12**: retomar flujo roto, commit, caza de vulnerabilidades/leaks,
  graphify, `Muse-Docs/`, primera compilación.
- **U13–U27**: probes Vulkan, compilar todas las demos y libs.
- **U28–U45**: fix drawprim, auditoría case-(in)sensitive (U35), commits,
  cortes por batería/apagones (U41/U45).
- **U47–U56**: "¿son stubs o funcionan?" (U47), los 3 `Engine.REZ`
  distintos (U54), llega `demo-sealhunter/` (U56, copia que funciona en
  Windows).
- **U58–U68**: adaptar sealhunter a Unix+Vulkan, borrar `.exe/.dll/.lib`,
  drivers mesa, primera ventana (rectángulo blanco en celeste).
- **U73–U79**: imagen invertida → normalizar orden de dibujado
  (Vulkan ≠ GL/DX); SDL2 evaluado y descartado; SDL3 + crash `BadMatch`
  en `XSetInputFocus`; menú sin flechas; mundo 3D sin render + crash
  ocasional.
- **U80–U84**: cortes de conexión repetidos; costo graphify no lineal
  (U82/U84, nunca diagnosticado del todo).
- **U86–U91**: verificación de `tarea.txt` 5/5, auditoría de calidad y la
  orden U89 que produjo `Muse-Docs/` + `TODO.md` + `GUIA-DEMOS.md` + la
  propia `CONVERSACION.md`.

## Estado que heredó la sesión 2 (cierre en `Muse-Docs/TODO.md`)

Menú booteando sin input navegable, `T_RenderCamera` no-op (celeste+HUD
esperado), mundo 3D y Engine full como hitos abiertos, 19 demos con solo
3 probes frescos. Todo lo de gameplay real (R1→R4) es sesión 2.
