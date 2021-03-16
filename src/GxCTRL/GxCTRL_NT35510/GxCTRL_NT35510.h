// created by Jean-Marc Zingg to be the GxCTRL_NT35510 class for the GxTFT library
// code extracts taken from http://www.lcdwiki.com/res/Program/Parallel_Port/3.97inch/8_16BIT_NT35510_800x480_MRB3973_V1.2/3.97inch_8&16BIT_Module_NT35510_800x480_MRB3973_V1.2.zip (lcd.h, lcd.c)
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" display from Ruijia Industry
// e.g. https://www.aliexpress.com/item/32676929794.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// this display also matches the FSMC TFT connector of the STM32F407VxT6 board e.g. https://www.aliexpress.com/item/1005001806399987.html
//
// note: this display class uses 16bit commands, for 16 bit solder jumper settings

#ifndef _GxCTRL_NT35510_H_
#define _GxCTRL_NT35510_H_

#include "../GxCTRL.h"

class GxCTRL_NT35510 : public GxCTRL
{
  public:
    GxCTRL_NT35510(GxIO& io) : GxCTRL(io) {};
    const char* name = "GxCTRL_NT35510";
    const uint32_t ID = 0x3551;
    uint32_t readID();
    uint32_t readRegister(uint8_t nr, uint8_t index = 0, uint8_t bytes = 1);
    uint16_t readPixel(uint16_t x, uint16_t y);
    void     readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data);
    void init();
    void setWindowAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setRotation(uint8_t r);
};

#define GxCTRL_Class GxCTRL_NT35510

#endif
