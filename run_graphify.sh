#!/bin/bash
# Indexacion con graphify - todo el directorio
# Requiere: graphify instalado (cargo install graphify o npm)
set -e
if ! command -v graphify &>/dev/null; then
  echo "graphify no encontrado. Instala con:"
  echo "  cargo install graphify"
  echo "  o npm i -g graphify"
  echo "  o pip install graphify"
  exit 1
fi

echo "Graphify version: $(graphify --version 2>&1 || graphify --help 2>&1 | head -n 1)"

OUT="graphify-output/nolf2-index.json"
mkdir -p graphify-output

echo "Indexando todo el contenido..."
# Intenta variantes de CLI - ajusta segun tu graphify
if graphify --help 2>&1 | grep -q "index"; then
  graphify index --path . --output "$OUT"
elif graphify --help 2>&1 | grep -q "\-\-root"; then
  graphify --root . --out "$OUT" --include "*.cpp,*.h,*.hpp,*.c"
else
  graphify . -o "$OUT"
fi

echo "Indice generado: $OUT"
ls -lh "$OUT"
head -n 100 "$OUT"
