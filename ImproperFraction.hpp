#pragma once

#include "pico/stdlib.h"

template <typename T>
T gcd(T x, T y)
{
    if (x == 0)
        return y;
    if (y == 0)
        return x;
    if (x < y)
        return gcd(y, x);
    return gcd(y, x % y);
}

class ImproperFraction
{
private:
    uint32_t a;
    uint32_t b;
    uint32_t c;

public:
    ImproperFraction(uint32_t a, uint32_t b, uint32_t c);
    ImproperFraction(uint32_t b, uint32_t c); // a = 0
    uint32_t getA() const;
    uint32_t getB() const;
    uint32_t getC() const;
};

ImproperFraction operator/(const uint32_t n, const ImproperFraction& ipf);

