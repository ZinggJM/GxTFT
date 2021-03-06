// created by Jean-Marc Zingg to be the GxIO_STM32F1_FSMC io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F103VC boards with FMSC TFT connector, e.g. for 
// https://www.aliexpress.com/item/STM32-core-development-TFT-LCD-screen-evaluation-board-with-high-speed-FSMC-SDIO-interface/32667841009.html
//
// the above board has matched TFT connector for this display (aka Tiky)
// https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F103V doc.
//
// this io class can be used with or adapted to other STM32F103V processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package STM32GENERIC, works with board "huaweiwx F103Z boards" variants "RedBull(F103ZE)" or "GENERIC(STM32F103ZE)".
// https://github.com/danieleff/STM32GENERIC
// settings used:
// USB: "Disabled, no USB", Serial communication: "SerialUART1", JTAG/SWD debugging: "JTAG [debug pins reserved: PB4,PB3,PA15,PA14,PA13]", Upload method: "STlink"
//
// also tried: DO NOT USE! DO NOT DOWNLOAD! DON'T ignore linker warning! -> flash erase (ST-Link utility) required afterwards!
// Arduino package STM32GENERIC, board "Blue F103VE/ZE/ZG boards" (ST-Link download failure). DO NOT USE! (version downloaded 7.7.2017)

#ifndef _GxIO_STM32GENERIC_STM32F1_FSMC_H_
#define _GxIO_STM32GENERIC_STM32F1_FSMC_H_

#include "../../GxIO.h"

#if defined(ARDUINO_ARCH_STM32) && (defined(STM32F103VE) || defined(STM32F103ZE) || defined(STM32F103ZG))

class GxIO_STM32F1_FSMC : public GxIO
{
  public:
    GxIO_STM32F1_FSMC();
    const char* name = "GxIO_STM32F1_FSMC";
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
    int8_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
};

#define GxIO_Class GxIO_STM32F1_FSMC

#endif

#endif
