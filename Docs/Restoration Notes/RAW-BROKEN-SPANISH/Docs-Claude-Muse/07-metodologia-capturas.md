# 07 — Metodología de capturas para debug (probes + PPM + PNG)

## Idea

Cada subsistema nuevo expone un **probe**: un flag CLI que levanta su
propio renderer headless (u opera el juego normal), dibuja UNA escena
sintética o real, la vuelca a **PPM** y auto-verifica píxeles o contadores.
El probe imprime una línea contrato (`*_RESULT ok=1`) y sale con código 0/1.
Así cada fase se verifica sin ventana, sin input y sin ojo humano en el
loop — la imagen solo se mira para confirmar composición.

## Piezas

- `VulkanRenderer::SnapshotPPM(path)` (`Renderer/vulkan/vk_headless.cpp`):
  graba el framebuffer offscreen a `P6 800x600`. Sin flip (NDC y=-1 es
  arriba, igual que la fila 0 del framebuffer).
- `gen_captures.py` (raíz del repo, stdlib + Pillow): corre los 4 jobs,
  guarda `.ppm` + `.log` y convierte a `.png`, todo en `./temp-captures/`
  (carpeta local, fuera del Temp del sistema para revisarla a mano):

```
--mesh-test                  → temp-captures/r1-mesh.{ppm,png,log}
--world-test                 → temp-captures/r2-world.{ppm,png,log}
--model-test                 → temp-captures/r3-models.{ppm,png,log}
--vulkan --frames=120        → temp-captures/r4-gameplay.{ppm,png,log}
```

Uso: `python gen_captures.py` (acepta `--exe RUTA`). El contrato de éxito
es el mismo de la sesión 1 (`HOST_RESULT ok=1`, `*_RESULT ok=1`).

## Probes y qué verifica cada uno

| Flag | Dibuja | Auto-check |
|---|---|---|
| `--mesh-test` | grid checker + 2 cubos (uno rojo tapado) | `nonClear>50000`, rojo presente pero minoritario (depth OK) |
| `--world-test` | `World.dat` real, cámara overview | `tris>500`, cobertura, `texMiss=0` |
| `--model-test` | seal/bruno/SnowMan/Mallet/HARMGuard en fila | parse OK de los 6, `texMiss=0` |
| (juego) | partida real 120 frames | `HOST_RESULT ok=1`, draws/vktris estables |

## Overrides por entorno (sin recompilar)

- `SEAL_CAM="px,py,pz,tx,ty,tz"` — cámara del `--world-test` (mirar
  cualquier punto del mundo).
- `SEAL_BLOCKS="0,3,5"` — dibuja solo esos bloques del mundo (aislar
  culpables visuales; los índices salen del dump de secciones).

## Lecciones que dejó la metodología

1. Los números primero: `tris/draws/texMiss` dicen SI funciona; la imagen
   dice si se VE bien. Varios bugs dieron `ok=1` con imagen rota.
2. Fijar semillas/posiciones en los probes: las corridas son bit a bit
   comparables (`draws`/`vktris` estables entre corridas; si cambian, algo
   se rompió — contrato heredado de la sesión 1).
3. Ver `09` (baricéntricas con signo roto daban cobertura falsa) y
   `08` (matriz mal traspuesta solo se vio en imagen): la imagen es el
   último juez, nunca el único.
