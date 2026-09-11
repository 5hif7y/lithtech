# ltmem — portable Linux (graphify wt 0-1, fase2)
# Generado via graphify (ahorro tokens: no leer 145 Libs files, usar idx)
cmake_minimum_required(VERSION 3.27)
file(GLOB LTMEM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB LTMEM_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
if(LTMEM_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ltmem_stub.cpp "int ltmem_stub(){return 0;}")
  set(LTMEM_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/ltmem_stub.cpp)
endif()
add_library(ltmem STATIC ${LTMEM_SOURCES} ${LTMEM_HEADERS})
target_include_directories(ltmem BEFORE PUBLIC
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ltmem PUBLIC lith_platform)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ltmem PRIVATE -fpermissive -Wno-error)
  target_compile_definitions(ltmem PRIVATE _LINUX)
endif()
