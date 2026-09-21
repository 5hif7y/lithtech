# 10 — R3: lector .ltb (bind pose) + skinning diferido

## Formato (espejado writer `tools/Model_Packer/lta2ltb_d3d.cpp` + readers)

`LTB_Header` 20 B (`fileType=1` D3D, `ver=9`) + `fileVersion u32` (23/25) +
15×u32 `allocs` (cantidades para validar cada etapa) + command-string +
`visRadius` + OBBs (`68 B c/u si fv>=24`, si no 64) + pieces + nodos +
weightsets + childmodels. **Se para antes de anims/sockets/bindings**
(no hacen falta para bind pose).

- Piece: nombre + nLODs + dists + 2×u32 tmp; por LOD: `nTex + 4×i32` +
  renderstyle + prio u8 + `render_object_type` (4=rígido, 5=skel, 6=VA) +
  blob (`iObjSize` + datos; el cursor se ancla SIEMPRE a `blobEnd`, lo que
  tolera tipos futuros).
- **OJO (bug real encontrado)**: `usedNodeListSize` es **u8 y va DENTRO del
  loop de LODs** (`ModelPiece::Load`), no una vez por piece. Con 1 LOD
  (seal) el error se enmascara; HARMGuard (3 LODs) lo delató (6 bytes de
  deriva).
- Mesh skel RD: `vc,pc,mbt,mbv + reindexed u8 + streamflags[4] + useMP u8`;
  MP agrega `min/maxBone (+ lista si reindexed)`; luego streams, índices
  u16, y solo RD trae `bonesets [u16 first,count + u8 bones[4] + u32 idx]`.
- Stream (1 solo en estos modelos, flags `0x13` = POS|NORM|UV1): orden de
  escritura del packer **[x,y,z, blends, normal, uv]** con tamaño de
  `GetVertexFlags_and_Size` (B3 no-indexado = 44 B). Los blends van en orden
  de hueso **ordenado** (`std::set` del writer) + 4º implícito `1−suma`.
- Nodos: `nombre + idx u16 + flags u8 + 16 floats GLOBALES + nHijos`
  (recursivo). Las matrices del fichero son la pose de bind tal cual.

## Decisiones clave (validadas contra `seal.lta` fuente)

1. **Bind pose = posiciones crudas, sin skinning.** Los vértices ya están
   en espacio de modelo; el skinning real compone `current·invBind` (por
   eso existe `m_mInvGlobalTransform`). Intentar skinear con las globales
   del fichero duplicó la altura del snowman (233→443). `cookModelMesh`
   copia directo; `skinVert` queda reservado para animación futura (R4+).
   Prueba: SnowMan pasó de "púa" a 3 esferas + galera + zanahoria.
2. **Texturas por slot, no por modelo**: `m_iTextures[]` indexa skins;
   HARMGuard trae `body1(slot 0)` + `eye(slot 1)`. El vivo resuelve
   `ModelTextures/HARMPurple.dtx` + `HARMHeadW1.dtx` por mesh.
3. **El jugador es HARMGuard**, no bruno: `CPlayerClnt::CreatePlayer`
   pide `HARMGuard.ltb + playerbase.ltb` con skins `HARMPurple +
   HARMHeadW1` (el mono morado de la captura). `playerbase.ltb` declara
   `pieces=0` (esqueleto/anims sin geometría propia): se acepta y no se
   dibuja. bruno era pista falsa.
4. `T_CreateObject` ignoraba `m_Pos/m_Rotation` (todo nacía en origen):
   ahora los copia al `ObjState` — sin esto el override de spawn no servía.

## Puentes juego→host (mínimos, marcados `R3-live`)

- `Host::notePlayer()` (decl. en `host_engine.h`, def. única en
  `host_sealhunter.cpp` — el header es single-TU, incluirlo en el juego
  rompía el link `LNK2005`; el juego solo forward-declara).
- `CPlayerClnt::Update` publica `(pos, yaw)` cada frame; `CreatePlayer`
  reubica el spawn de `(0,0,0)` al campo de juego.
- `ServerOnClientEnterWorld` teleporta el `CPlayerSrvr` al mismo punto
  (se busca su `ServerObj` por puntero).
- `drawModelsFrame()`: focas en posiciones del server (+ flop procedural
  `pitch=sin(t·5+i·2.1)·0.3`), snowman fijo, HARMGuard en el jugador,
  Mallet aproximado a la mano (`+up 32, pitch 0.35`).

## Verificación

`--model-test`: seal/bruno/SnowMan/Mallet/HARMGuard OK
(`totalTris=5698 texMiss=0`; playerbase: 0 meshes, ok),
`MODEL_RESULT ok=1` + `temp-captures/r3-models.png` (los 6 uno al lado
del otro, texturas correctas).
