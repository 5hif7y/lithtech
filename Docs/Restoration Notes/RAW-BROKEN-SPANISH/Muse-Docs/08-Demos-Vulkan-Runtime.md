# Demos Vulkan en runtime — lo no documentado (sesión 11–14 sep 2026)

Cubre lo posterior a `07-Plan-Ejecucion-y-Cierre.md`: las demos ya no solo
compilan, **corren** de verdad en Vulkan (commits `2df132a`, `914b5b8`,
`f833c0d`, `978f188`, `6dbfc9a`).

## Probes verificados (lavapipe, `VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/lvp_icd.json`)

```bash
./build/Samples/Samples/networking/sealhunter/networking_sealhunter \
  -rez demo-sealhunter/sealhunter/rez --vulkan --frames=60
# HOST_RESULT ok=1 stage=run frames=60 draws=4620 vktris=4680
./build/.../networking_sealhunter -rez demo-sealhunter/sealhunter/rez \
  --psurface --frames=60 --vulkan
# PSURFACE_RESULT ok=1 frames=60 draws=4620 presents=60
./build/Samples/Samples/graphics/drawprim/graphics_drawprim --vulkan --frames=60
# HOST_RESULT ok=1 ... (drawprim/bump sin cambios)
```

Contrato de líneas resultado: `HOST_RESULT ok=1`, `PSURFACE_RESULT ok=1`,
`WINDOW_RESULT ok=1`. `draws`/`vktris` estables entre corridas (4620/4680);
si cambian, algo se rompió.

## Modo ventana (`--window`)

- **SDL3 primero, X11 nativo como fallback** (`Platform/host/host_sdl3.*`).
  SDL3 vive en su propio TU: headers SDL2 y SDL3 no pueden mezclarse en un
  `.cpp` (el renderer ya arrastra SDL2 vía `vk_renderer.h`).
  `--x11` fuerza el fallback. Sin display: `sdl3 no disponible` → fallback →
  `WINDOW_RESULT ok=0` limpio, sin crash.
- **Foco**: hints WM + `XSetInputFocus` solo en `MapNotify`/clic. Nunca
  llamar justo tras `XMapWindow` (ventana aún no viewable → el servidor
  mata el proceso con `BadMatch`, caso real visto).
- Cada tecla loguea `HOST: key sym=... vk=... cmd=...`; útil para diagnóstico.

## Menú e input

- `--window` sin `--frames` arranca en el menú (modo `LOCAL_GAMEMODE_NONE`,
  como `run.bat`): `OnCommandOn` solo reenvía teclas al menú en ese modo.
  Auto-arrancar el juego mataba todo el input (causa real del menú muerto).
- Comandos: flechas = 1/2/3/4, Enter = 18, Shoot = 15 (`commandids.h`).
  Keycodes SDL2/SDL3 idénticos (verificado por probe); mapa X11 en
  `Host::mapKeysym`.
- Puente de entrada al mundo (`978f188`): `T_StartGame` levanta
  `worldStartRequested`; cada loop lo consume una vez y llama
  `shell->OnEnterWorld()` (imprime `HOST: entered world`). Sin esto,
  `m_bInWorld` quedaba falso tras elegir del menú y el tick/PollInput
  nunca corrían. El path auto-start lo consume explícito (sin doble entrada).

## Render

- Imagen upright: screen-top → NDC `y=-1`, filas PPM directas.
- `drawOrdered` reejecuta en orden de submission con guard
  `m_order.empty()` (Vulkan no ordena entre pipelines como GL/DX).
- Texto real: TGA twins + atlas freetype; fallback = quad blanco cuando no
  hay atlas.

## Evaluación SDL2 vs SDL3 (conclusa)

- sdl2-compat **no sirve**: `No available video device` incluso bajo Xvfb
  vivo. El `sdl2` real ya no existe en repos Arch.
- SDL3 real (`sdl3 3.4.12`) trae `wayland,x11,kmsdrm,offscreen,dummy,evdev`
  (verificado con `SDL_GetNumVideoDrivers`). Por eso se migró a SDL3.
