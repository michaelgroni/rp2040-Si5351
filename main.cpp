#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "Si5351.hpp"


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

    Si5351 si5351;
    si5351.setClkControl(0, false, false, 0, false, 3, 8);
    si5351.setPllInputSource(1);
    si5351.setPllParameters('a', 35, 0, 1033);
    si5351.setMultisynth0to5parameters(0, 90, 0, 15);
    si5351.resetPll();
    si5351.setOutput(0, true);

    return 0;
}
