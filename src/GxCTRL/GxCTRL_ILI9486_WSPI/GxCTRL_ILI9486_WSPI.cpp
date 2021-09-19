// created by Jean-Marc Zingg to be the GxCTRL_ILI9486_WSPI class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GxCTRL_ILI9486_WSPI.h"

#define ILI9486_CASET 0x2A
#define ILI9486_PASET 0x2B
#define ILI9486_RAMWR 0x2C
#define ILI9486_RAMRD   0x2E
#define ILI9486_MADCTL 0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

uint32_t GxCTRL_ILI9486_WSPI::readID()
{
  return readRegister(0xD3, 0, 3);
}

uint32_t GxCTRL_ILI9486_WSPI::readRegister(uint8_t nr, uint8_t index, uint8_t bytes)
{
  uint32_t rv = 0;
  bytes = min(bytes, 4);
  IO.startTransaction();
  IO.writeCommand(nr);
  IO.readData(); // dummy
  for (uint8_t i = 0; i < index; i++)
  {
    IO.readData(); // skip
  }
  for (; bytes > 0; bytes--)
  {
    rv <<= 8;
    rv |= IO.readData();
  }
  IO.endTransaction();
  return rv;
}

uint16_t GxCTRL_ILI9486_WSPI::readPixel(uint16_t x, uint16_t y)
{
  uint16_t rv;
  readRect(x, y, 1, 1, &rv);
  return rv;
}

void GxCTRL_ILI9486_WSPI::readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  uint32_t num = uint32_t(w) * uint32_t(h);
  IO.startTransaction();
  IO.writeCommand(ILI9486_CASET);  // Column addr set
  IO.writeData(x >> 8);
  IO.writeData(x & 0xFF);  // XSTART
  IO.writeData(xe >> 8);
  IO.writeData(xe & 0xFF); // XEND
  IO.writeCommand(ILI9486_PASET);  // Row addr set
  IO.writeData(y >> 8);
  IO.writeData(y);         // YSTART
  IO.writeData(ye >> 8);
  IO.writeData(ye);        // YEND
  IO.writeCommand(ILI9486_RAMRD);  // read from RAM
  IO.readData(); // dummy
  for (; num > 0; num--)
  {
    *data++ = IO.readData16();
  }
  IO.endTransaction();
}

void GxCTRL_ILI9486_WSPI::init()
{
  //Serial.println("GxCTRL_ILI9486_WSPI::init()");
  // doesn't work below 16MHz or above ?24MHz? default 16MHz would be 10MHz on STM32L476RG
  IO.setFrequency(20000000); // this should work
  IO.startTransaction();
  delay(10); // useful?
  IO.writeCommand16(0x3A);
  IO.writeData16(0x55);  // use 16 bits per pixel color
  IO.writeCommand16(0x36);
  IO.writeData16(0x48);  // MX, BGR == rotation 0
  // PGAMCTRL(Positive Gamma Control)
  IO.writeCommand16(0xE0);
  IO.writeData16(0x0F);
  IO.writeData16(0x1F);
  IO.writeData16(0x1C);
  IO.writeData16(0x0C);
  IO.writeData16(0x0F);
  IO.writeData16(0x08);
  IO.writeData16(0x48);
  IO.writeData16(0x98);
  IO.writeData16(0x37);
  IO.writeData16(0x0A);
  IO.writeData16(0x13);
  IO.writeData16(0x04);
  IO.writeData16(0x11);
  IO.writeData16(0x0D);
  IO.writeData16(0x00);
  // NGAMCTRL(Negative Gamma Control)
  IO.writeCommand16(0xE1);
  IO.writeData16(0x0F);
  IO.writeData16(0x32);
  IO.writeData16(0x2E);
  IO.writeData16(0x0B);
  IO.writeData16(0x0D);
  IO.writeData16(0x05);
  IO.writeData16(0x47);
  IO.writeData16(0x75);
  IO.writeData16(0x37);
  IO.writeData16(0x06);
  IO.writeData16(0x10);
  IO.writeData16(0x03);
  IO.writeData16(0x24);
  IO.writeData16(0x20);
  IO.writeData16(0x00);
  // Digital Gamma Control 1
  IO.writeCommand16(0xE2);
  IO.writeData16(0x0F);
  IO.writeData16(0x32);
  IO.writeData16(0x2E);
  IO.writeData16(0x0B);
  IO.writeData16(0x0D);
  IO.writeData16(0x05);
  IO.writeData16(0x47);
  IO.writeData16(0x75);
  IO.writeData16(0x37);
  IO.writeData16(0x06);
  IO.writeData16(0x10);
  IO.writeData16(0x03);
  IO.writeData16(0x24);
  IO.writeData16(0x20);
  IO.writeData16(0x00);
  IO.writeCommand16(0x11);  // Sleep OUT
  delay(150);   // wait some time
  IO.writeCommand16(0x29);  // Display ON
  IO.endTransaction();
  //Serial.println("GxCTRL_ILI9486_WSPI::init() done..");
}

void GxCTRL_ILI9486_WSPI::setWindowAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  IO.startTransaction();
  IO.writeCommand16(ILI9486_CASET);  // Column addr set
  IO.writeData16(x0 >> 8);
  IO.writeData16(x0 & 0xFF); // XSTART
  IO.writeData16(x1 >> 8);
  IO.writeData16(x1 & 0xFF); // XEND
  IO.writeCommand16(ILI9486_PASET);  // Row addr set
  IO.writeData16(y0 >> 8);
  IO.writeData16(y0);        // YSTART
  IO.writeData16(y1 >> 8);
  IO.writeData16(y1);        // YEND
  IO.writeCommand16(ILI9486_RAMWR);  // write to RAM
}

void GxCTRL_ILI9486_WSPI::setRotation(uint8_t r)
{
  IO.startTransaction();
  IO.writeCommand16(ILI9486_MADCTL);
  switch (r & 3) 
  {
    case 0:
      IO.writeData16(MADCTL_MX | MADCTL_BGR);
      break;
    case 1:
      IO.writeData16(MADCTL_MV | MADCTL_BGR);
      break;
    case 2:
      IO.writeData16(MADCTL_MY | MADCTL_BGR);
      break;
    case 3:
      IO.writeData16(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      break;
  }
  IO.endTransaction();
}
