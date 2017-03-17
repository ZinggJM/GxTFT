// created by Jean-Marc Zingg to be the GxCTRL_HX8357B class for the GxTFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#ifndef _GxCTRL_HX8357B_H_
#define _GxCTRL_HX8357B_H_

#include "../GxCTRL.h"

class GxCTRL_HX8357B : public GxCTRL
{
  public:
    GxCTRL_HX8357B(GxIO& io) : GxCTRL(io) {};
    const char* name = "GxCTRL_HX8357B";
    void init();
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setWindowKeepTransaction(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setRotation(uint8_t r);
};

#define GxCTRL_Class GxCTRL_HX8357B

#endif


