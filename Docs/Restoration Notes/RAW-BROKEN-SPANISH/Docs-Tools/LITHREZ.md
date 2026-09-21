# LITHREZ — empaquetador/desempaquetador `.rez`

Herramienta de línea de comandos para crear (`c`), ver (`v`) y extraer (`x`)
archivos de recursos `.rez` del engine LithTech (NOLF, NOLF2, Shogo, Blood 2, etc.).

- Binario: `LithRez.exe` (nombre de salida preservado del legacy).
- Fuente: `Engine/Engine/tools/LithRez/lithrez.cpp` (solo ese `.cpp`; `stdafx.h` vacío).
- Librería: `Engine/Engine/libs/rezmgr` (`rezmgr.cpp`, `rezfile.cpp`, `rezhash.cpp`, `rezutils.cpp`).
- Toolchain legacy (`LithRez.vcproj`, `Lib_RezMgr.vcproj`, `Tools.sln`) **intacto**: todo lo
  nuevo vive en `CMakeLists.txt` + `tdguard_stub.cpp`.

## 1. Uso

```
LITHREZ <comandos> <archivo.rez> [parámetros]

Comandos: c <rez> <dir raíz> [exts]   - Create  (carpeta -> .rez)
          v <rez>                     - View    (lista contenido)
          x <rez> <dir destino>       - Extract (.rez -> carpeta)
Opciones: v - Verbose      z - Warn zero len      l - Lower case ok
```

Los comandos y opciones **se combinan letra por letra** en un solo string
(`IsCommandSet`, `rezutils.cpp`): `cv` = create + verbose, `xl` = extract
aceptando minúsculas, etc.

```bat
:: empaquetar (pack): carpeta -> .rez (todas las extensiones por defecto *.*)
LithRez c game.rez C:\assets

:: empaquetar solo ciertos tipos
LithRez cv game.rez C:\assets *.ltb;*.dat;*.dtx

:: ver contenido
LithRez v game.rez

:: desempaquetar (unpack): .rez -> carpeta (se crea si no existe)
LithRez x game.rez C:\salida
```

Detalle de `c`: el 4.º parámetro es el filtro de extensiones (`*.*` por defecto).
Recorre el árbol con `_findfirst/_findnext` (`TransferDir`); cada archivo se guarda
como ítem con `Nombre` (mayúsculas salvo `l`), `Tipo` (extensión, 4 chars),
`ID` (el número si el nombre es numérico, si no `10000000+n`) y `Time` = **mtime
del archivo en disco**. Los subdirectorios se replican como `DirectoryEntry`.

Detalle de `x`: vuelca cada ítem como `<Nombre>.<Tipo>` recreando el árbol.
**No restaura los mtimes originales**: los archivos extraídos quedan con fecha "ahora".

Protección `CheckLithHeader` (`rezutils.cpp`, activa porque `lithrez.cpp` pasa
`bLithRez = TRUE`): ver/extraer exige el user-title `"LithTech Resource File"` o
archivos conocidos (`cshell.dll`, `cres.dll`, `sres.dll`, `object.lto`, `patch.txt`,
`sounds\dirtypesounds.`, `blood2.dep`, `riot.dep`). Si no, `ERROR! Not a LithTech
resource file!`.

## 2. Compilar vía CMake

Requisitos: Visual Studio 18 2026 (MSVC x64), CMake ≥ 3.27, vcpkg con `zlib` y
`sdl2` (triplet `x64-windows`). El `.lib` original `TdGuard.lib` (DRM de Touchdown)
**no está en el repo**: se usa `tdguard_stub.cpp` (`Init/DoWork → true`), válido
para empaquetar (todas las tools legacy lo linkeaban; el stub es reutilizable si se
portan más tools).

```bat
cmake -S . -B build-lithrez -G "Visual Studio 18 2026" -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE=C:/Users/Shifty/DEV/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DLITH_LIBS_PORTABLE=ON -DLITH_SAMPLES=OFF -DLITH_VULKAN=OFF -DLITH_TOOL_LITHREZ=ON
cmake --build build-lithrez --config Release --target lithrez
:: -> build-lithrez/Engine/Engine/tools/LithRez/Release/LithRez.exe
```

`LITH_TOOL_LITHREZ` (default `OFF`, en el `CMakeLists.txt` raíz) agrega los subdirs
`rezmgr` y `lith` (listas/hash que usa rezmgr) si `LITH_LIBS_PORTABLE` está en `OFF`.

## 3. Por qué el SHA1 del contenedor difiere tras `x` → `c`

