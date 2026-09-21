# Límites, gaps y riesgos conocidos (sesión 11–14 sep 2026)

## Sandbox: sin X11 local

- El sandbox bloquea sockets `AF_UNIX` (`socket() → EPERM`). Sin Xvfb, sin
  clientes X: **el modo `--window` no se puede probar desde aquí**.
  Verificación de ventana/input/render real = el usuario en su desktop.
- `require_escalated` no sirve con approval bypassed (nadie aprueba).
- `DISPLAY=:0` ausente: no hay X alcanzable.

## Entorno volátil

- `/tmp` es tmpfs: el reboot borró `/tmp/vksw/lvp_local.json`.
  Usar el ICD del sistema: `VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/lvp_icd.json`.
- Cortes `[net-timeout]` queman intentos del proveedor sin progreso visible;
  cada reintento reenvía el contexto acumulado (sesión desde el 11-sep).

## Mundo 3D: no renderiza (gap abierto, no bug)

- `T_RenderCamera` es no-op (`return LT_OK`). Tras Enter: celeste + HUD
  (puntos/dinero) es el comportamiento actual **esperado**.
- Hito pendiente: loader del mundo + BSP + pipeline de mundo + colisiones.
  Grande; no empezado.

## Crash ocasional: sin datos

- Reportado sin log. Candidatos no confirmados: teardown X11, paths
  FindServers/Host, swapchain Intel. **Falta el texto del terminal**.
- El tick in-world corre 60 frames headless sin crash; el menú también.

## Higiene del repo

- `demo-sealhunter/sealhunter/sealhunter.zip` conserva `.exe`/`.dll`
  originales (el árbol está limpio). Decidir si se borra.
- `graphify-output/nolf2-index.json` regenerado localmente, **sin
  commitear** (mezcla churn previo de 95k líneas). `pe_audit.py` obsoleto
  (apunta a binarios ya borrados).
- Directorios `build` vs `build2`: builds en `build/` (ver GUIA-DEMOS.md).
- Untracked ajenos (`Development/`, `Docs/`, duplicados `clientfx/`, `.venv/`):
  no tocar sin orden del usuario.
