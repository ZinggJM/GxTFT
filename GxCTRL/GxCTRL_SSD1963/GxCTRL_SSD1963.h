// created by Jean-Marc Zingg to be the GxCTRL_SSD1963 class for the GxTFT library
// code extracts taken from http://www.rinkydinkelectronics.com/download.php?f=UTFT.zip
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#ifndef _GxCTRL_SSD1963_H_
#define _GxCTRL_SSD1963_H_

#include "../GxCTRL.h"

class GxCTRL_SSD1963 : public GxCTRL
{
  public:
    GxCTRL_SSD1963(GxIO& io) : GxCTRL(io), physical_width(800), physical_height(480) {};
    GxCTRL_SSD1963(GxIO& io, uint16_t phy_w, uint16_t phy_h) : GxCTRL(io), physical_width(phy_w), physical_height(phy_w) {};
    const char* name = "GxCTRL_SSD1963";
    void init();
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setWindowKeepTransaction(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void setRotation(uint8_t r);
  private:
    uint8_t rotation;
    uint16_t physical_width, physical_height;
};

#define GxCTRL_Class GxCTRL_SSD1963

#endif

