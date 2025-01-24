#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"

#include "Si5351.hpp"
#include "ADF4351.hpp"
#include "ImproperFraction.hpp"

const auto I2C_PORT {i2c0};
const uint8_t I2C_SDA {0};
const uint8_t I2C_SCL {1};

const auto SPI_PORT = spi0;
const uint8_t PLL_SPI_SCK = 2; // SCLK
const uint8_t PLL_SPI_TX = 3;  // MOSI
const uint8_t PLL_LE = 5;      // chip select


int main()
{
    // I²C
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // SPI only TX
    gpio_set_function(PLL_SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PLL_SPI_TX,  GPIO_FUNC_SPI);
    spi_init(SPI_PORT, 1000000);

    // ADF4351
    const uint32_t PFD_ADF = 100000; // int-Mode => PFD == step
    const uint8_t R_ADF = 100;

    uint32_t frequency = 451600050;
    uint32_t offsetADF = frequency % PFD_ADF;
    uint32_t frequencyADF = frequency - offsetADF;
    const uint16_t nADF = frequencyADF / PFD_ADF;
    ImproperFraction fRefADF(PFD_ADF * R_ADF, offsetADF * R_ADF, nADF);

    // Si5351 multisynth with 900 MHz VCO
    const uint32_t F_XO_SI {25'000'000};
    const uint32_t F_VCO_SI {900'000'000};

    ImproperFraction mMultisynth = F_VCO_SI / fRefADF;
    const uint32_t ma = mMultisynth.getA();
    const uint32_t mb = mMultisynth.getB();
    const uint32_t mc = mMultisynth.getC();

    Si5351 si5351;
    si5351.setClkControl(0, false, false, 0, false, 3, 8);
    si5351.setPllInputSource(1);
    si5351.setPllParameters('a', F_VCO_SI/F_XO_SI, 0, 15);
    si5351.setMultisynth0to5parameters(0, ma, mb, mc);
    si5351.resetPll();
    si5351.setOutput(0, true);

    // ADF4351
    ADF4351 adf4351(PLL_LE, SPI_PORT);
    adf4351.setN(nADF);

    return 0;
}
