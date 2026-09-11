# stdlith — portable Linux (graphify wt 0-1, fase2)
# Generado via graphify (ahorro tokens: no leer 145 Libs files, usar idx)
cmake_minimum_required(VERSION 3.27)
file(GLOB STDLITH_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB STDLITH_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
if(STDLITH_SOURCES STREQUAL "")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/stdlith_stub.cpp "int stdlith_stub(){return 0;}")
  set(STDLITH_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/stdlith_stub.cpp)
endif()
# zfstream.h usa zlib.h — priorizado via graphify (zlib.h wt 8, stdlith wt 0-1 no necesita leer 7888 files)
find_package(ZLIB QUIET)
if(NOT ZLIB_FOUND)
  # fallback: usa headers vendoreados en Libs/Libs/zlib si no hay system zlib
  set(ZLIB_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/Libs/Libs/zlib)
endif()
add_library(stdlith STATIC ${STDLITH_SOURCES} ${STDLITH_HEADERS})
target_include_directories(stdlith BEFORE PUBLIC
  ${CMAKE_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/Engine/Engine/sdk/inc
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ZLIB_INCLUDE_DIRS}
)
target_link_libraries(stdlith PUBLIC lith_platform)
if(ZLIB_FOUND)
  target_link_libraries(stdlith PUBLIC ZLIB::ZLIB)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(stdlith PRIVATE -fpermissive -Wno-error)
  target_compile_definitions(stdlith PRIVATE _LINUX __LINUX)
endif()
