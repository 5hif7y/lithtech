#Graphify 

## Por qué graphify

LithTech Jupiter tiene **7888 archivos** (`.h:4491 .cpp:2599 .lta:210 .vcproj:220` etc). Leerlos todos para documentar o buscar vulns costaría ~9.4M tokens. Graphify indexa una vez y luego se consulta el JSON.

## Cómo se instaló / fallback

```bash
which graphify        # no found
pip install graphify  # ERROR: No matching distribution
cargo install graphify # cargo not found
npm i -g graphify     # npm not found
# → fallback Python:
python3 -m venv .venv
.venv/bin/python /tmp/graphify_index.py  # genera graphify-output/nolf2-index.json
```

**Generado:** `graphify-output/nolf2-index.json` (2.1M, 95120 líneas)  
**Stats:** `{'total_files': 7888, 'total_nodes': 7215, 'total_edges': 20657, 'by_category': {'Renderer/vulkan': 2, 'Tools': 12, 'Platform': 1, 'Libs': 145, 'Engine': 4357, 'Samples': 1266, 'Other': 97, 'Game': 2008}, 'by_ext': {'.h': 4491, '.cpp': 2599, '.lta': 210, '.sln': 67, '.vcproj': 220, '.c': 100, '.rc': 145, '.idl': 5, '.inl': 1, '.hpp': 24, '.lto': 26}}`  
**Top hubs (por `includes`):** [{'path': 'Game/Game/ObjectDLL/ObjectShared/GlobalServerMgr.cpp', 'outgoing': 125}, {'path': 'Engine/Engine/tools/Plugins/SDKFiles/PhotoshopSDK/samplecode/common/includes/PhotoshopSDK.h', 'outgoing': 68}, {'path': 'Game/Game/Libs/WONAPI/WONRouting/Routing.cpp', 'outgoing': 64}, {'path': 'Game/Game/Libs/WONAPI/WONRouting/AllRoutingOps.h', 'outgoing': 61}, {'path': 'Game/Game/ClientShellDLL/ClientShellShared/SFXMgr.cpp', 'outgoing': 61}]

## Cómo se usó para ahorrar tokens en esta doc

Toda la carpeta `Muse-Docs/` se generó con:

```python
idx = json.load(open("graphify-output/nolf2-index.json"))
stats = idx["stats"]  # no leer 7888 archivos
top = idx["top_hubs"][:20]  # priorizar
# → escribir 01-05.md sin re-leer 4357 Engine + 2008 Game files
```

**Medición:** `index_tokens = size/4 ≈ 532,519` vs `naive = 9,465,600` → **94% ahorro**.

## Verificación que funciona

```bash
ls -lh graphify-output/nolf2-index.json  # 2.1M
.venv/bin/python /tmp/vuln_scan.py       # usa idx["nodes"] para scan strcpy/sprintf (662/1078) sin re-escanear todo
.venv/bin/python /tmp/vuln_scan_after.py # re-scan solo 4 files fix + index
```

Graphify sigue detectando restantes (660 strcpy tras fix demo) — índice no quedo stale, es re-usable.

## Cuándo regenerar

```bash
.venv/bin/python /tmp/graphify_index.py  # tras grandes cambios
# o: ./run_graphify.sh  # intenta graphify binario, cae a fallback
```
