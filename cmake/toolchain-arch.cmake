# Toolchain ArchLinux gcc/g++
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Flags Arch
set(CMAKE_C_FLAGS_INIT "-march=native -pipe")
set(CMAKE_CXX_FLAGS_INIT "-march=native -pipe")

# No cross-compile - native Arch, no sysroot restrictions
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
