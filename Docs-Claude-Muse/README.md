# Docs-Claude-Muse — Índice

Documentación del trabajo realizado con Claude en esta conversación
(2026-09-17/18): indexación del repo con graphify, port MSVC de la demo
sealhunter y fases de gameplay.

| Archivo | Contenido |
|---|---|
| `01-graphify.md` | Indexación completa del repo (pipeline, números, outputs) |
| `02-msvc-port.md` | Auditoría y port MSVC: configure, fixes de compilación y link |
| `03-sealhunter-fases.md` | Fases A–E: server shell, tick, menú, visuales, melee |
| `04-hallazgos.md` | Daño preexistente del árbol, quirks y riesgos abiertos |

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
