# DIGESTED-PROMPT — restauración de código legacy (versión madura)

Prompt digerido de la sesión 11–14 sep 2026. Objetivo: mismo trabajo con
menos divague y menos tokens/cómputo.

## PROMPT

> Porta este árbol legacy Windows (MSVC + DirectX) a Linux gcc/g++ con
> CMake y renderer Vulkan. Restricciones: no borrar/modificar nada fuera
> de lo pedido; cada cambio se verifica ejecutando (probe o suite del
> repo); cada turno termina commiteado local o con el bloqueador citado.
> Trabaja por hitos chicos y medibles; ante un error, reproduce primero
> contra el código real y corrige la causa raíz, no el síntoma.

## Órdenes operativas

1. **Evidencia antes que síntesis**: lee archivos y corre comandos; el
   código manda sobre resúmenes y memoria. Re-verifica barato antes de
   afirmar.
2. **Un cambio, una verificación**: implementa lo mínimo que cierre el
   hito; corre el probe del área tocada + `HOST_RESULT`/`BUILD ok`;
   recién ahí commit.
3. **Tokens**: prohibido `find /`, `ls -R`, leer logs enteros o JSONs de
   MB. Usa `muse.search` acotado, `grep` con `head`, slices
   (`sed -n`, `offset/limit`), y el índice graphify por consultas
   puntuales (stats, top tokens, aristas), nunca entero.
4. **Sin frameworks nuevos**: si el repo no tiene harness, verifícate con
   sus propios probes/binarios; no agregues suites ni dependencias.
5. **Pregunta solo lo bloqueante**: decisiones de producto del usuario
   (borrar archivos, scope de hitos). Lo técnico reversible, decidirlo.
6. **-reporta**: resultado primero, comandos que lo prueban, qué quedó
   pendiente y qué dato falta del usuario. Sin relato del proceso.
7. **Entorno**: asume `/tmp` volátil y sandbox sin X; paths del
   sistema (ICD Vulkan) sobre `/tmp`. Si algo no se puede probar local,
   se marca NON VERIFICADO y se pide el dato, no se inventa.
8. **Costo**: turnos cortos y autocontenidos; cada mensaje nuevo debe
   poder funcionar standalone. Nada de re-verificar lo ya verde salvo
   que el código haya cambiado.
