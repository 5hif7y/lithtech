# RegMgr — desacoplado (graphify wt1-2, no crítico para demos)
cmake_minimum_required(VERSION 3.27)
add_library(RegMgr INTERFACE)
message(STATUS "RegMgr desacoplado: Windows Registry -> std::filesystem")
