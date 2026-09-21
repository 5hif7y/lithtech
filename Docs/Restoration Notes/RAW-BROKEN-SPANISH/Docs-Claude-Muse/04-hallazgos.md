# 04 — Hallazgos, daño preexistente y riesgos

## Daño preexistente del árbol (no causado por este trabajo)

**1907 headers con contenido = solo su nombre de archivo.** Origen: el fix
case-sensitive de Linux convirtió headers a symlinks; en Windows quedaron
como texto. Dos clases:
- **1541 archivos regulares pisados** (ej. `BaseFx.h`, `ClientFXDB.h`) →
  **restaurados con `git checkout`** (contenido = HEAD, verificado).
- **366 symlinks** (`Bouncychunkfx.h` y cía) → **no tocados**; varios ya
  estaban materializados con contenido real (precedente válido). Si un build
  MSVC los necesita, se materializan on-demand copiando el target.
- 2 falsos positivos con contenido real (`Rnginterstitialclient.h`, un
  `Stdafx.h`) se dejaron intactos.

⚠️ Discrepancia no explicada: `git status` pasó de 3880 a ~5040 modificados
durante el restore. Composición final verificada por muestreo (restaurados
limpios + symlinks + materializados + 6 edits propios), pero el delta del
contador no se pudo conciliar. Ninguna acción tocó rutas fuera de las
listadas en `02-msvc-port.md` y `03-sealhunter-fases.md`.

## Quirks conocidos

- **Paradoja `SDL_CreateWindow(VULKAN)`**: falla en el exe, funciona en un
  test idéntico (misma DLL, mismos flags). Bypasseada con superficie nativa;
  sin impacto funcional.
- **Doble tick**: dos edits solapados duplicaron `tickServerWorld` en 4
  loops (server corría 2×). Detectado por `time=58.47` vs frames y corregido;
  verificado 1× (`time=2.00` en 120 frames).
- **Salida limpia del menú**: en boot de menú sin input el exe sale solo con
  exit 0 (~1750 frames). Causa no identificada; no bloquea (Fase C lo maneja).
- **LIB con `git2.lib`**: la entrada de vcpkg en `LIB` apunta a un archivo
  `.lib`, no al directorio (inofensivo para CMake, molesto para `cl` manual:
  pasar la ruta completa de `SDL2.lib`).

## Riesgos para Linux (validar en próximo build)

Cambios con guarda `_WIN32`/`MSVC`/CMake-WIN32 no afectan Linux. Puntos a
mirar igual: fallbacks `ILTPhysics::` (duplicarían símbolo si Linux los
resolviera de otro lado — no se encontró otra definición), `bool
operator==`, headers restaurados (= HEAD, solo pueden mejorar),
`SDL_MAIN_HANDLED` está tras `if(WIN32)`.
