# OPTIMIZED Way For Porters — modernizar motores legacy con mínimos tokens

Cómo hacer este mismo trabajo (renderer Vulkan en engine 2000-2005)
gastando lo menos posible y sin prueba-y-error generalizado. Cada regla
nace de un costo real pagado en la restauración de LithTech/SealHunter.

## 0. Principio: el trial-and-error es un impuesto al feedback lento

Cada ciclo `hipótesis → build (minutos) → run → mirar PNG (~1.5k tokens
por imagen)` cuesta miles de tokens. Se optimiza en dos frentes: (a)
abaratar cada ciclo, (b) necesitar menos ciclos (análisis que cierra
antes de codificar).

## 1. Antes de tocar código (ahorra el 60%)

1. **Inventario con conteos, no con lecturas**: `apply --stat` del diff,
   `grep -c`, globs. Saber que son "4310 headers + 654 demos + 10
   archivos de verdad" evita leer 5000 archivos.
2. **Esqueleto primero**: extraer solo los turnos/decisiones del chat
   (un script que liste `Usuario N :: primera línea`), leer el cuerpo
   solo de lo no documentado.
3. **Doble implementación como spec**: todo formato binario se espeja del
   writer (tools) + reader (runtime) del árbol. No se escribe una línea
   del reader propio sin tener la tabla de campos completa.
4. **Fijar la enumeración y nombres de docs al inicio**: renombrar
   después (`N2-*` → `00-11`) + cazar referencias cuesta una pasada
   entera. Decidir `NN-tema.md` una vez.

## 2. Validar barato antes de compilar (ahorra rebuilds)

1. **Réplica en Python del parse**: caminar el binario con el layout
   hipotetizado y exigir que **cada contador cierre** (allocs, tri counts,
   `pos final == tamaño`). Un desvío de 6 bytes invalida la hipótesis —
   no se parcha, se re-lee el spec.
2. **Ground truth ASCII**: si existe fuente legible (`.lta/.map/.qc`),
   buscar triples exactos bit a bit para fijar offsets, strides y orden
   de campos. Una coincidencia de 12 bytes no es casualidad; tres, ley.
3. **Tests unitarios a mano para matemática**: baricéntrica con
   `A=(0,0),B=(4,0),C=(0,3),P=B → lB=1`; matriz con rotación 90° de un
   joint conocido. Lo que no se testea, se espeja mal (signos, traspuestas).
4. **Medir, no adivinar**: grilla de raycasts para placements; overrides
   por variable de entorno (`SEAL_CAM`, `SEAL_BLOCKS`) para inspeccionar
   **sin recompilar**. Cada knob que evita un rebuild ahorra un ciclo.

## 3. Codificar una vez (ahorra re-lecturas y re-verificaciones)

1. **Leer una vez, anclar el edit**: Edit exige haber leído; releer para
   "verificar" es tirar tokens (la herramienta ya confirma). Guardar
   offsets/líneas en la primera lectura.
2. **Llamadas en paralelo**: lo independiente va en el mismo bloque
   (Greps, lecturas, builds no dependientes). Nunca secuencial por defecto.
3. **Probes con auto-check numérico**: cada fase expone `*_RESULT ok=1`
   con umbrales (cobertura, conteos, oclusión). Los números deciden si se
   sigue; **la imagen solo se mira para composición**, una vez por fase,
   no por iteración.
4. **No re-verificar lo verde**: ledger de `draws/tris` estables; si no
   cambió el código del área, no se re-corre su probe.
5. **Regla graphify del repo**: `query` antes que `grep/read`, y
   `--update` tras cambios grandes — un grafo fresco evita re-explorar
   lo ya mapeado (fue la queja U82/U84: costo no lineal por re-trabajo).

## 4. Documentar al final, una vez (ahorra la pasada de renombres)

1. Escribir los docs con **numeración final** desde el principio.
2. Un doc por fase con el mismo esqueleto: formato-espejado-de-dónde /
   decisiones / números de verificación / lección. Nada de re-escrituras.
3. Generalizar al final (`TASK-DOC`/`TASK-PROMPT`), no durante: destilar
   con el trabajo terminado a la vista es una pasada, no cinco.

## 5. Anti-patrones vistos (no repetir)

- Adivinar placements y corregir por screenshot (→ medir en grilla primero).
- Ver 10 PNGs intermedios (→ un PNG por fase + asserts numéricos).
- One-liners Python con quoting hell en PowerShell (→ scripts a archivo).
- Leer `CONVERSACION.md` entera (125 KB) en vez de su esqueleto.
- Preguntas de alcance mal enmarcadas (una ronda perdida): dardefault
  recomendado + costo de cada opción.
- Rebuilding por cada tweak visual (→ knobs por entorno).
