# Docs-Claude-Muse — Índice

Documentación del trabajo realizado con Claude en esta conversación
(2026-09-17/18): indexación del repo con graphify, port MSVC de la demo
sealhunter y fases de gameplay.

| Archivo | Contenido |
|---|---|
| `00-origen-tarea.md` | Sesión 0 (`tarea.txt`): sandbox roto, scaffolding CMake/Vulkan |
| `01-graphify.md` | Indexación completa del repo (pipeline, números, outputs) |
| `02-msvc-port.md` | Auditoría y port MSVC: configure, fixes de compilación y link |
| `03-sealhunter-fases.md` | Fases A–E: server shell, tick, menú, visuales, melee |
| `04-hallazgos.md` | Daño preexistente del árbol, quirks y riesgos abiertos |
| `05-inventario-sesion-1.md` | Diff 289d5607 (5049 archivos) + mapa del chat U1–U91 |
| `06-sesion-recuperacion.md` | 2ª época: rescate de sesión, fix del crash `CGui`, decisión R1→R4 |
| `07-metodologia-capturas.md` | Probes + PPM/PNG + `gen_captures.py` en `temp-captures/` |
| `08-render-vulkan-r1.md` | Pipeline 3D (mesh+depth+MVP) en `Renderer/vulkan` |
| `09-mundo-dat-r2.md` | Lector de `World.dat` + DTX + skybox + suelo |
| `10-modelos-ltb-r3.md` | Lector `.ltb` bind-pose, HARMGuard es el jugador |
| `11-estado-y-pendientes.md` | Arreglo en el cañón, R4 pendiente (melee/score/sonidos) |
| `TASK-DOC.md` | Lecciones generalizadas: regenerar renderer Vulkan en motor legacy |
| `TASK-PROMPT.md` | Prompt de trabajo reutilizable para esa tarea (p. ej. mod de Half-Life) |

## Reproducir el build (MSVC + vcpkg)

```bat
cmake -S . -B build-msvc -G "Visual Studio 18 2026" -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE=C:/Users/Shifty/DEV/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-msvc --config RelWithDebInfo --target networking_sealhunter
```

Correr (desde `Samples/Samples/networking/sealhunter`, con vcpkg bin en PATH):

```bat
networking_sealhunter.exe --frames=120   :: headless, sale solo
networking_sealhunter.exe --window       :: ventana (modo menú si sin --frames)
```

## Estado al cierre de Fase B

- `networking_sealhunter.exe` linkea limpio en MSVC, corre headless y en ventana.
- Server in-process: 3 focas spawneadas, tick 1× (120 frames → `time=2.00`).
- Grafo actualizado: `graphify-out/` con 168.140 nodos.
- Cambios sin commitear (ver `git status`): CMake, shims `Platform/`, host,
  1541 headers restaurados, `BaseFx.h`, `ParsedMsg.cpp`, `ltinteger.h`, `math_phys.h`.
