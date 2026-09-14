# Guía: compilar y correr todas las demos (Arch Linux)

## 1. Dependencias

```bash
sudo pacman -S gcc cmake ninja vulkan-loader sdl2 sdl3 freetype2 libx11 libxtst
```

## 2. Compilar todo

```bash
cd nolf2_gpl_src
cmake -B build -DLITH_VULKAN=ON && cmake --build build -j$(nproc)
cmake --build build --target help | grep -E '^\.\.\. '  # lista targets
```

## 3. Dónde quedan los binarios

`build/Samples/Samples/<area>/<nombre>/<target>` (target con `_`).
Demos ejecutables (19): `audio_music`, `audio_sounds`,
`debugging_stacktrace`, `graphics_GuiMgr`, `graphics_bump`,
`graphics_clientfx`, `graphics_drawprim`, `graphics_effects`,
`graphics_fonts`, `graphics_renderdemo`, `graphics_shaders`,
`graphics_specialeffects1`, `graphics_video`, `networking_nettest`,
`networking_sealhunter`, `objects_doors`, `objects_pickups`,
`objects_projectiles` y `vulkan_demo`.

## 4. Correr

**Genéricas** (stub main → WinMain, sin flags):

```bash
./build/Samples/Samples/graphics/fonts/graphics_fonts
# LithTech demo stub main (graphify) -> calling WinMain ; exit 0
```

**Headless Vulkan** (lavapipe; ICD del sistema porque `/tmp` se limpia):

```bash
export VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/lvp_icd.json
./build/Samples/Samples/networking/sealhunter/networking_sealhunter \
  -rez demo-sealhunter/sealhunter/rez --vulkan --frames=60
# HOST_RESULT ok=1 stage=run frames=60 draws=4620 vktris=4680
./build/Samples/Samples/graphics/drawprim/graphics_drawprim \
  --vulkan --frames=60   # HOST_RESULT ok=1
```

**Present real** (sin ventana): agregar `--psurface` →
`PSURFACE_RESULT ok=1 frames=60 presents=60`.

**Ventana interactiva** (requiere display real; imposible en sandbox):

```bash
./build/.../networking_sealhunter -rez demo-sealhunter/sealhunter/rez --window
# SDL3 primero (--x11 fuerza X11 nativo). Flechas/Enter = menú,
# q o cerrar ventana = salir. Ver `08-Demos-Vulkan-Runtime.md`.
```

## 5. Snapshot

`--ppm=/tmp/out.ppm` vuelca el framebuffer headless a PPM.
