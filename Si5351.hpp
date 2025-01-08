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
     * @brief Blocks until Si5351 is intialized and CLKIN is valid
     */
    void waitAfterPowerOn();

    /**
     * @brief Reads a single byte blocking.
     * @param reg ist the register to read from.
     * 
     */
    uint8_t readByte(uint8_t register);

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
     * Disables the interrupt pin.
     */
    void disableInterrupts();

    /**
     * Disables the OEB pin.
    */
    void disableOEBPin();

    /**
     * @param clkIndex must be between 0 and 7. Other values are treated like 0.
     * @param powerDown sets the mode of the output driver.
     * @param intMode sets the multisynth int mode.
     * @param inputSource must be 0 (PLLA) or 1 (PLLB on Si5351A/C or VCXO on Si5351B). Other values are treated like 0.
     * @param invert inverts the output clock if set to true.
     * @param outputSource must be 0 (XTAL) or 3 (multisynth). Other values are treated like 3.
     * @param strenth must be 2, 4, 6 or 8. Other values are treated like 2.
     */
    void setClkControl(const uint8_t clkIndex, bool powerDown, bool intMode, uint8_t inputSource, bool invert, uint8_t outputSource, uint8_t strength);

    /**
     * @brief Sets the PLL dividers.
     * @param pllIndex must be 0 (PLL A) or 1 (PLL B). Other values are ignored.
     * @param integer is an unsigned 18-bit-number.
     * @param numerator is an unsigned 20-bit-number.
     * @param denominator is an unsigned 20-bit-number.
     */
    void setPllParameters(const uint8_t pllIndex, const uint32_t integer, const uint32_t numerator, const uint32_t denominator);

    /**
     * @brief Determins the state of a disabled output.
     * @param clkIndex must be between 0 and 7. Other values are treated like 0.
     * @param disState must be 0 (low), 1 (high), 2 (high Z) or 3 (never disabled). Higher bits are ignored.
     */
    void setOutputDisableState(uint8_t clkIndex, const uint8_t disState);

    /**
     * @brief Disables all output drivers and powers them down.
     */
    void setOutputsOff();

    /**
     * @brief Sets the clock input divider.
     * The parameters for the pll sources are ignored because
     * at the moment only the Si5351A ist supported.
     * @param inputDivider must be 1, 2, 4 or 8. Other values are treated like 1.
     */
    void setPllInputSource(const uint8_t inputDivider, const uint8_t sourceB=0, const uint8_t sourceA=0);
};