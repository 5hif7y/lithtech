# 06 — Recuperación de sesión y fix del crash de gameplay

Segunda época de trabajo. La sesión `289d5607-…` quedó inutilizada por
incompatibilidad planner Muse Spark 1.3 / Claude Code; su hueco se rellenó
con `00-origen-tarea.md` y `05-inventario-sesion-1.md`.

## Punto de partida

- Diff de la sesión anterior: `gitdiff-sesion1-289d5607-….diff` (28 MB,
  modernización CMake multiplataforma: GCC en Unix-likes, MSVC en Windows).
- Commit base: `2f170a45` ("todo funciona menos el gameplay de sealhunter").
- Objetivo del usuario: terminar de restaurar el gameplay hasta que se vea
  como `captures/captura-gameplay-original.png` (tercera persona en nieve
  con mazo, muñeco de nieve, focas, HUD foca + dinero).
- Grafo graphify ya construido (`graphify-out/`): todas las lecturas se
  orientaron primero con `graphify query`, según `CLAUDE.md`.

## Estado heredado (verificado, no supuesto)

- Demo `networking_sealhunter` compila y linkea en MSVC (`build-msvc/`).
- Menú bootea (~1754 frames sin crash), texturas TGA + atlas freetype cargan.
- Server in-process hace tick (3 focas `Seal0/1/2`, `updateticks=188`).
- Docs de la sesión 1 en `Docs-Claude-Muse/02-msvc-port.md` y
  `03-sealhunter-fases.md` (fases A/B hechas: sshell + server tick).

## Hallazgo 1 (bloqueante): crash al entrar al mundo

Headless `--vulkan --frames=120` moría con `0xC0000005` en el primer frame
in-world. Stack con `cdb`:

```
CGui::SetScoreText → CGui::Render → CLTClientShell::Render → Update → main
```

Causa: el constructor `CGui::CGui()` (ambas copias, `demo-sealhunter/…` y
`Samples/Samples/networking/sealhunter/…`) no inicializaba
`m_pScoreString` ni `m_pMoneyString`. La sesión 1 había agregado
`EnterGameUI() → ShowInGame()` (pantalla `SCREEN_INGAME`), que expuso el
bug: `Init()` salta la creación si el puntero no es `NULL`, y `Render()`
desreferenciaba basura.

Fix mínimo (2 líneas por archivo, en el init-list del constructor):

```cpp
m_pScoreString(NULL),
m_pMoneyString(NULL),
```

Verificación: `HOST_RESULT ok=1 stage=run frames=120 draws=1560 vktris=2160`,
`SERVER: objects=3 updateticks=188`. Snapshot `seal_now.ppm`: HUD
(foca + `$0.00`) idéntico en layout al original, 2 billboards de foca,
fondo negro (sin mundo todavía → motiva R1/R2).

## Decisión de alcance

Se propuso al usuario elegir entre billboards jugables / solo menú /
renderer 3D completo. El usuario ordenó **recuperar la renderización
completa en Vulkan** ("el código es funcional, solo hay que completarlo").
El análisis honesto del seam (`CClientMgr::Render → RenderStruct::RenderScene`,
único backend real `sys/d3d`, 160 archivos/38k LOC D3D9, sin D3DX para
compilarlo) llevó al plan R1→R4 documentado en `08…11`.
