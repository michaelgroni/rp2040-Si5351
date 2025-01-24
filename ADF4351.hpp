#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"

class ADF4351
{
public:
    ADF4351(const uint8_t pllOutLE, spi_inst_t* spiPort); 
    void setN(const uint_least16_t n);

private:
    const uint8_t PLL_OUT_LE; // SPI chip select
    spi_inst_t* spiPort;
    uint_least16_t n {0};

    uint32_t pllFrequency(uint32_t frequency) const;
    void writePLL(const uint8_t* values);
};
