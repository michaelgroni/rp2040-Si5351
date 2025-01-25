#include "ImproperFraction.hpp"

#include <numeric>

ImproperFraction::ImproperFraction(uint32_t a, uint32_t b, uint32_t c)
{
    if (b >= c)
    {
        const auto n = b / c;
        a += n;
        b -= n * c;
    }


    const auto g = gcd(b, c);
    b = b / g;
    c = c / g;

    this->a = a;
    this->b = b;
    this->c = c;
}

ImproperFraction::ImproperFraction(uint32_t b, uint32_t c)
    : ImproperFraction(0, b, c)
{}

uint32_t ImproperFraction::getA() const
{
    return a;
}

uint32_t ImproperFraction::getB() const
{
    return b;
}

uint32_t ImproperFraction::getC() const
{
    return c;
}

ImproperFraction operator/(const uint32_t n, const ImproperFraction& ipf)
{
    const auto a = ipf.getA();
    const auto b = ipf.getB();
    const auto c = ipf.getC();

    uint64_t newB = n; 
    uint64_t newC = ((uint64_t) a*c + b) / c;

    uint32_t newA = 0;
    while (newB >= newC)
    {
        newA++;
        newB = newB -newC;
    }

    const auto g = gcd(newB, newC);
    newB = newB / g;
    newC = newC / g;



    while (newC > 1'048'575) // Si5351 restriction
    {
        newB = newB >> 1;
        newC = newC >> 1;
    }

    return ImproperFraction(newA, (uint32_t) newB, (uint32_t) newC);
}