// created by Jean-Marc Zingg to be the GxCTRL_ST7796S class for the GxTFT library
// code extracts taken from http://www.lcdwiki.com/res/Program/Parallel_Port/4.0inch/8_16BIT_ST7796S_MRB3951_V1.0/4.0inch_8&16BIT_Module_ST7796S_MRB3951_V1.0.zip (lcd.h, lcd.c)
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "4.0 inch HD TFT LCD Screen with PCB Board ILI9488 ST7796S Drive IC 320(RGB)*480 8Bit Parallel Interface (Touch/No Touch)" display from Maithoga
// e.g. https://www.aliexpress.com/item/1899444540.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// this display also matches the FSMC TFT connector of the STM32F407VxT6 board e.g. https://www.aliexpress.com/item/1005001806399987.html

#ifndef _GxCTRL_ST7796S_H_
#define _GxCTRL_ST7796S_H_

#include "../GxCTRL.h"

class GxCTRL_ST7796S : public GxCTRL
{
  public:
    GxCTRL_ST7796S(GxIO& io) : GxCTRL(io) {};
    const char* name = "GxCTRL_ST7796S";
    const uint32_t ID = 0x7796;
    uint32_t readID();
    uint32_t readRegister(uint8_t nr, uint8_t index = 0, uint8_t bytes = 1);
    uint16_t readPixel(uint16_t x, uint16_t y);
    void     readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data);
    void init();
    void setWindowAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setRotation(uint8_t r);
};

#define GxCTRL_Class GxCTRL_ST7796S

#endif
