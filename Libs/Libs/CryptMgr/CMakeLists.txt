# CryptMgr — portable Linux (graphify wt 0-1, fase2)
# Generado via graphify (ahorro tokens: no leer 145 Libs files, usar idx)
cmake_minimum_required(VERSION 3.27)
file(GLOB CRYPTMGR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB CRYPTMGR_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
if(CRYPTMGR_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/CryptMgr_stub.cpp "int CryptMgr_stub(){return 0;}")
  set(CRYPTMGR_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/CryptMgr_stub.cpp)
endif()
add_library(CryptMgr STATIC ${CRYPTMGR_SOURCES} ${CRYPTMGR_HEADERS})
target_include_directories(CryptMgr BEFORE PUBLIC
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(CryptMgr PUBLIC lith_platform)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(CryptMgr PRIVATE -fpermissive -Wno-error)
  target_compile_definitions(CryptMgr PRIVATE _LINUX)
endif()