Verificado con `nolf003cres.rez` (745.674 bytes): el reempaquetado pesa **lo mismo**
pero el SHA1 cambia. Comparación byte a byte: **solo 25 bytes difieren de 745.674**,
en 5 campos de metadatos. El contenido (los 745.472 bytes de `CRES.DLL`) es
bit a bit idéntico.

Formato (`FileMainHeaderStruct` + `FileDirEntryRezHeader`, `#pragma pack(1)`,
`Engine/Engine/libs/rezmgr/rezmgr.cpp:58-112`):

| Offset | Campo | Original (2001) | Reempaquetado | Causa |
|---|---|---|---|---|
| 52–61 (10 B) | cola de `FileType` (`"RezMgr Version 1 … MONOLITH INC."`) | espacios `0x20` | ceros `0x00` | **Introducido por la modernización**: `Flush()` hace `memset(…, ' ')` y luego `lith_safe_copy` (fix de seguridad) rellena con ceros; el `strcpy` original preservaba los espacios (`rezmgr.cpp:1761-1763`, `Platform/security.h:42`) |
| 139–142 (4 B) | `RootDirTime` (header) | `0x77EDF850` (basura) | `0x00000023` (basura) | **Bug upstream preexistente**: `m_nRootDirTime` **nunca se inicializa** en `CRezMgr::CRezMgr()` (falta el `= 0` que sí tienen los vecinos, `rezmgr.cpp:1142-1171`); es basura de stack en ambos archivos |
| 147–150 (4 B) | `Time` (header, última modificación) | `0x3AEE22BD` = 2001-05-01 | `0x6AB04FF5` = fecha actual | **Inherente al roundtrip**: `Time` = mtime de los ítems; `x` no restaura mtimes, `c` estampa "ahora" (`rezutils.cpp:468`, `rezmgr.cpp:469-472`) |
| 652–655 (4 B) | `Time` del ítem `CRES` | `0x3AEE22BD` (2001) | `0x6AB04FF5` (ahora) | Misma causa que el anterior |
| 656–659 (4 B*) | `ID` del ítem `CRES` | `10000000` | `0` | **Comportamiento upstream del path create**: `WriteDirBlock` escribe `Header.Rez.ID = 0;` hardcodeado (`rezmgr.cpp:1865`); el ID calculado (`nMiscID`) nunca se persiste. Igual suerte corren `NumKeys = 0` y el comentario (siempre vacío) |

\* de los 4 bytes del ID solo 3 difieren (`80 96 98` vs `00 00 00`; el 4.º ya era `00`).

En resumen: **2 diferencias son timestamp reales (inherentes), 1 es basura no
inicializada (bug upstream), 1 es ID hardcodeado a 0 (upstream) y 1 es
relleno de espacios→ceros (introducido por el fix de seguridad)**. Ninguna afecta
datos: el árbol extraído de ambos archivos es idéntico (ver §5).

## 4. Cómo lograr un SHA1 idéntico

> **Estado: implementado y verificado (2026-09-20).** Los 4 cambios están en
> `Engine/Engine/libs/rezmgr/rezmgr.{h,cpp}` y `rezutils.cpp`. Matriz de
> resultados al final de la sección.

Quien reempaquete con la tool original **nunca** obtendrá el SHA1 de un `.rez`
legacy: el `RootDirTime` original es basura irreproducible. Para que el roundtrip
`x` → `c` sea byte-idéntico, se hicieron estos cambios (ninguno toca gameplay):

1. **Inicializar `m_nRootDirTime = 0`** (y `m_nRootDirSize = 0`, mismo bug latente)
   en `CRezMgr::CRezMgr()` (`rezmgr.cpp`, junto a `m_nRootDirPos = 0`) — elimina la
   basura no determinista. ✅
2. **Rellenar `FileType` con espacios** (`rezmgr.cpp`, `Flush`): se copian solo
   `strlen` bytes sobre el buffer ya espaciado con `memset` (igual que `UserTitle`),
   en vez de `lith_safe_copy` que rellenaba con ceros. Reproduce el layout legacy
   exacto: string + `' '` + espacios. ✅
3. **Persistir el ID real** (`rezmgr.cpp`, `WriteDirBlock`): nuevo miembro
   `CRezItm::m_nID` (+ `GetID()`), inicializado en `InitRezItm` (antes el parámetro
   `nID` se **descartaba**) y escrito en `Header.Rez.ID` (antes hardcodeado a `0`).
   La lectura ya pasaba el ID a `InitRezItm`, así que el roundtrip es estable. ✅
