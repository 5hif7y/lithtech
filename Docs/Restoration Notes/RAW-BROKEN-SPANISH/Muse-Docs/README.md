# Muse-Docs — LithTech Jupiter Port Linux

Documentación generada **usando graphify para ahorrar tokens** (ver `02-Graphify-Uso-y-Ahorro-Tokens.md`).

## Índice

1. [Resumen Ejecutivo](01-Resumen-Ejecutivo.md) — 4 pasos tarea.txt, toolchain, ahorro 94%
2. [Graphify — Uso y Ahorro Tokens](02-Graphify-Uso-y-Ahorro-Tokens.md) — fallback Python, medición, regeneración
3. [Vulnerabilidades y Leaks](03-Vulnerabilidades-y-Leaks.md) — scan 662/1078/399, top files, leaks new>>delete, fixes 3 archivos + Platform/security.h
4. [Compilación](04-Compilacion.md) — toolchain Arch gcc 16.1, shims, Vulkan vendoreado, por qué Engine stub
5. [Git e Historial](05-Git-e-Historial.md) — 2 commits, qué se trackea, .gitignore
6. [Auditoría Librerías y Demos](06-Auditoria-Librerias-y-Demos.md) — 17 libs + 16 demos, todas FAIL aisladas, principal sí compila
7. [Estructura Repo (graphify by_category)](../graphify-output/nolf2-index.json) — Engine:4357 Game:2008 Libs:145 Samples:1266

## Quick start

```bash
cmake -B build --toolchain cmake/toolchain-arch.cmake -DLITH_VULKAN=ON
cmake --build build -j$(nproc)  # → lith_engine 7.3K + renderer_vulkan 185K
```

## Graphify — token saving demo

```bash
.venv/bin/python /tmp/vuln_scan.py          # triage sin leer 7888 files
.venv/bin/python /tmp/vuln_scan_after.py    # re-scan 4 files fix + index
```

## Sesión 11–14 sep (runtime Vulkan)

8. [Demos Vulkan en Runtime](08-Demos-Vulkan-Runtime.md) — probes, ventana SDL3/X11, menú, input, SDL2 vs SDL3
9. [Límites, Gaps y Riesgos](09-Limites-Gaps-Riesgos.md) — sandbox sin X, RenderCamera no-op, crash pendiente
10. [Guía Demos](GUIA-DEMOS.md) — compilar y correr las 19 demos
11. [TODO](TODO.md) — tareas faltantes
12. [Conversación](CONVERSACION.md) — copia directa de la sesión
13. [Digested Prompt](DIGESTED-PROMPT.md) — prompt maduro para restauración legacy
