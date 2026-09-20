#ifndef __LTINTEGER_H__
#define __LTINTEGER_H__
#include <cstdint>
#ifndef __int64
#ifndef _WIN32
// gcc/Clang Linux: map MSVC __int64 to int64_t, but keep unsigned handling via ifdef above.
// (On Windows __int64 is a builtin keyword: redefining it as a macro breaks
// <cstdint> types, intrin.h and every 'unsigned __int64' below.)
#define __int64 int64_t
#endif
#endif


/*!
Portable integer types. 
*/
typedef unsigned int        uint; // This is at least 16 bits

typedef char			int8;
typedef short int		int16;
typedef int				int32;

#if defined(__LINUX) || defined(_LINUX)
typedef long long       int64;
#else
typedef __int64         int64;
#endif

typedef unsigned char		uint8;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;

#if defined(__LINUX) || defined(_LINUX)
typedef unsigned long long  uint64;
#else
typedef unsigned __int64    uint64;
#endif

#endif