4. **Preservar mtimes en `x` y re-estamparlos en `c`** (`rezutils.cpp`):
   - `ExtractDir`: `_utime` por archivo + `SetDirTime` por directorio al final
     (después de procesar hijos, que tocan el mtime del padre).
   - **Subdetalle importante**: `_utime` de MSVC **no abre directorios** (falla
     silencioso); en `_WIN32` se usa `CreateFile(FILE_FLAG_BACKUP_SEMANTICS)` +
     `SetFileTime` (conversión unix→FILETIME con `+11644473600`); en Linux basta
     `utime`. Sin esto, los `Time` de subdirectorios volvían como "ahora".
   - `TransferDir`: `SetTime(disk)` al ítem **después** de `Save()` (tanto `Create`
     como `Save` llaman `MarkCurTime` = "ahora" y lo pisan), estampado del subdir
     **post-recursión**, y `g_nLastDiskTime` (último mtime visto, espejando la
     semántica legacy) aplicado al header vía nuevo `CRezMgr::SetLastTimeModified`
     + nuevo `CRezDir::SetTime`. ✅

Con 1–4 aplicados, `x` → `c` de un archivo **creado por la tool ya corregida**
da SHA1 idéntico (punto fijo verificado: reempaquetar el reempaquetado da el mismo
hash). Los `.rez` legacy con basura en `RootDirTime` seguirán difiriendo en esos
4 bytes (irrecuperables por definición).

Nota: 1 y 2 cambian bytes del contenedor respecto a lo que genera la tool sin
corregir; 3 cambia el formato efectivo (IDs reales en vez de 0). Si algún juego
depende de `ID == 0` en archivos creados por LithRez, revisar antes de usar en
producción (el `v`/`x` no se ven afectados).

### Matriz de verificación (binario actual)

### Matriz de verificación (binario actual, 9/9 archivos de `REZ-tests/`)

Protocolo por archivo: `v` (cuenta) → `x` fresco → `c` (repack1) → SHA1 vs
original → `x` de repack1 → `c` (repack2) → SHA1 repack1 vs repack2
(estabilidad) → comparación de árboles extraídos archivo por archivo.
Todos los comandos exit 0.

| archivo | tamaño | contenido | contenedor == original? | estable repack2? | árboles idénticos? | causa de la diferencia |
|---|---|---|---|---|---|---|
| `MODERNIZER.REZ` | 30.816.142 | 18 dirs / 114 ítems | ✅ idéntico (`96989031…`) | ✅ | ✅ 114 | — |
| `NOLF.REZ` | 618.254.258 | 213 dirs / 4871 ítems | ❌ | ✅ | ✅ 4871 | orden de recorrido ≠ orden del 2000 → `pos` (4761) + `id` (1076) + `Time` de header (último ítem) |
| `nolf003cres.rez` | 745.674 | 1 dir / 1 ítem | ❌ 4 bytes | ✅ | ✅ 1 | solo `RootDirTime` (basura `0x77EDF850` vs `0`) |
| `NOLF2.REZ` | 300.703.727 | 152 dirs / 6075 ítems | ❌ | ✅ | ✅ 6075 | orden de recorrido: `pos` (6149) + `id` (1285); headers idénticos (tiempos incluidos) |
| `NOLFCRES003.REZ` | 745.674 | 1 dir / 1 ítem | ❌ 4 bytes | ✅ | ✅ 1 | solo `RootDirTime` (entradas idénticas, verificado por `fc`) |
| `NOLFGOTY.REZ` | 170.190.784 | 50 dirs / 387 ítems | ❌ 4 bytes* | ✅ | ✅ 387 | entradas idénticas; headers iguales salvo `RootDirTime` (`0x77EDF840` vs `0`) |
| `nolfu003.rez` | 18.194.503 | 10 dirs / 25 ítems | ❌ | ✅ | ✅ 25 | varianza parcial de orden (`pos`=10, `id`=2) + `Time` de header a 1 s (`…E6` vs `…E5`, el último ítem difiere) |
| `nolfu003cres.rez` | 745.674 | 1 dir / 1 ítem | ❌ 4 bytes | ✅ | ✅ 1 | solo `RootDirTime` (entradas idénticas, verificado por `fc`) |
| `WidescreenGOTY.rez` | 1.315.020 | 1 dir / 1 ítem | ❌ ~60 bytes | ✅ | ✅ 1 | creado por **otra tool** (`FileType = "WinRez LT 3.0 … BlackAngel Software"`); LithRez escribe su propio FileType. Además `Time`: `0` → mtime real |

\* inferido por construcción (entradas idénticas + headers iguales salvo el campo),
verificado byte a byte con `fc` en los demás casos chicos.

