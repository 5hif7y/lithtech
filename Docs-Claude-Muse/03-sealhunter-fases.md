# 03 — Fases de gameplay sealhunter

Objetivo "completar": cazar focas a melee en tercera persona. Sin paquetes
nuevos de vcpkg (SDL2+freetype alcanzan; red same-process).

## Fase A ✅ — sshell compila y linkea

- `sshell/src/*.cpp` agregado al target (+ include dir). `BaseClass` viene
  del SDK (`ltengineobjects.h`), el comentario que lo excluía quedó obsoleto.
- Agregados al target: `sdk/inc/iltbaseclass.cpp`,
  `sdk/inc/ltengineobjects.cpp` (implementaciones SDK de
  BaseClass/ClassDef) y `clientfx/Shared/ParsedMsg.cpp` (lo usa
  ServerSpecialFX) + `#include <cctype>` (`toupper`).
- Exe 783 KB, link limpio, headless `ok=1`.

## Fase B ✅ — server tick + spawn de focas

Nuevo `Platform/host/host_server.h` (solo lo incluye `host_sealhunter.cpp`):
- **Tabla de objetos**: `HOBJECT`/`HLOCALOBJ` = punteros opacos a `ServerObj`
  (pos/rot/scale, timers, props, nombre). `SetHOBJECT()` para `m_hObject`.
- **`HostILTServer : ILTServer`**: ~75 overrides (los 30 de ILTServer + ~45
  de ILTCSBase, todos puros) + `apply()` que cablea ~80 function-pointers
  (`GetClass`, `CreateObject`, `GetProp*`, `SetNextUpdate`, `ObjectToHandle`,
  `IntersectSegment`…).
- **Fábrica**: `GetClass` camina `__g_ClassDefinerHead`; `CreateObject`
  construye con `m_ConstructFn`, registra handle y dispara
  `MID_PRECREATE` (WORLDFILE) + `MID_OBJECTCREATED`.
- **Suelo**: `IntersectSegment` contra plano y=0 (sin geometría de mundo).
- **`HostILTCommon`** (24), **`HostILTModel`** (9 + 48 fallbacks base
  generados), **`HostILTSoundMgr`** (30, set EAX20), **`HostServerPhysics`**
  dedicado (8, sin estado compartido con el cliente). Clases cliente del
  SDK (`ILTClientSoundMgr`, `ILTModelClient`, `ILTClientPhysics`) como bases
  donde corresponde; `_InterfaceImplementation` en las 5.
- **Driver** en `host_sealhunter.cpp`: `bindServerInterfaces()` (asigna
  `g_pLTServer/g_pLTSCommon/g_pLTSPhysics/g_pLTSModel/g_pLTSSoundMgr`),
  `CLTServerShell` + `OnServerInitialized/PreStartWorld`, spawn de
  **Seal0/1/2**, `tickServerWorld(1/60)` junto a cada `shell->Update()` y
  línea `SERVER: objects/updateticks/time`.
- Verificado: `SERVER: objects=3 updateticks=180 time=2.00` en 120 frames
  (tick 1×), menú bootea 1754 frames sin crash.

## Fase C (siguiente) — menú funcional

El menú visible ya existe (solo arranca mundo con `StartNormalGame` diferido
al click). Falta: que las opciones normal/host/join disparen el arranque con
server (hoy el server corre siempre de fondo) y alta del jugador
(`OnClientEnterWorld` → `CPlayerSrvr`).

## Interludio — menú con texturas y texto

Las capturas del usuario mostraron el menú sin backdrop ni strings: el
`rezDir` relativo solo resolvía con cwd=root (doble-click = cwd del exe →
todo placeholder, incluido el atlas freetype). Fix en `host_engine.h`:
`resolveAsset()` ahora también busca hacia arriba desde el exe y el cwd
(`<filesystem>`, 8 niveles). Verificado corriendo desde la carpeta del exe:
cero "not found", splash/sealgui vía twins `.tga`. Queda confirmación visual
del usuario.

## Fase D — focas visibles (billboards)

Sin renderer de modelos `.ltb`, las focas se dibujan como quads texturizados
(`tex/seal/seal.tga`) proyectados por la cámara cliente (follow, fovX=90°):
`ObjState::type` (+`OT_CAMERA`), `DrawServerWorld()` en `host_server.h`
(piggyback en el tick: update precede al render del mismo frame), solo con
cliente en mundo. Verificado por log (textura id=8, draws suben); falta
confirmación visual (posición/tamaño/orientación del sprite).

## Fase D — focas visibles

Los objetos server no tienen representación cliente. Camino: billboards vía
`DrawPrim` primero, modelos `.ltb` del rez después.

## Fase E — melee + win/lose + polish

Ataque cliente → daño server → `Die`/`FlopAround` (+ `AIVolume0` y
`DecrementSealCount`), win/lose, sonidos (`Snd/Seal2.wav` existe pero
`PlaySound` está comentado en seal.cpp) y texturas del rez.
