#!/bin/bash
# Limpieza de binarios Windows - NOLF2 LithTech Jupiter
# Generado por Muse Code - Ejecutar desde nolf2_gpl_src/
set -e
echo "[1/3] Buscando binarios Windows..."
find . -type f \( \
  -iname "*.exe" -o -iname "*.dll" -o -iname "*.lib" -o -iname "*.pdb" \
  -o -iname "*.obj" -o -iname "*.exp" -o -iname "*.ilk" -o -iname "*.idb" \
  -o -iname "*.pch" -o -iname "*.bsc" \
\) -print | tee /tmp/windows-binaries.txt

COUNT=$(wc -l < /tmp/windows-binaries.txt)
echo "Encontrados: $COUNT archivos"

if [ "$COUNT" -eq 0 ]; then
  echo "Nada que borrar."
  exit 0
fi

echo "[2/3] Borrando..."
xargs -a /tmp/windows-binaries.txt rm -v

echo "[3/3] Limpiando directorios de build MSVC..."
find . -type d \( -name "Debug" -o -name "Release" -o -name "ipch" -o -name ".vs" \) -print
find . -type d \( -name "Debug" -o -name "Release" -o -name "ipch" -o -name ".vs" \) -exec rm -rf {} + 2>/dev/null || true

echo "Limpieza completa. Verifica con: git status"
