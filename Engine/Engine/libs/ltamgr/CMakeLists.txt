# ltamgr — portable Linux (graphify wt 0-1, fase2)
# Generado via graphify (ahorro tokens: no leer 145 Libs files, usar idx)
cmake_minimum_required(VERSION 3.27)
file(GLOB LTAMGR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB LTAMGR_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
if(LTAMGR_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ltamgr_stub.cpp "int ltamgr_stub(){return 0;}")
  set(LTAMGR_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/ltamgr_stub.cpp)
endif()
add_library(ltamgr STATIC ${LTAMGR_SOURCES} ${LTAMGR_HEADERS})
target_include_directories(ltamgr BEFORE PUBLIC
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ltamgr PUBLIC lith_platform)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ltamgr PRIVATE -fpermissive -Wno-error)
  target_compile_definitions(ltamgr PRIVATE _LINUX)
endif()
