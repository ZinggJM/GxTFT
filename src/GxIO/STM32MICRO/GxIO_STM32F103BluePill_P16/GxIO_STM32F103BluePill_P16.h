// created by Jean-Marc Zingg to be the GxIO_STM32F103BluePill_P16 io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/New-7-inch-TFT-LCD-module-800x480-SSD1963-Touch-PWM-For-Arduino-AVR-STM32-ARM/32667404985.html
//
// for STM32F103C8T6 Minimum System, also known as BluePill
// https://www.aliexpress.com/item/10pcs-lot-STM32F103C8T6-ARM-STM32-Minimum-System-Development-Board-Module-For-Arduino/32581770994.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "BluePill F103C8".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#ifndef _GxIO_STM32F103BluePill_P16_H_
#define _GxIO_STM32F103BluePill_P16_H_

#include "../../GxIO.h"

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"

class GxIO_STM32F103BluePill_P16 : public GxIO
{
  public:
    GxIO_STM32F103BluePill_P16();
    const char* name = "GxIO_STM32F103BluePill_P16";
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
    void writeAddrMSBfirst(uint16_t addr);
    void startTransaction();
    void endTransaction();
    void selectRegister(bool rs_low); // for generalized readData & writeData (RA8875)
    void setBackLight(bool lit);
  private:
    void setDataPinsOutput();
    void setDataPinsInput();
    int16_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
};

#define GxIO_Class GxIO_STM32F103BluePill_P16

#endif

#endif
