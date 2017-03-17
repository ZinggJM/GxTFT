/*
  See rights and use declaration in License.h
  This library has been modified for the Maple Mini.
  Includes DMA transfers on DMA1 CH2 and CH3.
*/

// modified by Jean-Marc Zingg to be the GxTFT_GFX class for the GxTFT library
// (the "light" display class using Adafruit_GFX for graphics)
// original source taken from https://github.com/stevstrong/Adafruit_ILI9486_STM32/tree/master

#include "GxTFT_GFX.h"

// Swap any type
template <typename T> static inline void
swap(T& a, T& b)
{
  T t = a;
  a = b;
  b = t;
}

/*****************************************************************************/
// Constructor uses GxIO, the pins being specific to each device
/*****************************************************************************/
GxTFT_GFX::GxTFT_GFX(GxIO& io, GxCTRL& controller, uint16_t w, uint16_t h) : Adafruit_GFX(w, h), IO(io), Controller(controller)
{
  _tft_width = w;
  _tft_height = h;
}
/*****************************************************************************/
void GxTFT_GFX::init()
{
  IO.init();
  Controller.init();
  // match orientation and physical dimensions to constructor dimension
  if (_tft_width > _tft_height)
  {
    Controller.setRotation(1); // landscape
    swap(_tft_width, _tft_height);
  }
  IO.setBackLight(true);
}
/*****************************************************************************/
void GxTFT_GFX::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  Controller.setWindow(x0, y0, x1, y1);
}
/*****************************************************************************/
void GxTFT_GFX::pushColor(uint16_t color)
{
  IO.writeData16(color);
}
/*****************************************************************************/
void GxTFT_GFX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
  Controller.setWindowKeepTransaction(x, y, x, y);
  IO.writeData16(color);
  IO.endTransaction();
}
/*****************************************************************************/
void GxTFT_GFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height || h < 1)) return;
  if ((y + h - 1) >= _height)
  {
    h = _height - y;
  }
  Controller.setWindowKeepTransaction(x, y, x, y + h - 1);
  IO.writeData16(color, h);
  IO.endTransaction();
}
/*****************************************************************************/
void GxTFT_GFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height || w < 1)) return;
  if ((x + w - 1) >= _width)
  {
    w = _width - x;
  }
  Controller.setWindowKeepTransaction(x, y, x + w - 1, y);
  IO.writeData16(color, w);
  IO.endTransaction();
}

/*****************************************************************************/
void GxTFT_GFX::fillScreen(uint16_t color)
{
  Controller.setWindowKeepTransaction(0, 0, _width - 1, _height - 1);
  IO.writeData16(color, uint32_t(_width) * uint32_t(_height));
  IO.endTransaction();
}

/*****************************************************************************/
void GxTFT_GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  // rudimentary clipping (drawChar w/big text requires this)
  if ((x >= _width) || (y >= _height || h < 1 || w < 1)) return;
  if ((x + w - 1) >= _width)
  {
    w = _width  - x;
  }
  if ((y + h - 1) >= _height)
  {
    h = _height - y;
  }
  Controller.setWindowKeepTransaction(x, y, x + w - 1, y + h - 1);
  IO.writeData16(color, uint32_t(w) * uint32_t(h));
  IO.endTransaction();
}

/*
  Draw lines faster by calculating straight sections and drawing them with fastVline and fastHline.
*/
/*****************************************************************************/
void GxTFT_GFX::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  if ((y0 < 0 && y1 < 0) || (y0 > _height && y1 > _height)) return;
  if ((x0 < 0 && x1 < 0) || (x0 > _width && x1 > _width)) return;
  if (x0 < 0) x0 = 0;
  if (x1 < 0) x1 = 0;
  if (y0 < 0) y0 = 0;
  if (y1 < 0) y1 = 0;

  if (y0 == y1) {
    if (x1 > x0) {
      drawFastHLine(x0, y0, x1 - x0 + 1, color);
    }
    else if (x1 < x0) {
      drawFastHLine(x1, y0, x0 - x1 + 1, color);
    }
    else {
      drawPixel(x0, y0, color);
    }
    return;
  }
  else if (x0 == x1) {
    if (y1 > y0) {
      drawFastVLine(x0, y0, y1 - y0 + 1, color);
    }
    else {
      drawFastVLine(x0, y1, y0 - y1 + 1, color);
    }
    return;
  }

  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  }
  else {
    ystep = -1;
  }

  int16_t xbegin = x0;
  if (steep) {
    for (; x0 <= x1; x0++) {
      err -= dy;
      if (err < 0) {
        int16_t len = x0 - xbegin;
        if (len) {
          drawFastVLine (y0, xbegin, len + 1, color);
          //writeVLine_cont_noCS_noFill(y0, xbegin, len + 1);
        }
        else {
          drawPixel(y0, x0, color);
          //writePixel_cont_noCS(y0, x0, color);
        }
        xbegin = x0 + 1;
        y0 += ystep;
        err += dx;
      }
    }
    if (x0 > xbegin + 1) {
      //writeVLine_cont_noCS_noFill(y0, xbegin, x0 - xbegin);
      drawFastVLine(y0, xbegin, x0 - xbegin, color);
    }

  }
  else {
    for (; x0 <= x1; x0++) {
      err -= dy;
      if (err < 0) {
        int16_t len = x0 - xbegin;
        if (len) {
          drawFastHLine(xbegin, y0, len + 1, color);
          //writeHLine_cont_noCS_noFill(xbegin, y0, len + 1);
        }
        else {
          drawPixel(x0, y0, color);
          //writePixel_cont_noCS(x0, y0, color);
        }
        xbegin = x0 + 1;
        y0 += ystep;
        err += dx;
      }
    }
    if (x0 > xbegin + 1) {
      //writeHLine_cont_noCS_noFill(xbegin, y0, x0 - xbegin);
      drawFastHLine(xbegin, y0, x0 - xbegin, color);
    }
  }
}
/*****************************************************************************/
// Pass 8-bit (each) R,G,B, get back 16-bit packed color
/*****************************************************************************/
uint16_t GxTFT_GFX::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
/*****************************************************************************/
void GxTFT_GFX::setRotation(uint8_t r)
{
  Controller.setRotation(r);
  switch (r & 3)
  {
    case 0:
      _width  = _tft_width;
      _height = _tft_height;
      break;
    case 1:
      _width  = _tft_height;
      _height = _tft_width;
      break;
    case 2:
      _width  = _tft_width;
      _height = _tft_height;
      break;
    case 3:
      _width  = _tft_height;
      _height = _tft_width;
      break;
  }
}
/*****************************************************************************/
void GxTFT_GFX::invertDisplay(boolean i)
{
  Controller.invertDisplay(i);
}

