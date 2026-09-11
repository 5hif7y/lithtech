cmake_minimum_required(VERSION 3.27)
project(LithtechJupiter CXX C)

# Toolchain: gcc/g++ en ArchLinux
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "" FORCE)
endif()

# Flags gcc/g++ - reemplazo de MSVC (graphify: 89 windows.h, 24 __asm, 148 windows_tokens → shims)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(
    -Wall -Wextra -Wno-multichar -Wno-write-strings -Wno-unused-parameter
    -Wno-narrowing -Wno-invalid-offsetof -fpermissive -fno-strict-aliasing
    -fvisibility=hidden
    -include ${CMAKE_SOURCE_DIR}/Platform/math_shim.h
  )
  add_compile_definitions(
    _LINUX
    _RETAIL
    NOMINMAX
    STRICT
    _HAS_EXCEPTIONS=0
  )
endif()

# Dependencias Linux - Vulkan vendoreado si no esta en sistema (Arch sin sudo no tiene vulkan-headers)
if(EXISTS ${CMAKE_SOURCE_DIR}/third_party/vulkan/include/vulkan/vulkan.h)
  set(Vulkan_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/vulkan/include)
  set(Vulkan_FOUND TRUE)
  add_library(Vulkan::Vulkan INTERFACE IMPORTED)
  target_include_directories(Vulkan::Vulkan INTERFACE ${Vulkan_INCLUDE_DIR})
  target_link_libraries(Vulkan::Vulkan INTERFACE vulkan)
  find_package(Vulkan QUIET)
  if(NOT Vulkan_FOUND)
    set(Vulkan_FOUND TRUE)
  endif()
else()
  find_package(Vulkan QUIET)
endif()
if(NOT Vulkan_FOUND)
  message(WARNING "Vulkan headers no encontradas - Renderer/vulkan compilara como stub sin Vulkan. Instala vulkan-headers para build completo.")
endif()
find_package(SDL2 REQUIRED)

option(LITH_VULKAN "Usar renderer Vulkan (reemplaza D3D8/9)" ON)
option(LITH_SDL_WINDOW "Usar SDL2 para windowing/input (Linux)" ON)

# Shim de plataforma - header-only
add_library(lith_platform INTERFACE)
target_include_directories(lith_platform INTERFACE
  ${CMAKE_SOURCE_DIR}/Platform
)
target_compile_definitions(lith_platform INTERFACE PLATFORM_LINUX)

# Libs portables (Fase 2, graphify wt 0-1) — se añaden antes de Engine para que Engine pueda linkear si LITH_ENGINE_FULL=ON
option(LITH_LIBS_PORTABLE "Compilar libs portables via graphify (Fase2)" OFF)
if(LITH_LIBS_PORTABLE)
  foreach(_lib ButeMgr CryptMgr stdlith lith ltmem ltamgr rezmgr)
    if(EXISTS ${CMAKE_SOURCE_DIR}/Libs/Libs/${_lib}/CMakeLists.txt)
      add_subdirectory(Libs/Libs/${_lib})
    elseif(EXISTS ${CMAKE_SOURCE_DIR}/Engine/Engine/libs/${_lib}/CMakeLists.txt)
      add_subdirectory(Engine/Engine/libs/${_lib})
    endif()
  endforeach()
endif()

# Engine - file glob resiliente a estructura exacta
add_subdirectory(Engine)

if(LITH_VULKAN)
  add_subdirectory(Renderer/vulkan)
else()
  message(WARNING "LITH_VULKAN=OFF - renderer D3D no compila en Linux, quedara stub")
endif()

# Opcional: Client/Server si existen
if(EXISTS ${CMAKE_SOURCE_DIR}/Client/CMakeLists.txt)
  add_subdirectory(Client)
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/Server/CMakeLists.txt)
  add_subdirectory(Server)
endif()

message(STATUS "LithTech Jupiter - Arch gcc/g++ | Vulkan=${LITH_VULKAN} | Build=${CMAKE_BUILD_TYPE}")
