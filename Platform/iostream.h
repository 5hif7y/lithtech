#pragma once
// Shim VC6 <iostream.h> -> C++17 <iostream>
#include <iostream>
#include <iomanip>
#include <fstream>
// VC6 iostream.h put everything in global, modern in std::
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::istream;
using std::ostream;
using std::iostream;
using std::fstream;
using std::ifstream;
using std::ofstream;
