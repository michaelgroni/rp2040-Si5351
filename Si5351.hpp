#pragma once

#include "hardware/i2c.h"

class Si5351
{
private:
    i2c_inst* I2C_PORT;
    const uint8_t I2C_ADDR;
    const uint8_t SDA;
    const uint8_t SCL; 
    const double XTAL_FREQ;

    /**
     * @ brief Blocks until Si5351 is intialized and CLKIN is valid
     */
    void waitAfterPowerOn();

public:
    /**
     * @param i2cPort is an I2C controller, for example `i2c0`. It must have been initialized.
     * @param i2cAddr is the I2C address.
     * @param sda is a GPIO pin that can be used as SDA with `i2cPort`, for example `0`.
     * @param scl is a GPIO pin that can be used as SCL with `i2cPort`, for example `1`.
     * @param xtalFreq is the crystal frequency in Hz.
     */
    Si5351(i2c_inst* i2cPort = i2c0, uint8_t i2cAddr = 0x60, uint8_t sda = 0, uint8_t scl = 1, double xtalFreq = 25000000);

    /**
     * @brief Disables all output drivers and powers them down.
     */
    void setOutputsOff();

    /**
     * Disables the interrupt pin.
     */
    void disableInterrupts();

    /**
     * Disables the OEB pin.
    */
    void disableOEBPin()

};