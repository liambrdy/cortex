#ifndef __CORTEX_MATH_H
#define __CORTEX_MATH_H

#include "cortex_platform.h"

union v2
{
    struct
    {
        real32 x, y;
    };

    real32 e[2];
};

inline v2 V2(real32 x, real32 y)
{
    v2 result;

    result.x = x;
    result.y = y;

    return result;
}

inline v2 operator*(real32 a, v2 b)
{
    v2 result;

    result.x = a * b.x;
    result.y = a * b.y;

    return result;
}

inline v2 operator*(v2 b, real32 a)
{
    v2 result;

    result.x = a * b.x;
    result.y = a * b.y;

    return result;
}

inline v2 &operator*=(v2 &a, real32 b)
{
    a = b * a;

    return a;
}

inline v2 operator-(v2 a)
{
    v2 result;

    result.x = -a.x;
    result.y = -a.y;

    return result;
}

inline v2 operator+(v2 a, v2 b)
{
    v2 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

inline v2 &operator+=(v2 &a, v2 b)
{
    a = a + b;

    return a;
}

inline v2 operator-(v2 a, v2 b)
{
    v2 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

union v4
{
    struct
    {
        real32 x, y, z, w;
    };

    real32 e[4];
};

inline v4 V4(real32 x, real32 y, real32 z, real32 w)
{
    v4 result;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    return result;
}

inline v4 operator*(real32 a, v4 b)
{
    v4 result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    result.w = a * b.w;

    return result;
}

inline v4 operator*(v4 b, real32 a)
{
    v4 result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    result.w = a * b.w;

    return result;
}

inline v4 &operator*=(v4 &a, real32 b)
{
    a = b * a;

    return a;
}

inline v4 operator-(v4 a)
{
    v4 result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = -a.w;

    return result;
}

inline v4 operator+(v4 a, v4 b)
{
    v4 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;

    return result;
}

inline v4 &operator+=(v4 &a, v4 b)
{
    a = a + b;

    return a;
}

inline v4 operator-(v4 a, v4 b)
{
    v4 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;

    return result;
}

struct rectangle2
{
    v2 min;
    v2 max;
};

#endif