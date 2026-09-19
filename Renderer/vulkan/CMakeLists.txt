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

target_compile_definitions(renderer_vulkan PUBLIC RENDERER_VULKAN)
if(WIN32)
  target_compile_definitions(renderer_vulkan PUBLIC VK_USE_PLATFORM_WIN32_KHR)
else()
  target_compile_definitions(renderer_vulkan PUBLIC VK_USE_PLATFORM_XLIB_KHR)
endif()

# Shaders GLSL -> SPIR-V embebido (tri pipeline headless + ventana)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
  find_program(GLSLANG_VALIDATOR glslangValidator)
  find_package(Python3 QUIET COMPONENTS Interpreter)
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
    # Embebe tri.vert/tri.frag como header para el pipeline headless
    if(Python3_FOUND)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tri_spv.h
        COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tri.vert -o ${CMAKE_CURRENT_BINARY_DIR}/tri.vert.spv
        COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tri.frag -o ${CMAKE_CURRENT_BINARY_DIR}/tri.frag.spv
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/embed_spv.py ${CMAKE_CURRENT_BINARY_DIR}/tri.vert.spv ${CMAKE_CURRENT_BINARY_DIR}/tri.frag.spv ${CMAKE_CURRENT_BINARY_DIR}/tri_spv.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tri.vert ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tri.frag
      )
      add_custom_target(vk_tri_spv DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tri_spv.h)
      add_dependencies(renderer_vulkan vk_tri_spv)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tri_tex_spv.h
        COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tri_tex.vert -o ${CMAKE_CURRENT_BINARY_DIR}/tri_tex.vert.spv
        COMMAND ${GLSLANG_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tri_tex.frag -o ${CMAKE_CURRENT_BINARY_DIR}/tri_tex.frag.spv
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/embed_spv.py ${CMAKE_CURRENT_BINARY_DIR}/tri_tex.vert.spv ${CMAKE_CURRENT_BINARY_DIR}/tri_tex.frag.spv ${CMAKE_CURRENT_BINARY_DIR}/tri_tex_spv.h kTriTexVertSpv kTriTexFragSpv
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tri_tex.vert ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tri_tex.frag
      )
      add_custom_target(vk_tri_tex_spv DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tri_tex_spv.h)
      add_dependencies(renderer_vulkan vk_tri_tex_spv)
      target_include_directories(renderer_vulkan PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    endif()
  endif()
endif()
