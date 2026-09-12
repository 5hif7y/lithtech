#pragma once
// Linux/Windows portable StackTracer shim (graphify 6 remaining)
#include <cstdint>
#include <cstring>
#include <vector>
struct StackEntry {
    uint64_t ip = 0;
    uint64_t retAddr = 0;
    uint64_t ebp = 0;
};
struct SymbolDetails {
    int ErrorCode = 1;
    char Filename[256] = {0};
    char Name[256] = {0};
    int LineNumber = 0;
};
class StackTrace {
public:
    std::vector<StackEntry> entries;
    size_t size() const { return entries.empty() ? 1 : entries.size(); }
    const StackEntry& operator[](size_t i) const { static StackEntry s; return entries.empty() ? s : entries[i % entries.size()]; }
    StackEntry& operator[](size_t i) { static StackEntry s; return entries.empty() ? s : entries[i % entries.size()]; }
};
class CStackTracer {
public:
    void DoStackTrace(StackTrace& t) { t.entries.push_back(StackEntry()); }
    void LoadSymbolsInDir(const char*, const char*) {}
    void GetSymbolByAddr(SymbolDetails& s, uint64_t) { s.ErrorCode = 1; }
    void UnloadSymbols() {}
};
#define StackTracer CStackTracer
