#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "Si5351.hpp"
#include "ImproperFraction.hpp"

const auto I2C_PORT {i2c0};
const uint8_t I2C_SDA {0};
const uint8_t I2C_SCL {1};


int main()
{
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    const uint32_t PFD_ADF = 100000; // int-Mode => PFD == step
    const uint8_t R_ADF = 100;
    const uint32_t F_VCO_SI = 900'000'000;

    // ADF 4351
    uint32_t frequency = 451600000;
    uint32_t offsetADF = frequency % PFD_ADF;
    uint32_t frequencyADF = frequency - offsetADF;
    uint16_t nADF = frequencyADF / PFD_ADF;
    ImproperFraction fRefADF(PFD_ADF * R_ADF, offsetADF * R_ADF, nADF);

    // Si5351 multisynth with 900 MHz VCO
    ImproperFraction mMultisynth = F_VCO_SI / fRefADF;
    const uint32_t ma = mMultisynth.getA();
    const uint32_t mb = mMultisynth.getB();
    const uint32_t mc = mMultisynth.getC();

    Si5351 si5351;
    si5351.setClkControl(0, false, false, 0, false, 3, 8);
    si5351.setPllInputSource(1);
    si5351.setPllParameters('a', 36, 0, 15);
    si5351.setMultisynth0to5parameters(0, ma, mb, mc);
    si5351.resetPll();
    si5351.setOutput(0, true);

    return 0;
}
