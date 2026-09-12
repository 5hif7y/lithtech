#pragma once
#include <string>
class CStackTracer { public: static void Trace(const char*) {} };
#define StackTracer CStackTracer
#define StackTrace CStackTracer
