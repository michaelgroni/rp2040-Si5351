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

void Si5351::setClkControl(const uint8_t clkIndex, bool powerDown, bool intMode, uint8_t inputSource, bool invert, uint8_t outputSource, uint8_t strength)
{
    uint8_t data[2] = {16, 0x00};

    if (clkIndex <= 7) data[0] += clkIndex;

    if (powerDown) data[1] += 0x80;
    if (intMode) data[1] += 0x40;
    if (inputSource == 1) data[1] += 0x20;
    if (invert) data[1] += 0x10;
    if (outputSource > 0) data[1] += 0x0C;

    switch(strength)
    {
        case 8:
            data[1] += 0x03;
            break;
        case 6:
            data[1] += 0x02;
            break;
        case 4:
            data[1] = 0x01;
            break;
    }
}

void Si5351::setOutputDisableState(uint8_t clkIndex, const uint8_t disState)
{
    uint8_t data[3] = {24, 0x00, 0x00};

    if (clkIndex > 7) clkIndex = 0;

    uint16_t deleteMask = ~(0x03 << clkIndex); // for example 0xF3 to clear the state of CLK1
    uint8_t deleteLow = (uint8_t) deleteMask;
    uint8_t deleteHigh = (uint8_t) (deleteMask >> 8);

    uint16_t dataMask = (disState && 0x03) << clkIndex;
    uint8_t dataLow = (uint8_t) dataMask;
    uint8_t dataHigh = (uint8_t) (dataMask >> 8);

    i2c_write_blocking(I2C_PORT, I2C_ADDR, data, 1, false);
    i2c_read_blocking(I2C_PORT, I2C_ADDR, data+1, 2, false);

    data[1] &= deleteLow;
    data[2] &= deleteHigh;

    data[1] += dataLow;
    data[2] += dataHigh;

    i2c_write_blocking(I2C_PORT, I2C_ADDR, data, 3, false);
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

void Si5351::setPllInputSource(const uint8_t inputDivider, const uint8_t sourceB=0, const uint8_t sourceA=0)
{
    uint8_t data[2] = {15, 0x00};

    switch(inputDivider)
    {
        case 8:
            data[1] = 0xC0;
            break;
        case 4:
            data[1] = 0x80;
            break;
        case 2:
            data[1] = 0x40;
            break;
        case 1:
        default:
            data[1] = 0x00;
    }

    i2c_write_blocking(I2C_PORT, I2C_ADDR, data, 2, false);
}

void Si5351::setPllParameters(const uint8_t pllIndex, const uint32_t integer, const uint32_t numerator, const uint32_t denominator)
{
    uint8_t data[9];

    switch (pllIndex)
    {
        case 0:
            data[0] = 26; // PLL a
            break;
        case 1:
            data[0] = 34; // PLL b
            break;
        default:
            return;
    }

    const uint32_t p1 = 128 * integer + (128 * numerator / denominator) - 512;              // 18 bits
    const uint32_t p2 = 128 * numerator - denominator * (128 * numerator / denominator);    // 20 bits
    const uint32_t p3 = denominator;                                                        // 20 bits

    data[1] = p3 >> 8;                          // register 26 or 34
    data[2] = p3;                               // register 27 or 35
    data[3] = p1 >> 16;                         // register 28 or 36 
    data[4] = p1 >> 8;                          // register 29 or 37
    data[5] = p1;                               // register 30 or 38
    data[6] = ((p3 >> 16) << 4) | (p2 >> 16);   // register 31 or 39
    data[7] = p2 >> 8;                          // register 32 or 40
    data[8]= p2;                                // register 33 or 41
}