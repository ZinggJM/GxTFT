// created by Jean-Marc Zingg to be the GxCTRL_ILI9486 class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#ifndef _GxCTRL_ILI9486_H_
#define _GxCTRL_ILI9486_H_

#include "../GxCTRL.h"

class GxCTRL_ILI9486 : public GxCTRL
{
  public:
    GxCTRL_ILI9486(GxIO& io) : GxCTRL(io) {};
    const char* name = "GxCTRL_ILI9486";
    void init();
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setWindowKeepTransaction(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setRotation(uint8_t r);
};

#define GxCTRL_Class GxCTRL_ILI9486

#endif

