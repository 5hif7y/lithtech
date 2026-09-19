# networking/sealhunter — demo Linux (graphify Fase4, 93 nodes, auto-generated)
cmake_minimum_required(VERSION 3.27)
file(GLOB DEMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cshell/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/shared/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/sshell/src/*.cpp") # sshell: server objects (Seal/Snowman/Player) + CLTServerShell, BaseClass comes from SDK ltengineobjects.h
if(DEMO_SOURCES STREQUAL "")
  file(GLOB_RECURSE DEMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
endif()
if(DEMO_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "int dummy(){return 0;}")
  set(DEMO_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()
add_executable(networking_sealhunter EXCLUDE_FROM_ALL ${DEMO_SOURCES} ${CMAKE_SOURCE_DIR}/Platform/host/host_sealhunter.cpp ${CMAKE_SOURCE_DIR}/Platform/demo_link_stubs.cpp
  # SDK game-object implementations (BaseClass/ILTBaseClass/ClassDef registry)
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc/iltbaseclass.cpp
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc/ltengineobjects.cpp
  # clientfx message parser used by ServerSpecialFX (self-contained)
  ${CMAKE_SOURCE_DIR}/Engine/Engine/clientfx/Shared/ParsedMsg.cpp)
target_include_directories(networking_sealhunter BEFORE PRIVATE
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_SOURCE_DIR}/Engine/Engine/runtime/shared/src
  ${CMAKE_SOURCE_DIR}/Engine/Engine/runtime/client/src
  ${CMAKE_SOURCE_DIR}/Engine/Engine/clientfx
  ${CMAKE_SOURCE_DIR}/Engine/Engine/clientfx/Shared
  ${CMAKE_CURRENT_SOURCE_DIR}/shared/src
  ${CMAKE_CURRENT_SOURCE_DIR}/cshell/src
  ${CMAKE_CURRENT_SOURCE_DIR}/sshell/src
  ${CMAKE_SOURCE_DIR}/Renderer/vulkan
)
# Fallback: ensure main exists
target_link_libraries(networking_sealhunter PRIVATE lith_platform lith_engine renderer_vulkan SDL2::SDL2 Vulkan::Vulkan)
find_package(SDL3 QUIET)
if(SDL3_FOUND)
  target_link_libraries(networking_sealhunter PRIVATE SDL3::SDL3)
  target_compile_definitions(networking_sealhunter PRIVATE HAS_SDL3)
  target_sources(networking_sealhunter PRIVATE
    ${CMAKE_SOURCE_DIR}/Platform/host/host_sdl3.cpp)
  message(STATUS "networking/sealhunter: SDL3 windowing enabled")
endif()
if(UNIX)
  target_link_libraries(networking_sealhunter PRIVATE X11)
endif()
find_package(Freetype QUIET)
if(Freetype_FOUND)
  target_link_libraries(networking_sealhunter PRIVATE Freetype::Freetype)
  target_compile_definitions(networking_sealhunter PRIVATE HAS_FREETYPE)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(networking_sealhunter PRIVATE -fpermissive -fno-rtti -include ${CMAKE_SOURCE_DIR}/Platform/math_shim.h)
  target_compile_definitions(networking_sealhunter PRIVATE _LINUX PLATFORM_LINUX)
endif()
if(WIN32)
  # SDL2 renames main->SDL_main on Windows (SDL_main.h); the demo provides a
  # plain console main(), so opt out of the redirection (no SDL2main needed).
  target_compile_definitions(networking_sealhunter PRIVATE SDL_MAIN_HANDLED)
endif()
message(STATUS "Demo networking/sealhunter configured (graphify 93 nodes)")
