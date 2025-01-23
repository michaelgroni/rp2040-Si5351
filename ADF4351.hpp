#pragma once

#include "pico/stdlib.h"

class ADF4351
{
public:
    ADF4351(const uint8_t pllOutLE); 
    void write(const uint32_t frequency);

private:
    const uint8_t PLL_OUT_LE; // SPI chip select
    uint32_t oldPllFrequency = 0;

    uint32_t pllFrequency(uint32_t frequency) const;
    void writePLL(const uint8_t* values);
};
