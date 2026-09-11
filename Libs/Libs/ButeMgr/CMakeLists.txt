# ButeMgr — desacoplado en Linux (MFC CString/CRect/CPoint -> std, hash_set)
# Graphify wt1, pero requiere MFC completo (CString, CRect, CPoint, hash_set)
# Para demos, se usa INTERFACE stub que no compila ButeMgr real
cmake_minimum_required(VERSION 3.27)
add_library(ButeMgr INTERFACE)
target_include_directories(ButeMgr INTERFACE ${CMAKE_SOURCE_DIR}/Platform)
message(STATUS "ButeMgr desacoplado: MFC -> std::string (Linux) - INTERFACE")
