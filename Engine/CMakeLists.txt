# Engine - LithTech Jupiter Core
# En Linux gcc: Engine completo (MSVC6/DX8, 1193 cpp, 89 windows.h, 10 __asm, 49 DirectX) no compila directo
# Se usa stub documentado; set LITH_ENGINE_FULL=ON para intentar build completo incremental
option(LITH_ENGINE_FULL "Compilar Engine completo (requiere port incremental)" OFF)
if(LITH_ENGINE_FULL)
  file(GLOB_RECURSE ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
  )
  file(GLOB_RECURSE ENGINE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
  )
  list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/(d3d_.*|dx.*|win_.*)\\.cpp$")
  list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/Engine/clientfx/.*")
  list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/Engine/tools/.*")
  list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/Engine/libs/.*")
  list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/built/.*")
else()
  # Stub Linux - permite que toolchain gcc+Vulkan se verifique sin portar 1193 archivos MSVC
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/lith_engine_stub.cpp "// LithTech Jupiter - Linux stub\n// Engine completo requiere port: 89 windows.h, 10 __asm, 49 DirectX (ver graphify-output)\n#include \"Platform/platform.h\"\nint lith_engine_linux_stub(){return 0;}\n// Export simbolos esperados por renderer\n")
  set(ENGINE_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/lith_engine_stub.cpp)
  set(ENGINE_HEADERS "")
endif()

add_library(lith_engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})
target_include_directories(lith_engine BEFORE PUBLIC
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Shared
  ${CMAKE_SOURCE_DIR}/Engine
)
target_link_libraries(lith_engine PUBLIC lith_platform SDL2::SDL2)
target_compile_definitions(lith_engine PUBLIC ENGINE_LINUX)

# Math con SSE en vez de __asm
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(lith_engine PRIVATE -msse2 -mfpmath=sse)
endif()
