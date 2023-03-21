#ifndef __CORTEX_PLATFORM_H
#define __CORTEX_PLATFORM_H

#include <string>
#include <stdint.h>
#include <limits>
#include <float.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#define internal static
#define global static
#define local_persist static

#define PIf 3.14159265358979323846264

#ifdef CORTEX_SLOW
#define Assert(expression) if(!(expression)) { volatile int *p = 0; *p = 0; }
#else
#define Assert(expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath");

#define Kilobytes(value) ((value) * 1024LL)
#define Megabytes(value) (Kilobytes(value) * 1024LL)
#define Gigabytes(value) (Megabytes(value) * 1024LL)

struct game_memory
{
    bool isInitialized;

    uint64 permanentStorageSize;
    void *permanentStorage;
};

#define GAME_UPDATE(name) void name(game_memory *memory)
typedef GAME_UPDATE(game_update);

#endif