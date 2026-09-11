# graphics/fonts — demo Linux (graphify Fase4, 36 nodes, auto-generated)
cmake_minimum_required(VERSION 3.27)
file(GLOB DEMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cshell/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/sshell/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/shared/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/cres/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/sres/src/*.cpp")
if(DEMO_SOURCES STREQUAL "")
  file(GLOB_RECURSE DEMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
endif()
if(DEMO_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "int dummy(){return 0;}")
  set(DEMO_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()
add_executable(graphics_fonts EXCLUDE_FROM_ALL ${DEMO_SOURCES})
target_include_directories(graphics_fonts BEFORE PRIVATE
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_SOURCE_DIR}/Engine/Engine/runtime/shared/src
  ${CMAKE_SOURCE_DIR}/Engine/Engine/runtime/client/src
  ${CMAKE_SOURCE_DIR}/Engine/Engine/clientfx
  ${CMAKE_SOURCE_DIR}/Engine/Engine/runtime/shared/src
)
target_link_libraries(graphics_fonts PRIVATE lith_platform lith_engine renderer_vulkan SDL2::SDL2 Vulkan::Vulkan)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(graphics_fonts PRIVATE -fpermissive -include ${CMAKE_SOURCE_DIR}/Platform/math_shim.h)
  target_compile_definitions(graphics_fonts PRIVATE _LINUX PLATFORM_LINUX)
endif()
message(STATUS "Demo graphics/fonts configured (graphify 36 nodes)")
