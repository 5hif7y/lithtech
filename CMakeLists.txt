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
    __LINUX
    _RETAIL
    NOMINMAX
    STRICT
    _HAS_EXCEPTIONS=0
  )
endif()

# En MSVC los macros min/max de windows.h rompen <algorithm>/<limits>
# (std::max, numeric_limits::max, std::clamp). El codigo 2003-era que los
# necesite usara __min/__max o <algorithm> explicito.
if(MSVC)
  add_compile_definitions(NOMINMAX)
endif()
# Dependencias Vulkan - en Windows se prefiere el SDK instalado (vulkan-1.lib);
# en Linux se usa el vendoreado si no hay headers de sistema (Arch sin sudo no tiene vulkan-headers)
if(WIN32)
  find_package(Vulkan QUIET)
endif()
if(NOT Vulkan_FOUND AND EXISTS ${CMAKE_SOURCE_DIR}/third_party/vulkan/include/vulkan/vulkan.h)
  set(Vulkan_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/vulkan/include)
  set(Vulkan_FOUND TRUE)
  if(NOT TARGET Vulkan::Vulkan)
    add_library(Vulkan::Vulkan INTERFACE IMPORTED)
    target_include_directories(Vulkan::Vulkan INTERFACE ${Vulkan_INCLUDE_DIR})
    target_link_libraries(Vulkan::Vulkan INTERFACE vulkan)
  endif()
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
if(WIN32)
  target_compile_definitions(lith_platform INTERFACE PLATFORM_WINDOWS)
  # Platform/windows.h sombrea a <windows.h> en el include path: el dispatcher
  # incluye el header real del SDK por ruta absoluta (LITH_REAL_WINDOWS_H).
  if(DEFINED ENV{WindowsSdkDir})
    file(TO_CMAKE_PATH "$ENV{WindowsSdkDir}" _LITH_WINSDK_DIR)
  else()
    get_filename_component(_LITH_WINSDK_DIR
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE)
  endif()
  if(NOT CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
    file(GLOB _LITH_SDK_VERS RELATIVE "${_LITH_WINSDK_DIR}/Include" "${_LITH_WINSDK_DIR}/Include/10.*")
    list(SORT _LITH_SDK_VERS)
    list(GET _LITH_SDK_VERS -1 CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
  endif()
  set(LITH_REAL_WINDOWS_H
    "${_LITH_WINSDK_DIR}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/windows.h")
  if(NOT EXISTS "${LITH_REAL_WINDOWS_H}")
    message(FATAL_ERROR "No se encontro el SDK header real: ${LITH_REAL_WINDOWS_H}")
  endif()
  message(STATUS "LITH_REAL_WINDOWS_H=${LITH_REAL_WINDOWS_H}")
  target_compile_definitions(lith_platform INTERFACE
    "LITH_REAL_WINDOWS_H=\"${LITH_REAL_WINDOWS_H}\"")
  # Idem para <crtdbg.h>: el shim sombrea al header real (vive en el UCRT
  # del SDK, no en MSVC) y rompe <vector>/<memory> (_RPTF0/_CRT_ASSERT).
  set(LITH_REAL_CRTDBG_H
    "${_LITH_WINSDK_DIR}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/ucrt/crtdbg.h")
  if(NOT EXISTS "${LITH_REAL_CRTDBG_H}")
    message(FATAL_ERROR "No se encontro el header real crtdbg.h: ${LITH_REAL_CRTDBG_H}")
  endif()
  message(STATUS "LITH_REAL_CRTDBG_H=${LITH_REAL_CRTDBG_H}")
  target_compile_definitions(lith_platform INTERFACE
    "LITH_REAL_CRTDBG_H=\"${LITH_REAL_CRTDBG_H}\"")
else()
  target_compile_definitions(lith_platform INTERFACE PLATFORM_LINUX)
endif()

# Libs portables (Fase 2, graphify wt 0-1) — se añaden antes de Engine para que Engine pueda linkear si LITH_ENGINE_FULL=ON
option(LITH_LIBS_PORTABLE "Compilar libs portables via graphify (Fase2)" OFF)
if(LITH_LIBS_PORTABLE)
  foreach(_lib ButeMgr CryptMgr stdlith lith ltmem ltamgr rezmgr genregmgr dtxmgr RegMgr RegMgr32 MFCStub dibmgr StackTracer zlib)
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

option(LITH_SAMPLES "Compilar demos Samples" ON)
if(LITH_SAMPLES)
  # Demos via graphify (18 demos, 740 nodes) — cada demo tiene su CMakeLists.txt
  file(GLOB SAMPLE_DIRS "${CMAKE_SOURCE_DIR}/Samples/Samples/*/*")
  foreach(_d ${SAMPLE_DIRS})
    if(EXISTS ${_d}/CMakeLists.txt)
      add_subdirectory(${_d})
    endif()
  endforeach()
endif()

message(STATUS "LithTech Jupiter - Arch gcc/g++ | Vulkan=${LITH_VULKAN} | Build=${CMAKE_BUILD_TYPE}")
add_subdirectory(DemoVulkan)
add_custom_target(check_case COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/check_case.cmake COMMENT "Checking case-sensitive includes via graphify")
