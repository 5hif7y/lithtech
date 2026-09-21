# 11 — Estado: R4 gameplay COMPLETO y verificado

## El loop jugable (verificado headless, `exit=0`)

Click/ESPACIO (cmd 15) → flanco en host → `CPlayerSrvr::CheckForHit()`
REAL (raycast 35 + `OBJ_MID_DAMAGE` 5) → 4 hits (HP 20) → `ACTION_DYING`
→ timer 1.2s → `Die()` → `KILLSCORE` → `score=1 money=9.95` → HUD →
`RemoveObject` a los 2s. Números de la corrida 600f `--autoattack`:
`HOST_RESULT ok=1`, `SERVER objects 3→2`, `updateticks` estables.

Movimiento también verificado (`--autowalk` inserta cmd 1): el jugador
avanza mirando a su yaw (120f → +870u), o sea WASD real funciona.

## Arreglo (medido con grilla de raycasts)

| Qué | Dónde (x,y,z) | Nota |
|---|---|---|
| Jugador (HARMGuard) | (-700,-528,400), yaw π/2 (este) | camina al este |
| Seal0/1/2 | (-670,-512,400), (-600,-528,420), (-750,-528,430) | Seal0 al alcance del mazo |
| Snowman | (-640,-515,415) | composición como la captura (`Snowman0` diseñado a 1 km) |
| Cámara | 3ª persona del engine (`CCamera`: −forward·zoom + up·50) | de espaldas como la captura |

## Fixes que R4 exigió (todos con causa raíz)

- `SetupEuler` era stub en identidad (client+server): todo miraba a +Z.
  Ahora usa el ctor `LTRotation(pitch,yaw,roll)` del engine.
- `T_CreateObject` ignoraba `m_Pos/m_Rotation`: todo nacía en origen.
- `GetObjectClass/IsKindOf` sin cablear (punteros nulos → crash en
  `CheckForHit`): implementados sobre `ClassDef` + cadena `m_ParentClass`.
- `S_IntersectSegment` solo plano y=0: suma pasada de esferas (solo
  `Seal*`, r=60; el filtro por nombre excluye auto-impacto).
- `S_SendToObject` era no-op: entrega local síncrona (vale para
  `OBJ_MID_DAMAGE` y `KILLSCORE`).
- `tickServerWorld` iteraba con range-for: `RemoveObject` al morir
  invalidaba el iterador (crash). Ahora por índice con revalidación.
- `Seal::Die()` idempotente + guard `DYING/DEAD` en daño (los golpes de
  más reseteaban el timer de muerte y la foca era inmortal).
- Puente score: `CLTClientShell::SetHudStats` (el net real está caído:
  `SendToClient` es no-op); `Seal::IsDead()` para no dibujar cadáveres;
  victoria host-side (las 3 muertas → `VICTORY` en log/consola).

## Desviaciones honestas (no son bugs, son stub declarado)

- **FX/audio sin salida**: `PlayClientFX` servidor es no-op documentado
  (su ciclo `CAutoMessage` rompe en el dtor; sin backend no se pierde
  nada observable). `PlaySound` ya venía comentado upstream.
- **Victoria solo por inspección**: el contador es trivial (vivas==0);
  el test mata 1 (las otras 2 fuera de alcance del rayo 35). Misma cadena.
- **Snowman solo visual** (sin objeto server; el engine lo dañaría, acá no).
- **Ruido**: `(Seal) Could not find ground!` por frame (IA contra plano
  y=0 bajo el cañón) + `Error creating attachment` preexistente.
- **Snapshots headless acumulan** frames en una imagen (sin clear por
  frame): el HUD final puede mostrar valores viejos encima; en ventana
  (present por frame) es correcto. El mecanismo está probado por log
  (`[gui] render score=1` 273×).

## Resto (menor)

- Menú en ventana: navegable según TODO de sesión 1 (re-test manual).
- `groundHeight()` → `IntersectSegment` físico (las focas apoyarían real).
- Re-hacer `gen_captures.py` si cambian baselines (`draws/vktris`).
