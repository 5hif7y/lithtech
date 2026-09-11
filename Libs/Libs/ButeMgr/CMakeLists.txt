# ButeMgr — portable Linux (graphify wt 0-1, fase2)
# Generado via graphify (ahorro tokens: no leer 145 Libs files, usar idx)
cmake_minimum_required(VERSION 3.27)
file(GLOB BUTEMGR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB BUTEMGR_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
if(BUTEMGR_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ButeMgr_stub.cpp "int ButeMgr_stub(){return 0;}")
  set(BUTEMGR_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/ButeMgr_stub.cpp)
endif()
add_library(ButeMgr STATIC ${BUTEMGR_SOURCES} ${BUTEMGR_HEADERS})
target_include_directories(ButeMgr BEFORE PUBLIC
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ButeMgr PUBLIC lith_platform stdlith lith)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ButeMgr PRIVATE -fpermissive -Wno-error)
  target_compile_definitions(ButeMgr PRIVATE _LINUX)
endif()
