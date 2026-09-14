# TODO — tareas faltantes (14 sep 2026)

## Funcional (requiere display real del usuario)

- [ ] Retest `--window`: flechas/Enter en menú + `HOST: entered world` tras Enter.
- [ ] Pasar texto del crash ocasional (terminal completo tras el corte).
- [ ] Tuning fino de input in-game (sensibilidades, repetición).

## Hitos grandes

- [ ] **Mundo 3D**: loader + BSP + pipeline de mundo + colisiones
      (`T_RenderCamera` hoy es no-op; celeste+HUD es lo esperado).
- [ ] Port completo Engine (`LITH_ENGINE_FULL=ON`; exige sed + math_shim).
- [ ] Harness mínimo de tests (repo no tiene; hoy solo probes + líneas RESULT).

## Higiene / decisiones del usuario

- [ ] Borrar `demo-sealhunter/sealhunter/sealhunter.zip` (conserva .exe/.dll) ¿sí/no?
- [ ] Commitear `graphify-output/nolf2-index.json` regenerado ¿sí/no?
- [ ] Re-smoke de las 19 demos tras cambios futuros (hoy solo 3 con probe fresco).
- [ ] `pe_audit.py` obsoleto (apunta a binarios borrados): borrar o actualizar.
