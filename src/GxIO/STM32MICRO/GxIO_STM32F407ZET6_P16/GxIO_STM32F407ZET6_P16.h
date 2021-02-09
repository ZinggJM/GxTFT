// created by Jean-Marc Zingg to be the GxIO_STM32F407ZET6_P16 io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a wiring for the TFT connector of the black STM32F407ZET6 board, e.g.
// https://www.aliexpress.com/item/32689262341.html
// for details of the board see https://github.com/mcauser/BLACK_F407ZE
//
// e.g. for direct matching display
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), 
// board "Generic STM32F4 series" part "BLACK F407VE", or "BLACK F407VG", or "BLACK F407ZE", or "BLACK F407ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#ifndef _GxIO_STM32F407ZET6_P16_H_
#define _GxIO_STM32F407ZET6_P16_H_

#include "../../GxIO.h"

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F407xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_BLACK_F407ZE) || defined(ARDUINO_BLACK_F407ZG)

class GxIO_STM32F407ZET6_P16 : public GxIO
{
  public:
    GxIO_STM32F407ZET6_P16(bool bl_active_high = true);
    const char* name = "GxIO_STM32F407ZET6_P16";
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

#define GxIO_Class GxIO_STM32F407ZET6_P16

#endif
#endif

#endif
