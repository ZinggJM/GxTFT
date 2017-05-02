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
    // controller specific identification
    const char* name = "GxCTRL";
    const uint32_t ID = 0x0000;
    // read functions are controller specific and optional
    virtual uint32_t readID() {return 0;};
    virtual uint32_t readRegister(uint8_t nr, uint8_t index = 0, uint8_t bytes = 1) {return 0;};
    virtual uint16_t readPixel(uint16_t x, uint16_t y) {return 0;};
    virtual void     readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data) {};
    // the controller specific methods, must be implemented in the subclasses
    virtual void init() = 0;
    virtual void setWindowAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) = 0;
    virtual void setRotation(uint8_t r) = 0;
    // controller specific method(s) that may need be called, e.g. reset clipping
    virtual void clearWindowAddress() {}; // reset any clipping by controller
    // the bare minimum, to allow optimization
    virtual void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    virtual void drawPixel(uint16_t x, uint16_t y, uint16_t color);
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    // the methods an advanced controller may implement directly
    virtual bool drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {return false;};
    virtual bool fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)  {return false;};
    virtual bool drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {return false;};
    virtual bool fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {return false;};
    virtual bool drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) {return false;};
    virtual bool fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) {return false;};
    virtual bool drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color) {return false;};
    virtual bool fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color) {return false;};
    virtual bool drawCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color) {return false;};
    virtual bool fillCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color) {return false;};
    // nice to have, controller specific
    virtual void invertDisplay(bool i);
  protected:
    GxIO& IO;
};

#endif