**Varianza de orden de recorrido** (NOLF, NOLF2, parcial en nolfu003): los datos se
escriben en orden de enumeración `_findfirst` del disco actual; los `.rez` del
2000 se empaquetaron en otro orden (FS de la época). Como los IDs se asignan
secuenciales en ese orden (`10000000+n`), también rotan. El orden de entradas en
los bloques (por hash, determinista) sí coincide, y todo el resto
(tamaños, tiempos, tipos, nombres, comentarios) es idéntico. No es recuperable
desde una carpeta plana (el orden original no existe en disco); el repack del
repack **siempre** es estable porque el orden del disco no cambia.

## 5. Informe del trabajo (sesión 2026-09-20)

**Objetivo**: comprobar si LITHREZ compila y si desempaquetar→reempaquetar
preserva el SHA1 de `REZ-tests/`.

1. **Orientación con Graphify** (índice existente en `graphify-out/`): localizados
   `main` (`tools/LithRez/lithrez.cpp`), `RezCompiler/TransferDir/ExtractDir/
   CheckLithHeader/IsCommandSet` (`libs/rezmgr/rezutils.cpp`) y la dependencia
   `Lib_RezMgr.vcproj → Libs/lith`. Baseline SHA1 (9 archivos) registrado.
2. **Estrategia de build**: solo hay `gcc` (MinGW) en PATH, pero `rezmgr.cpp`
   arrastra headers de physics que no compilan con GCC; se detectó VS18 + MSVC
   14.50 instalados y un `build-msvc/` ya configurado → build CMake con generador
   `Visual Studio 18 2026`, sin tocar toolchain legacy. `TdGuard.lib` ausente →
   stub propio.
3. **Fixes para compilar** (`rezutils.cpp`, `lithrez.cpp`): `#include <ctype.h>`
   (`toupper`, C3861), scope `for (int i…)` estilo MSVC6 (C2065), link contra `lith`
   (`CLTBaseList/CBaseHash/CVirtBaseList`), target `lithrez` + `tdguard_stub` vía
   `LITH_TOOL_LITHREZ`.
4. **Crash al ver (`0xC0000409`)**: stack trace con `cdb` mostró que el programa
   completaba el trabajo y moría al salir en `ucrtbase!_close` — `GetFileSize`
   cerraba el fd **dos veces** (silencioso en MSVC6, abort en UCRT). Eliminado el
   segundo `_close`.
5. **Crash al crear (`0xC0000005`)**: handle de `_findfirst` guardado en `long`
   (32 bits) truncado en x64 → `intptr_t` en 3 sitios (`rezutils.cpp` ×2,
   `rezmgr.cpp` ×1).
6. **Verificación funcional**: `v`/`x`/`c` exit 0 en `nolf003cres.rez` (1 recurso),
   `WidescreenGOTY.rez`, y `MODERNIZER.REZ` (18 dirs, 114 recursos). Roundtrip:
   mismo tamaño, SHA1 distinto, **árboles extraídos idénticos** (114/114 SHA1 por
   archivo + rutas). Investigación de los 25 bytes → §3–§4 de este documento.

**Estado final**: matriz completa 9/9 (§4). Pendiente: refrescar el índice
Graphify (`graphify update`) para los archivos tocados; decidir si los cambios
del §4 (IDs reales, `RootDirTime=0`) son aceptables para assets de producción.

### 6. Implementación de byte-identidad (continuación de la sesión)

6. **Los 4 cambios del §4 implementados**: `m_nID` en `CRezItm`, init de
   `m_nRootDirTime/m_nRootDirSize`, `FileType` con espacios, `WriteDirBlock` con
   ID real, mtimes en `x`/`c` (`SetDiskTime`/`SetDirTime`, `g_nLastDiskTime`,
   `SetLastTimeModified`, `CRezDir::SetTime`).
7. **Hallazgo clave en el camino**: `TransferDir` parecía estampar mtimes de disco,
   pero `Create()` y `Save()` llaman `MarkCurTime` ("ahora") y lo pisan — por eso
   hizo falta re-estampar **después** de `Save()` y post-recursión en subdirs.
8. **`_utime` no funciona en directorios Windows** (falla silencioso): se agregó
   `SetDirTime` con `CreateFile(FILE_FLAG_BACKUP_SEMANTICS)` + `SetFileTime`.
   Detectado porque los `Time` de subdirs de MODERNIZER volvían como "ahora"
   (parseado entrada por entrada del bloque de directorios para aislarlo).
9. **Verificación final**: matriz del §4 — MODERNIZER byte-idéntico,
   nolf003cres a 4 bytes (basura legacy), Widescreen difiere solo por ser de otra
   tool, repack-del-repack idéntico en los 3.
