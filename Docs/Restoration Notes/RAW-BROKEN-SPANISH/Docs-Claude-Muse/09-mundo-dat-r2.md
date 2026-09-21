# 09 — R2: lector de la sección render de World.dat

## Formato (espejado del writer + reader D3D, ambos en el árbol)

- `World.dat`: `versión(u32=85)` + **6 markers** `CFileMarker` (offsets a
  objetos, blind, lightgrid, física, partículas, **render = slot 5**,
  `+24`) + infostring + posMin/Max + worldOffset + WorldTree + submundos +
  … + sección render en `slot[5]=309614`.
- `CD3D_RenderWorld::Load`: `nBloques` + bloques + `FixupChildren` +
  worldmodels recursivos (nombre + sub-mundo).
- `CD3D_RenderBlock::Load`, por bloque: centro/half + secciones
  `[tex0,tex1(str u16+bytes), shader u8, nTris, effect str, lightmap
  w/h/size+skip]` + vértices crudos **`SRBVertex` 68 B**
  (`pos + uv0 + uv1(skip) + color + normal/tangente/binormal(skip)`) +
  tris `[3×u32 idx + u32 polyIdx(skip)]` + skyportals/oclusores/lightgroups
  (skip con su formato) + `childFlags u8 + 2×u32 hijos`.
- Secciones con `tex0` vacía se saltan (12 tris); sprites (`.spr`) se
  cuentan aparte (0 en este mapa).

## Código (`Platform/host/host_world.h`, header-only como `host_server.h`)

`DatReader` LE con cotas, `parseRenderBlock/parseRenderWorld`,
`drawWorldMesh()` (push al pipeline R1, color de vértice o fullbright),
`vpFromBasis4`, `groundHeight()` (baricéntricas 2D en XZ, máx-Y),
`worldStore()/worldReady()/worldTried()` (carga única compartida),
`decodeDTX()` (ver abajo), `lookAt4`.

## Texturas DTX (`decodeDTX`)

Header 164 B, `m_Extra[2]` (= byte 26) es `BPPIdent`
(3=BPP_32, 4/5/6=DXT1/3/5 — ¡no 0/1/3/5!). Solo mip base:
BPP_32 como **BGRA** (convención D3D) → RGBA; DXT1/3/5 con decode propio
(~120 líneas, paletas + alfa explícito/interpolado). Fallback a gemelo
`.tga` cuando existe. `resolveWorldTex()` prueba
`base/nombre`, `+.dtx`, `Tex/<base>.dtx` (+ gemelos `.tga`); cache por nombre.
Resultado: 23/23 secciones con textura (`texMiss=0`): nieves `tfsib*`,
hielo `Ice01`, piedra `st0873`, cielo `Sky2_*`, muros translúcidos.

## Skybox que sigue a la cámara

El worldmodel `Sky0201.DemoSkyWorldModel0` es un cubo 248³ en coords fijas
de autor. Como el engine (`SkyDef`), `drawWorldMesh` traslada sus bloques
(detectados por prefijo `TexFX`) a `campos - centro`. Sin esto, cielo negro.

## Lección: cobertura falsa por signo (leer antes de fiarse de un query)

El primer `groundHeight`/`query.py` aceptaba puntos fuera del triángulo por
un `l1` con signo roto; "encontraba" suelo (213/226/…) donde el C++
(correcto) decía hueco. La forma correcta (Ericson):

```
d  = (z2-z3)(x1-x3) + (x3-x2)(z1-z3)
lA = ((z2-z3)(x-x3) + (x3-x2)(z-z3)) / d
lB = ((z3-z1)(x-x3) + (x1-x3)(z-z3)) / d   # ¡con C, no con B!
```

Verificado a mano con `A=(0,0),B=(4,0),C=(0,3),P=B → lB=1`. El mapa de
suelo válido se re-midió con `dbg_grid.py` (raycasts en grilla) y mandó el
diseño del campo de juego (ver 11).

## Verificación

`--world-test`: `blocks=10 sections=24 tris=4115`, bbox real del nivel,
`WORLD_RESULT ok=1` + `temp-captures/r2-world.png` (cañón de hielo con
cielo, y con `SEAL_CAM` se inspecciona cualquier punto).
