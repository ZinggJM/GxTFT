// created by Jean-Marc Zingg to be the GxIO_STM32Nucleo144_P16 io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a wiring for the TFT connector on my Nucleo-144 proto board, matching the pinout of the following processor board:
// https://www.aliexpress.com/item/Free-shipping-STM32F407VET6-development-board-Cortex-M4-STM32-minimum-system-learning-board-ARM-core-board/32618222721.html
//
// e.g. for direct matching display
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), 
// board "Nucleo-144" part "Nucleo F767ZI", or "Nucleo F746ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#ifndef _GxIO_STM32Nucleo144_P16_H_
#define _GxIO_STM32Nucleo144_P16_H_

#include "../../GxIO.h"

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F7xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_NUCLEO_F767ZI) || defined(ARDUINO_NUCLEO_F746ZG)

class GxIO_STM32Nucleo144_P16 : public GxIO
{
  public:
    GxIO_STM32Nucleo144_P16(bool bl_active_high = false);
    const char* name = "GxIO_STM32Nucleo144_P16";
    void reset();
    void init();
    uint8_t readDataTransaction();
    uint16_t readData16Transaction();
    uint8_t readData();
    uint16_t readData16();
    uint32_t readRawData32(uint8_t part); // debug purpose
    void writeCommandTransaction(uint8_t c);
    void writeCommand16Transaction(uint16_t c);
    void writeDataTransaction(uint8_t d);
    void writeData16Transaction(uint16_t d, uint32_t num = 1);
    void writeCommand(uint8_t c);
    void writeCommand16(uint16_t c);
    void writeData(uint8_t d);
    void writeData(uint8_t* d, uint32_t num);
    void writeData16(uint16_t d, uint32_t num = 1);
    void writeAddrMSBfirst(uint16_t d);
    void startTransaction();
    void endTransaction();
    void selectRegister(bool rs_low); // for generalized readData & writeData (RA8875)
    void setBackLight(bool lit);
  private:
    int8_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
    bool _bl_active_high;
};

#define GxIO_Class GxIO_STM32Nucleo144_P16

#endif
#endif

#endif
