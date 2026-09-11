#pragma once
// LithTech Jupiter - Security hardening para gcc/g++ (Arch)
// Fix para vulnerabilidades detectadas via graphify: 662 strcpy, 1078 sprintf, 399 memcpy, leaks new/delete 4516/2547
#include "platform.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>
#include <string>

// --- Safe string ops: reemplazan strcpy/strcat/sprintf inseguros ---
// Uso: LITH_STRCPY(dst, src)  donde dst es array char[N] o buffer con size conocido
// Para buffers dinamicos, usar LITH_STRNCPY(dst, src, dstSize)

#ifndef LITH_STRCPY
#define LITH_STRCPY(dst, src) do { \
    static_assert(sizeof(dst) != sizeof(void*), "LITH_STRCPY requiere array, no puntero - usar LITH_STRNCPY"); \
    strncpy((dst), (src), sizeof(dst)-1); (dst)[sizeof(dst)-1] = '\0'; \
} while(0)
#endif

#ifndef LITH_STRNCPY
#define LITH_STRNCPY(dst, src, sz) do { \
    if ((sz) > 0) { strncpy((dst), (src), (sz)-1); (dst)[(sz)-1] = '\0'; } \
} while(0)
#endif

#ifndef LITH_STRCAT
#define LITH_STRCAT(dst, src) do { \
    size_t _dlen = strnlen((dst), sizeof(dst)); \
    if (_dlen < sizeof(dst)-1) strncat((dst), (src), sizeof(dst)-_dlen-1); \
} while(0)
#endif

#ifndef LITH_SNPRINTF
#define LITH_SNPRINTF(dst, fmt, ...) do { \
    snprintf((dst), sizeof(dst), (fmt), ##__VA_ARGS__); \
} while(0)
#endif

// Para punteros dinamicos con size explicito
inline void lith_safe_copy(char* dst, size_t dstSz, const char* src) {
    if (!dst || dstSz==0) return;
    if (!src) { dst[0]='\0'; return; }
    strncpy(dst, src, dstSz-1);
    dst[dstSz-1]='\0';
}
inline void lith_safe_cat(char* dst, size_t dstSz, const char* src) {
    if (!dst || !src || dstSz==0) return;
    size_t len = strnlen(dst, dstSz);
    if (len < dstSz-1) strncat(dst, src, dstSz-len-1);
}

// --- RAII para leaks: reemplaza char* new[] / malloc sin free ---
// Ejemplo: auto buf = lith_make_buffer(nFileLength);  // vector<char> con RAII
inline std::vector<char> lith_make_buffer(size_t n) {
    std::vector<char> v(n);
    if (!v.empty()) v[0]='\0';
    return v;
}
// Wrapper para strstream legacy que espera char* raw: usar lith_buffer_data()
inline char* lith_buffer_data(std::vector<char>& v) { return v.data(); }

// --- Overflow check para memcpy ---
inline bool lith_memcpy_checked(void* dst, size_t dstSz, const void* src, size_t count) {
    if (!dst || !src) return false;
    if (count > dstSz) return false; // overflow prevented
    memcpy(dst, src, count);
    return true;
}

// --- Validacion Vulkan/Renderer ---
inline bool lith_validate_ptr(const void* p) { return p != nullptr; }
inline bool lith_validate_size(size_t count, size_t max) { return count <= max; }
