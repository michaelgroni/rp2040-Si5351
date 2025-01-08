#include <Si5351.hpp>

#include "pico/stdlib.h"

using namespace std;

array<uint32_t, 3> Si5351::dividerParameters(const uint a, const uint b, const uint c) const
{
    array<uint32_t, 3> p;
    p[0] = 128 * a + (128 * b / c) - 512; // 18 bits
    p[1] = 128 * b - c * (128 * b / c);   // 20 bits
    p[2] = c;                             // 20 bits
    return p;
}

array<uint8_t, 9> Si5351::registerContent(const uint8_t address, const std::array<uint32_t, 3> &p) const
{
    array<uint8_t, 9> data;
    data.at(0) = address;
    data[1] = p.at(2) >> 8;                            
    data[2] = p.at(2);                              
    data[3] = p.at(0) >> 16;                          
    data[4] = p.at(0) >> 8;                           
    data[5] = p.at(0);                                
    data[6] = ((p.at(2) >> 16) << 4) | (p.at(1) >> 16);
    data[7] = p.at(1) >> 8;                             
    data[8] = p.at(1);                                
    return data;
}

array<uint8_t, 9> Si5351::registerContent(const uint8_t address, const uint a, const uint b, const uint c) const
{
    return registerContent(address, dividerParameters(a, b, c));
}

uint8_t Si5351::readByte(uint8_t reg)
{
    i2c_write_blocking(I2C_PORT, I2C_ADDR, &reg, 1, false);
    uint8_t data;
    i2c_read_blocking(I2C_PORT, I2C_ADDR, &data, 1, false);
    return data;        
}

void Si5351::waitAfterPowerOn()
{
    uint8_t siRegister = 0;
    uint8_t siData;
    do
    {
        siData = readByte(0);
    } while ((siData & 0x88) != 0);
}

Si5351::Si5351(i2c_inst *i2cPort, uint8_t i2cAddr, uint8_t sda, uint8_t scl, double xtalFreq)
    : I2C_PORT(I2C_PORT), I2C_ADDR(i2cAddr), SDA(sda), SCL(scl), XTAL_FREQ(xtalFreq)
{
    waitAfterPowerOn();
    setOutputsOff();
    disableInterrupts();
    disableOEBPin();
    
    // disable spread spectrum (might be supported in coming releases)
    uint8_t register149 = readByte(149);
    register149 |= 0x7F;
    i2c_write_blocking(I2C_PORT, I2C_ADDR, &register149, 1, false);
}

void Si5351::disableInterrupts()
{
    array<uint8_t, 2> data {2, 0xF0};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), data.size(), false);
}

void Si5351::disableOEBPin()
{
    array<uint8_t, 2> data {9, 0xFF};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), data.size(), false);
}

void Si5351::setClkControl(const uint8_t clkIndex, bool powerDown, bool intMode, uint8_t inputSource, bool invert, uint8_t outputSource, uint8_t strength)
{
    array<uint8_t, 2> data {16, 0x00};

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

    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), data.size(), false);
}

void Si5351::setOutputDisableState(uint8_t clkIndex, const uint8_t disState)
{
    array<uint8_t, 3> data {24, 0x00, 0x00};

    if (clkIndex > 7) clkIndex = 0;

    uint16_t deleteMask = ~(0x03 << clkIndex); // for example 0xF3 to clear the state of CLK1
    uint8_t deleteLow = (uint8_t) deleteMask;
    uint8_t deleteHigh = (uint8_t) (deleteMask >> 8);

    uint16_t dataMask = (disState && 0x03) << clkIndex;
    uint8_t dataLow = (uint8_t) dataMask;
    uint8_t dataHigh = (uint8_t) (dataMask >> 8);

    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), 1, false);
    i2c_read_blocking(I2C_PORT, I2C_ADDR, data.data()+1, 2, false);

    data[1] &= deleteLow;
    data[2] &= deleteHigh;

    data[1] += dataLow;
    data[2] += dataHigh;

    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), data.size(), false);
}

void Si5351::setMultisynth0to5parameters(const uint8_t multisynth, const uint32_t integer, const uint32_t num, const uint32_t denom, uint8_t outDiv = 0) const
{    
    uint8_t address; // first register to be written

    if (multisynth > 5)
    {
        return;
    }
    else
    {
        address = 42 + multisynth * 8;
    }

    auto data = registerContent(address, integer, num, denom);

    outDiv &= 0x07; // ignore bits left of 2^2
    outDiv << 4;    // shift to the correct position
    data.at(2) |= outDiv;

    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), sizeof(data), false);
}

void Si5351::setOutputsOff()
{
    // off
    array<uint8_t, 2> data {3, 0xFF};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), data.size(), false);

    // power down
    data[1] = 0x8C;
    for (data[0] = 16; data[0] <= 23; data[0]++)
    {
        i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), data.size(), false);
    }
}

void Si5351::setPllInputSource(const uint8_t inputDivider, const uint8_t sourceB=0, const uint8_t sourceA=0)
{
    array<uint8_t, 2> data {15, 0x00};

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

    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), data.size(), false);
}

void Si5351::setPllParameters(const char pll, const uint32_t integer, const uint32_t numerator, const uint32_t denominator)
{
    uint8_t address; // first register to be written

    switch (pll)
    {
        case 'a':
            address = 26; // PLL a
            break;
        case 'b':
            address = 34; // PLL b
            break;
        default:
            return;
    }

    const auto data = registerContent(address, integer, numerator, denominator);
    i2c_write_blocking(I2C_PORT, I2C_ADDR, data.data(), sizeof(data), false);
}