#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#if defined(_MSC_VER)
  #include <io.h>
#else
  #include <unistd.h>
#endif

typedef uint64_t ULL;
typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t  U8;
typedef int64_t  I64;
typedef int32_t  I32;
typedef int16_t  I16;
typedef int8_t   I8;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define RELEASE                1
#define VERSION                1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define BUFFER_SIZE            65535

#define DEF_HELP               0
#define DEF_VERSION            0
#define DEF_VERBOSE            0
#define DEF_FORCE              0
#define DEF_THREAD             4
#define DEF_ALPHA_SIZE         2
#define DEF_MAXT               10000

#define MIN_THREAD             1
#define MAX_THREAD             65535
#define MIN_ALPHA_SIZE         2
#define MAX_ALPHA_SIZE         255
#define MIN_MAXT               1
#define MAX_MAXT               99999999
#define MAX_STR                4098

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

