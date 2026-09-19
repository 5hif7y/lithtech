# 01 — Indexación graphify del repo

## Comando

`/graphify .` con scope **repo completo** (decisión del usuario ante el
warning de corpus grande).

## Corpus detectado

- **10.813 archivos · ~6,5M palabras** (code 7526, docs 251, papers 26,
  imágenes 2, video 3008).
- Videos excluidos con criterio: 3004 `.wav` (SFX/diálogos del juego) +
  4 `.avi`; transcribirlos no aportaba nada al grafo.
- Sin `GEMINI_API_KEY`: la extracción semántica la hicimos con subagentes
  del host (16 chunks, `general-purpose`).

## Pipeline ejecutado

1. `detect` → `graphify-out/.graphify_detect.json` (top dirs: Engine 4268,
   Development 2924, Game 1996, Samples 1220…).
2. **AST** sobre 7526 archivos de código en background:
   **150.382 nodos, 280.811 aristas**.
3. **16 chunks semánticos** en paralelo (root docs, atributos TO2,
   música/voz, engine tools, game i18n, libs cmake, Muse-Docs, samples,
   26 PDFs Photoshop SDK + JupiterLTASchema, 2 logos): **346 nodos**.
4. Merge → **150.728 nodos, 280.966 edges**.
5. Health check: 8721 aristas con endpoint colgante (típico de AST en C++),
   26 self-loops, colapsos MultiDiGraph→Graph esperados. Grafo usable.
6. Build + cluster: **150.629 nodos, 271.464 edges, 4138 comunidades**.
7. Etiquetado: 40 principales curadas a mano, resto por directorio.
8. `graph.html` (agregado por comunidad), `GRAPH_REPORT.md` (14.043 líneas).

## God nodes (top)

`DeviceDispatcher` (1466), `operator<=>` (1378), `DWORD` (1067),
`VULKAN_HPP_NOEXCEPT` (717), `LTRotation` (584), `SplineShape`, `Interface`,
`PatchObject`, `CString`, `CAI`.

## Update posterior (sesión sealhunter/MSVC)

`graphify update` con 1555 archivos cambiados (1548 code + 7 docs):
AST incremental + 1 chunk semántico manual (`.graphify_chunk_upd.json`,
CMakeLists + 3 anims TO2). Reetiquetado por firma de directorio
(`step5b`, estable ante reclusters). Resultado:
**168.140 nodos (+17.511), 299.425 edges, 3912 comunidades**, HTML y reporte
regenerados. Scripts del pipeline viven en `graphify-out/` (se limpian solos
al cerrar cada corrida).
