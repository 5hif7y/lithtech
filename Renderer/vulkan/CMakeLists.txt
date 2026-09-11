# Renderer Vulkan - reemplazo D3D8/9 para Linux
if(NOT Vulkan_FOUND)
  find_package(Vulkan QUIET)
endif()

file(GLOB VULKAN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)
file(GLOB VULKAN_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

# Si no hay fuentes aun, crea stub compilable
if(VULKAN_SOURCES STREQUAL "")
  set(VULKAN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/vk_renderer.cpp)
endif()

add_library(renderer_vulkan STATIC ${VULKAN_SOURCES} ${VULKAN_HEADERS})

target_include_directories(renderer_vulkan PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}/Engine
)

target_link_libraries(renderer_vulkan PUBLIC
  Vulkan::Vulkan
  SDL2::SDL2
  lith_platform
  lith_engine
)

target_compile_definitions(renderer_vulkan PUBLIC RENDERER_VULKAN VK_USE_PLATFORM_XLIB_KHR)

# Shaders GLSL -> SPIR-V (si existen shaders/)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
  find_program(GLSLANG_VALIDATOR glslangValidator)
  if(GLSLANG_VALIDATOR)
    file(GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag)
    foreach(SHADER ${SHADERS})
      get_filename_component(SHADER_NAME ${SHADER} NAME)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.spv
        COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER} -o ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.spv
        DEPENDS ${SHADER}
      )
      list(APPEND SPV_SHADERS ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.spv)
    endforeach()
    add_custom_target(vk_shaders ALL DEPENDS ${SPV_SHADERS})
    add_dependencies(renderer_vulkan vk_shaders)
  endif()
endif()
