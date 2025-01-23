#include "ImproperFraction.hpp"

#include <numeric>

uint32_t ImproperFraction::gcd(uint32_t x, uint32_t y)
{
    if (x == 0)
        return y;
    if (y == 0)
        return x;
    if (x < y)
        return gcd(y, x);
    return gcd(y, x % y);
}

ImproperFraction::ImproperFraction(uint32_t a, uint32_t b, uint32_t c)
{
    if (b > c)
    {
        const auto n = b / c;
        a += n;
        b -= n * c;
    }

    if (b > 3 && c > 3)
    {
        const auto g = gcd(b, c);
        b = b / g;
        c = c / g;
    }

    this->a = a;
    this->b = b;
    this->c = c;
}

ImproperFraction::ImproperFraction(uint32_t b, uint32_t c)
    : ImproperFraction(0, b, c)
{}

uint32_t ImproperFraction::getA()
{
    return a;
}

uint32_t ImproperFraction::getB()
{
    return b;
}

uint32_t ImproperFraction::getC()
{
    return c;
}

ImproperFraction operator/(uint32_t n, ImproperFraction ipf)
{
    const auto a = ipf.getA();
    const auto b = ipf.getB();
    const auto c = ipf.getC();

    uint64_t newB = n * c;
    uint64_t newC = a * c + b;

    uint32_t newA = 0;
    while (newB >= newC)
    {
        newA++;
        newB = newB -newC;
    }

    while (newC > 1'048'575) // Si5351 restriction
    {
        newB = newB >> 1;
        newC = newC >> 1;
    }

    return ImproperFraction(newA, (uint32_t) newB, (uint32_t) newC);
}