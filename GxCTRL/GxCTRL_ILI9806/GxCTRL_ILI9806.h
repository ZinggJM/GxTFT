// created by Jean-Marc Zingg to be the GxCTRL_ILI9806 class for the GxTFT library
// code extracts taken from code and documentation from Ruijia Industry (Tiky_LCD.h, Tiky_LCD.c)
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#ifndef _GxCTRL_ILI9806_H_
#define _GxCTRL_ILI9806_H_

#include "../GxCTRL.h"

class GxCTRL_ILI9806 : public GxCTRL
{
  public:
    GxCTRL_ILI9806(GxIO& io) : GxCTRL(io) {};
    const char* name = "GxCTRL_ILI9806";
    void init();
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setWindowKeepTransaction(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setRotation(uint8_t r);
};

#define GxCTRL_Class GxCTRL_ILI9806

#endif

