#include <Si5351.hpp>

void Si5351::waitAfterPowerOn()
{
    uint8_t siRegister = 0;
    uint8_t siData;
    do
    {
        i2c_write_blocking(I2C_PORT, I2C_ADDR, &siRegister, 1, false);
        i2c_read_blocking(I2C_PORT, I2C_ADDR, &siData, 1, false);
    } while ((siData & 0x88) != 0);
}

Si5351::Si5351(i2c_inst i2cPort, uint8_t i2cAddr, uint8_t sda, uint8_t scl, double xtalFreq)
    : I2C_PORT(I2C_PORT), I2C_ADDR(i2cAddr), SDA(sda), SCL(scl), XTAL_FREQ(xtalFreq)
{
    waitAfterPowerOn();
}

void Si5351::outputsOff()
{
    uint8_t allOutputsOff[2] = {3, 0xFF};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, allOutputsOff, 2, false);
}