// created by Jean-Marc Zingg to be the GxCTRL base class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#ifndef _GxCTRL_H_
#define _GxCTRL_H_

#include <Arduino.h>
#include "../GxIO/GxIO.h"

class GxCTRL
{
  public:
    GxCTRL(GxIO& io) : IO(io) {};
    const char* name = "GxCTRL";
    virtual void init();
    virtual void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    virtual void setWindowKeepTransaction(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    virtual void setRotation(uint8_t r);
    virtual void invertDisplay(bool i) {IO.writeCommandTransaction(i ? 0x21 : 0x20);};
  protected:
    GxIO& IO;
};

#endif

