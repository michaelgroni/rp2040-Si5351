#include "ImproperFraction.hpp"

#include <cmath>

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
    const auto ipfA = ipf.getA();
    const auto ipfB = ipf.getB();
    const auto ipfC = ipf.getC();

    double result = n / (ipfA + (double) ipfB / ipfC);

    uint32_t a = std::floor(result);

    double fraction = result - a;
    uint64_t c = 100'000'000;
    uint64_t b = fraction * 100'000'000;

    auto g = gcd(b, c);
    c = c / g;
    b = b / g;

    return ImproperFraction(a, b, c);
}