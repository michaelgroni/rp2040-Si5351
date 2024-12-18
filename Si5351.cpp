#include <Si5351.hpp>

Si5351::Si5351(i2c_inst i2cPort, uint8_t sda, uint8_t scl, double xtalFreq)
:I2C_PORT(i2cPort), SDA(sda), SCL(scl), XTAL_FREQ(xtalFreq)
{}