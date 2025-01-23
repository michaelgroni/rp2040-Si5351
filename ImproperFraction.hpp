#pragma once

#include "pico/stdlib.h"

class ImproperFraction
{
private:
    uint32_t a;
    uint32_t b;
    uint32_t c;

    uint32_t gcd(uint32_t x, uint32_t y);

public:
    ImproperFraction(uint32_t a, uint32_t b, uint32_t c);
    ImproperFraction(uint32_t b, uint32_t c); // a = 0
    uint32_t getA();
    uint32_t getB();
    uint32_t getC();
};

ImproperFraction operator/(uint32_t i, ImproperFraction f);