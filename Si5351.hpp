#pragma once

#include "hardware/i2c.h"

class Si5351
{
private:
    const i2c_inst I2C_PORT;
    const uint8_t SDA;
    const uint8_t SCL; 
    const double XTAL_FREQ;

public:
    /**
     * @param i2cPort is an I2C controller, for example `i2c0`. It must have been initialized.
     * @param sda is a GPIO pin that can be used as SDA with `i2cPort`, for example `0`.
     * @param scl is a GPIO pin that can be used as SCL with `i2cPort`, for example `1`.
     * @param xtalFreq is the crystal frequency in Hz.
     */
    Si5351(i2c_inst i2cPort, uint8_t sda, uint8_t scl, double xtalFreq);
};