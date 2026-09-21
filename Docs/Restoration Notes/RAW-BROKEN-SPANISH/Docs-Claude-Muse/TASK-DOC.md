# TASK-DOC — Regenerar un renderer Vulkan para un motor legacy

Destilado de restaurar el gameplay 3D de LithTech Jupiter (SealHunter) en
Vulkan. Aplica a cualquier motor 2000-2005 (Half-Life y mods, LithTech,
id Tech 3, Unreal 2): el juego funciona en APIs legacy, hay que llevarlo
a Vulkan sin reescribir el engine.

## 0. Forma del problema (reconocerla)

- Existe `RenderScene(camara)` → `backend API-legacy` (D3D8/9, OpenGL 1.x).
- El backend legacy **no compila hoy** (D3DX, SDKs muertos, drivers), así
  que "portarlo" no es opción real aunque el código "esté ahí".
- El juego (lógica, IA, GUI, server) **sí funciona**: se puede correr
  headless con stubs y verificarlo antes de tocar un píxel 3D.
- Hay un backend **nulo/dummy** en el árbol: es la plantilla exacta del
  seam (qué funciones hay que implementar y en qué orden se llaman).

## 1. El principio central: no portes el driver, alimenta uno mínimo

Portar `sys/d3d` (decenas de miles de LOC: estados, shaders, lightmaps,
vertex buffers) es reescribir un renderer D3D9. El camino acotado:

1. **Pipeline 3D mínimo nuevo**: vértice `(pos3, uv, color)` + MVP por push
   constants + **depth buffer**. Fullbright primero (sin luz); un solo
   shader de `textura × color`.
2. **Reutilizar los loaders funcionales**: el parseo de mundo/modelos/
   texturas del engine sirve; solo se sustituye el *emit* final D3D por
   push de triángulos al pipeline nuevo.
3. **Fidelidad por capas**: mundo texturizado → modelos estáticos (bind
   pose) → translúcidos/skybox → animación → lightmaps. Cada capa se
   verifica por separado (ver `07-metodologia-capturas.md`).

## 2. Arqueología de formatos (el método que siempre funcionó)

Cada formato binario del engine tiene **DOS implementaciones en el árbol**:
el **packer/writer** (tools) y el **reader** de runtime. El tercero (el
nuestro) se espeja de ambos:

1. Leer el writer (orden exacto de campos) + el reader (tamaños).
2. Implementar el reader mínimo con **cotas en cada lectura** (falla ruidoso,
   nunca lee fuera del buffer).
3. **Validar con contadores en cada etapa**: allocations (piezas, nodos,
   tris, verts), counts anidados, `pos final == tamaño esperado`. Si un
   número no cierra, el parse va mal aunque "parezca" funcionar.
4. Ante disputa de layout (¿qué float es qué?), ground truth contra la
   **fuente ASCII** si existe (`.lta`, `.map`, `.qc`, `.smd`): buscar
   triples exactos bit a bit en el binario para aprender offsets y strides.
5. Formatos de string, packing y enums **se verifican, no se suponen**:
   strings `u16+bytes` vs NUL; structs con padding de plataforma;
   `BPP_32=3` no `0`; `usedNodeListSize` u8 no u32; datos por-LOD vs
   por-piece (leer el loop del reader, no adivinar).

## 3. Catálogo de trampas (casi todas pagadas en esta sesión)

- **Matrices**: verificar convención con un caso concreto (rotación 90° de
  un joint conocido). Row-major × column-major se ve bien en números y
  rompe todo en imagen. Regla: tras tocar matrices, mirar el PNG sí o sí.
- **Baricéntricas**: el `l1` con la variable cambiada (B en vez de C)
  acepta regiones espejadas y da "suelo" falso. Test unitario a mano:
  `A=(0,0),B=(4,0),C=(0,3),P=B → lB=1`.
- **Blends**: orden = huesos **ordenados** (`std::set` del writer) + 4º
  peso implícito `1−suma`; renormalizar solo sobre huesos válidos.
- **Bind pose = posiciones crudas**: los vértices ya están en espacio de
  modelo; el skinning compone `current·invBind` (por eso el engine guarda
  inversas). Skinnear con las globales del fichero duplica transformadas.
- **Texturas**: gemelos `.tga` primero (legibles sin decode), DTX/DXT
  después; cachear por nombre; `texSlot` por mesh (cuerpo ≠ cabeza).
- **Skybox**: vive en coords fijas de autor; hay que recentrarla en cámara
  cada frame (mirar `SkyDef` del engine para el comportamiento).
- **Colocación de objetos**: medir con raycasts en grilla, no adivinar;
  los placements de diseñador (ASCII del mapa) dan XZ, la Y se verifica
  contra el mesh. Sin física, nada "cae a su sitio".
- **MSVC**: `NOMINMAX`, sin VLAs, `u8/u16` exactos en streams, headers
  single-TU (ODR: el juego solo forward-declara, define el host).

## 4. Ladder de verificación (de barato a caro)

1. **Contadores**: bloques/secciones/tris/verts/nodos/`texMiss=0`.
2. **Auto-checks de píxeles** en el probe (cobertura, color dominante,
   oclusión: lo de atrás no tapa lo de adelante).
3. **Imagen**: composición, orientación, texturas correctas, sin spikes.
4. **Estabilidad**: mismos `draws/tris` entre corridas; regresión del 2D
   tras tocar el 3D (mismo pass, distinto pipeline).

## 5. Mapa instancia → Half-Life (ejemplo de transferencia)

| LithTech (esta sesión) | Half-Life / mod | Mismo rol |
|---|---|---|
| `World.dat` renderblocks | `.bsp` (lumps faces/edges/surfedges) | polys + uv + textura por sección |
| `Sky0201…` worldmodel | `skyname` + `env/` TGAs | skybox que sigue cámara |
| `.ltb` + bonesets | `.mdl` studio (bones, seqs, bodyparts) | bind pose primero, anims después |
| `.dtx`/`.tga` | `.wad` + sprites `.spr` | decode propio + cache |
| `GameStartPoint0` en `.lta` | `info_player_start` en `.map`/entidades | XZ de diseño, Y medida |
| `RenderStruct` + backend nulo | `refexport_t` (`r_studio`/`r_bsp` nulos) | el seam a implementar |

El plan R1→R4 se transfiere tal cual: pipeline 3D mínimo → BSP estático →
modelos en bind pose → entidades/gameplay → animación/luz.
