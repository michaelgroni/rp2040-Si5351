#include <Si5351.hpp>

#include "pico/stdlib.h"

void Si5351::waitAfterPowerOn()
{
    uint8_t siRegister = 0;
    uint8_t siData;
    do
    {
        siData = readByte(0);
    } while ((siData & 0x88) != 0);
}

uint8_t Si5351::readByte(uint8_t reg)
{
    i2c_write_blocking(I2C_PORT, I2C_ADDR, &reg, 1, false);
    uint8_t data;
    i2c_read_blocking(I2C_PORT, I2C_ADDR, &data, 1, false);
    return data;        
}

Si5351::Si5351(i2c_inst *i2cPort, uint8_t i2cAddr, uint8_t sda, uint8_t scl, double xtalFreq)
    : I2C_PORT(I2C_PORT), I2C_ADDR(i2cAddr), SDA(sda), SCL(scl), XTAL_FREQ(xtalFreq)
{
    waitAfterPowerOn();
    setOutputsOff();
    disableInterrupts();
    disableOEBPin();
}

void Si5351::setOutputsOff()
{
    // off
    uint8_t data[2] = {3, 0xFF};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, data, 2, false);

    // power down
    data[1] = 0x8C;
    for (data[0] = 16; data[0] <= 23; data[0]++)
    {
        i2c_write_blocking(I2C_PORT, I2C_ADDR, data, 2, false);
    }
}

void Si5351::disableInterrupts()
{
    uint8_t data[2] = {2, 0xF0};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, data, 2, false);
}

void Si5351::disableOEBPin()
{
    uint8_t data[2] = {9, 0xFF};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, data, 2, false);
}