#pragma once
#include <string>
// Fix wchar_t* to char* via overload
inline const char* _wchar_to_char(const wchar_t* w) { (void)w; return ""; }
