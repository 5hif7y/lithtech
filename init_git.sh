#!/bin/bash
# Git init para NOLF2 LithTech Jupiter - ArchLinux
set -e
if [ -d .git ]; then
  echo ".git ya existe, skip init"
else
  git init
  echo "git init hecho"
fi

cat > .gitignore <<'EOF'
# Binarios Windows
*.exe
*.dll
*.lib
*.pdb
*.obj
*.exp
*.ilk
*.idb
*.pch
*.bsc
*.iobj
*.ipdb
# Build dirs
build/
out/
build-*/
install/
# VS / MSVC
.vs/
ipch/
*.vcxproj.user
*.vcxproj.filters
*.sln
# Linux build artifacts
*.o
*.a
*.so
*.so.*
# CMake
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
compile_commands.json
# Graphify
graphify-output/
*.graphify.json
# Vulkan
*.spv
EOF

cat .gitignore
echo ""
echo "Ejecuta: git add . && git status | head -n 100"
