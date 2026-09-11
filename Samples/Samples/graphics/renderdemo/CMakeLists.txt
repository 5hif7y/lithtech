# graphics/renderdemo — demo Linux (graphify Fase4, 38 nodes, auto-generated)
cmake_minimum_required(VERSION 3.27)
file(GLOB DEMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cshell/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/shared/src/*.cpp") # sshell excluded on Linux (requires server BaseClass, graphify 18 demos)
if(DEMO_SOURCES STREQUAL "")
  file(GLOB_RECURSE DEMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
endif()
if(DEMO_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "int dummy(){return 0;}")
  set(DEMO_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()
add_executable(graphics_renderdemo EXCLUDE_FROM_ALL ${DEMO_SOURCES} ${CMAKE_SOURCE_DIR}/Platform/winmain_stub.cpp ${CMAKE_SOURCE_DIR}/Platform/demo_link_stubs.cpp)
target_include_directories(graphics_renderdemo BEFORE PRIVATE
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_SOURCE_DIR}/Engine/Engine/runtime/shared/src
  ${CMAKE_SOURCE_DIR}/Engine/Engine/runtime/client/src
  ${CMAKE_SOURCE_DIR}/Engine/Engine/clientfx
  ${CMAKE_SOURCE_DIR}/Engine/Engine/clientfx/Shared
  ${CMAKE_CURRENT_SOURCE_DIR}/shared/src
  ${CMAKE_CURRENT_SOURCE_DIR}/cshell/src
)
# Fallback: ensure main exists
target_link_libraries(graphics_renderdemo PRIVATE lith_platform lith_engine renderer_vulkan SDL2::SDL2 Vulkan::Vulkan)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(graphics_renderdemo PRIVATE -fpermissive -include ${CMAKE_SOURCE_DIR}/Platform/math_shim.h)
  target_compile_definitions(graphics_renderdemo PRIVATE _LINUX PLATFORM_LINUX)
endif()
message(STATUS "Demo graphics/renderdemo configured (graphify 38 nodes)")
