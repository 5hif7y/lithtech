cmake_minimum_required(VERSION 3.27)
add_executable(vulkan_demo main.cpp)
target_include_directories(vulkan_demo BEFORE PRIVATE
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
)
target_link_libraries(vulkan_demo PRIVATE lith_platform renderer_vulkan SDL2::SDL2 Vulkan::Vulkan)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(vulkan_demo PRIVATE -fpermissive -include ${CMAKE_SOURCE_DIR}/Platform/math_shim.h)
endif()
message(STATUS "vulkan_demo configured (standalone, demuestra que demos pueden renderizar con Vulkan)")
