#include "ADF4351.hpp"

void ADF4351::setN(const uint_least16_t n)
{
   if (this->n != n)
   {
        this->n = n;

        // write R5
        uint8_t r5[] = {0x00, 0x58, 0x00, 0x05};
        writePLL(r5);

        // write R4
        uint8_t r4[] = {0x00, 0x30, 0x10, 0x3C};
        writePLL(r4);
        
  
        // write R3
        uint8_t r3[] = {0x00, 0x40, 0x04, 0xB3};
        writePLL(r3);
    
        // write R2
        uint8_t r2[] = {0x18, 0x19, 0x0F, 0xC2};
        writePLL(r2);        
  
        // write R1
        uint8_t r1[] = {0x08, 0x00, 0x80, 0x11};
        writePLL(r1);
       
        // write R0
        uint32_t r0value = ((uint32_t) n) << 15;
        uint8_t r0[4];
        uint8_t* fake8bit = (uint8_t*) &r0value;
        for (size_t i = 0; i<4; i++)
        {
            r0[3-i] = *(fake8bit+i);
        }
        writePLL(r0);
   }
}

ADF4351::ADF4351(const uint8_t pllOutLE, spi_inst_t* spiPort)
:PLL_OUT_LE(pllOutLE), spiPort(spiPort)
{
    // chip select
    gpio_init(PLL_OUT_LE);
    gpio_set_dir(PLL_OUT_LE, true); 
    gpio_put(PLL_OUT_LE, 1);
}


void ADF4351::writePLL(const uint8_t *values)
{
    gpio_put(PLL_OUT_LE, 0);
    spi_write_blocking(spiPort, values, 4);
    gpio_put(PLL_OUT_LE, 1);
}