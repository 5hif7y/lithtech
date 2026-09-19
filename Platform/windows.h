#pragma once
// Dispatcher: Platform/ shadows the SDK on the include path, so a plain
// <windows.h> always lands here. On real Windows forward to the SDK header
// by absolute path (LITH_REAL_WINDOWS_H, set by CMake); elsewhere use the
// Linux shim (same-dir quoted include => order-independent).
#include "platform.h"
#ifdef _WIN32
#ifndef LITH_REAL_WINDOWS_H
#error "Platform/windows.h: LITH_REAL_WINDOWS_H not defined (configure via CMake)"
#endif
#include LITH_REAL_WINDOWS_H
#else
#include "windows_linux.h"
#endif
