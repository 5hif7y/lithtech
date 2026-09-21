# LithRez - empaquetador/desempaquetador .rez (LITHREZ c/v/x) via CMake.
# No usa LithRez.vcproj (toolchain legacy, intacto).
#
#   cmake -B build -DLITH_TOOL_LITHREZ=ON [...]
#
# Uso: LithRez <comandos> <archivo.rez> [params]  (c=crear, v=ver, x=extraer)

# TdGuard.lib original no esta en el repo -> stub (Init/DoWork siempre OK).
add_library(tdguard_stub STATIC tdguard_stub.cpp)
target_include_directories(tdguard_stub PUBLIC
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
)

add_executable(lithrez lithrez.cpp)
set_target_properties(lithrez PROPERTIES OUTPUT_NAME LithRez)
# Mismas defines que LithRez.vcproj (Release): WIN32, REZMGRFULL, _CONSOLE.
# _CRT_SECURE_NO_WARNINGS: el codigo 2004 usa strcpy/strupr (warnings, no errores).
target_compile_definitions(lithrez PRIVATE
  WIN32
  REZMGRFULL
  _CONSOLE
  NOMINMAX
  _CRT_SECURE_NO_WARNINGS
)
target_link_libraries(lithrez PRIVATE rezmgr lith tdguard_stub)
