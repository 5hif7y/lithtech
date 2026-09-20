# TASK-PROMPT — Prompt de trabajo: regenerar renderer Vulkan en motor legacy

Pegar esto (completando lo marcado con `[…]`) para arrancar la misma tarea
en otro engine (Half-Life/mods, LithTech, id Tech 3…). Asume repo con
código + assets originales y toolchain moderna (CMake + MSVC/GCC).

---

Actúa como ingeniero de render especializado en motores legacy. Objetivo:
**[JUEGO/MOD] corriendo en Vulkan con el gameplay restaurado hasta verse
como `[captura-objetivo.png]`**. El código del juego funciona en APIs
legacy; solo hay que completarlo en Vulkan. No reescribas el engine.

## Reglas de trabajo (no negociables)

1. **Graphify primero** (si existe `graphify-out/`): cada pregunta de
   arquitectura, llamada o dependencia se orienta con
   `graphify query` antes de leer fuentes. Si el fuente contradice al
   grafo, manda el fuente.
2. **Herramienta mínima**: entiende la arquitectura, identifica los
   archivos afectados, inspecciona, haz el cambio coherente más pequeño,
   verifica. Nada de reescribir subsistemas.
3. **Prohibido fiarse de números sin imagen** (y viceversa): todo cambio
   de render se verifica con contadores Y con captura.
4. Commits/PRs solo cuando se pidan. Confirmar antes de borrar/sobrescribir.

## Fase 0 — Orientación (salida: mapa + plan R1→R4)

1. Localiza el seam: `RenderScene(cámara)` → backend legacy. Encuentra el
   backend **nulo/dummy**: es la plantilla de qué implementar.
2. Mide el backend legacy (archivos/LOC): si es un driver D3D/GL completo,
   **NO se porta**; se escribe un pipeline 3D mínimo nuevo
   (`pos3+uv+color`, MVP por push constants, depth, fullbright).
3. Inventaría assets: mundo compilado (`[World.dat/.bsp]`), modelos
   (`[.ltb/.mdl]`), texturas (`[.dtx/.wad]`), placements ASCII
   (`[.lta/.map]`) si existen.
4. Entrega el plan por fases con criterios de salida (usa R1→R4 de
   `TASK-DOC.md` como esqueleto).

## Fases de ejecución (cada una: implementar → probe → contadores → PNG)

- **R1 — pipeline 3D**: shaders + depth + MVP. Probe sintético
  (grid + cubo ocluido). Contrato: `ok=1` + imagen con perspectiva y depth
  correctos + **regresión del 2D idéntica**.
- **R2 — mundo estático**: reader mínimo espejado del writer+reader del
  árbol (con cotas y validación por contadores en cada etapa). Texturas con
  decode propio + fallback a gemelos legibles. Skybox recentrada en cámara.
- **R3 — modelos en bind pose**: parse espejado; **posiciones crudas, sin
  skinning** (verificar contra la fuente ASCII si la hay); skins por slot;
  identificar el modelo del jugador desde el código que lo spawnea (no
  adivinar por nombre).
- **R4 — gameplay**: entidades del server en posiciones medidas con
  raycasts en grilla (nunca adivinadas), cámara del engine, HUD existente,
  melee/daño/score, sonidos. Lo visual ya probado se congela.

## Metodología de capturas (obligatoria desde R1)

Cada fase expone un probe CLI headless que vuelca `PPM` (readback
offscreen) y se auto-verifica (`*_RESULT ok=1`, exit code). Un script
(`gen_captures.py`) corre todos los probes y convierte a PNG en
`./temp-captures/` con sus logs. Overrides por entorno para inspeccionar
sin recompilar (cámara libre, aislamiento de bloques/lumps). Ver
`07-metodologia-capturas.md` como referencia implementada.

## Trampas a vigilar (checklist antes de declarar cada fase OK)

- [ ] Convención de matrices verificada con caso concreto + PNG mirado.
- [ ] Baricéntricas/raycasts testeados a mano (triángulo conocido).
- [ ] Tamaños de stream (`u8/u16/u32`, strings, packing) espejados del
      writer, no supuestos; counts validados contra allocations.
- [ ] `texMiss=0` (toda sección dibujada tiene textura resuelta).
- [ ] Contadores estables entre corridas; `ok=1` en los 4 probes.
- [ ] Documentado en `Docs-Claude-Muse/` (qué se hizo, formato
      espejado de dónde, qué se verificó y con qué números).

## Cierre de fase

Línea `*_RESULT ok=1` + PNG en `temp-captures/` + entrada en el índice de
docs. Si algo se ve mal aunque los números cierren, la imagen manda: vuelve
a la capa anterior, no parchees hacia adelante.
