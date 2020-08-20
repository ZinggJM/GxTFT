// created by Jean-Marc Zingg to be the GxIO_STM32F4_FSMC io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F407V, STM32F407Z boards with FMSC TFT connector, e.g. for 
// https://www.aliexpress.com/item/Free-shipping-STM32F407VET6-development-board-Cortex-M4-STM32-minimum-system-learning-board-ARM-core-board/32618222721.html
//
// e.g. for display with matching connector
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F407V doc.
//
// this io class can be used with or adapted to other STM32F407V/Z processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), 
// board "Generic STM32F4 series" part "BLACK F407VE", or "BLACK F407VG", or "BLACK F407ZE", or "BLACK F407ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#ifndef _GxIO_STM32GENERIC_STM32F4_FSMC_H_
#define _GxIO_STM32GENERIC_STM32F4_FSMC_H_

#include "../../GxIO.h"

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F407xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_BLACK_F407VE) || defined(ARDUINO_BLACK_F407VG) || defined(ARDUINO_BLACK_F407ZE) || defined(ARDUINO_BLACK_F407ZG)

class GxIO_STM32F4_FSMC : public GxIO
{
  public:
    GxIO_STM32F4_FSMC(bool bl_active_high = true);
    const char* name = "GxIO_STM32F4_FSMC";
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

#define GxIO_Class GxIO_STM32F4_FSMC

#endif
#endif

#endif
