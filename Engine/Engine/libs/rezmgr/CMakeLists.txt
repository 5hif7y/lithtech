# rezmgr — portable Linux (graphify wt 0-1, fase2)
# Generado via graphify (ahorro tokens: no leer 145 Libs files, usar idx)
cmake_minimum_required(VERSION 3.27)
file(GLOB REZMGR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB REZMGR_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
if(REZMGR_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/rezmgr_stub.cpp "int rezmgr_stub(){return 0;}")
  set(REZMGR_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/rezmgr_stub.cpp)
endif()
add_library(rezmgr STATIC ${REZMGR_SOURCES} ${REZMGR_HEADERS})
target_include_directories(rezmgr BEFORE PUBLIC
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_CURRENT_SOURCE_DIR}
)
find_package(ZLIB REQUIRED)
target_link_libraries(rezmgr PUBLIC lith_platform ZLIB::ZLIB)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(rezmgr PRIVATE -fpermissive -Wno-error)
  target_compile_definitions(rezmgr PRIVATE _LINUX)
endif()
