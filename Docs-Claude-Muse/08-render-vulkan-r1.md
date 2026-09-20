# 08 — R1: pipeline 3D en el renderer Vulkan

## Por qué

`VulkanRenderer` era solo 2D (tris planos + quads texturados, sin depth ni
MVP). Todo lo 3D (mundo, modelos) necesitaba un pipeline con profundidad.
Se extendió, sin romper el 2D existente (regresión: mismos
`draws=1560 vktris=2160` en sealhunter headless).

## Cambios (`Renderer/vulkan/`)

- Shaders nuevos `shaders/mesh3d.vert/.frag`: `in vec3 pos + vec2 uv +
  vec4 color`, MVP por **push constants** (`mat4`, 64 B), fragmento
  `textura * color` (fullbright; lightmaps/renderstyles quedan para R4+).
- Embed como `mesh3d_spv.h` (`kMesh3dVertSpv/kMesh3dFragSpv`) con el mismo
  patrón `glslangValidator + embed_spv.py` de `tri_spv.h` (nuevo bloque en
  `CMakeLists.txt`; `static`→miembro para compartir el creador de depth
  entre `vk_renderer.cpp` y `vk_headless.cpp`).
- API: `PushTri3D(tex, v3)`, `SetViewProj(m16 col-mayor)`,
  `PendingMeshTris()`; tercer `kind` (`mesh`) en `DrawItem`/`drawOrdered`
  (replay en orden de submission como los pipelines 2D).
- Depth: `findDepthFormat()` (D32→D24S8→D16), attachment de profundidad en
  el render pass de ventana Y en el offscreen headless, `clearValueCount=2`
  en `Present/RenderWindowFrame/SnapshotPPM`, pipeline mesh con
  `depthTest+depthWrite, LESS, CULL_NONE`.
- Convención NDC del proyecto (verificada en código y en imagen):
  `y=-1` es ARRIBA con viewport normal; la matriz del host ya lleva el flip
  (`P[1][1] = -f`), el shader no niega nada. Rango Z `[0,1]`.

## Matemáticas del host (`Platform/host/host_world.h`, antes en el .cpp)

`matMul4 / buildView4 / buildPersp4 / lookAt4 / vpFromBasis4`, todo
columna-mayor, cámara que mira a lo largo de **+F** (`w = dist-adelante`).
Cámara de juego: base del `OT_CAMERA` del cliente + `fovX=90`.

## Bug que solo la imagen mostró (lección)

Primera versión de `buildView` guardaba la rotación **sin trasponer**
(`o[1]=r.y` en vez de `o[4]=r.y`): 48k píxeles, suelo cortado a la mitad,
caja flotando. Los números (`MESH_RESULT`) no lo delataban; el snapshot sí.
Regla: tras un cambio de matrices, mirar SIEMPRE el PNG aunque el probe dé
`ok=1`.

## Verificación

`--mesh-test`: grid checker 12×12 + cubo texturado delante + cubo rojo
detrás (solo la tapa asoma). `MESH_RESULT ok=1 tris=312 nonClear=126110
red=1356` + `temp-captures/r1-mesh.png` con perspectiva, textura y oclusión
correctas.
