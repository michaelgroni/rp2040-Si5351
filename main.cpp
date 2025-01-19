#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "Si5351.hpp"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
const auto I2C_PORT {i2c0};
const uint8_t I2C_SDA {0};
const uint8_t I2C_SCL {1};



int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    Si5351 si5351;

    si5351.setPllInputSource(1);
    si5351.setPllParameters('a', 36, 0, 1);
    si5351.setMultisynth0to5parameters(0, 90, 0, 2);
    si5351.resetPll();
    si5351.setClkControl(0, false, false, 0, false, 3, 8);
}
