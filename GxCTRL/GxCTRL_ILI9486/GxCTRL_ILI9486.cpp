// created by Jean-Marc Zingg to be the GxCTRL_ILI9486 class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#include "GxCTRL_ILI9486.h"

#define ILI9486_CASET 0x2A
#define ILI9486_PASET 0x2B
#define ILI9486_RAMWR 0x2C
#define ILI9486_MADCTL 0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void GxCTRL_ILI9486::init()
{
  // this controller seems to need separate transactions; reason speed on SPI ?
  //Serial.println("GxCTRL_ILI9486::init()");
  IO.writeCommandTransaction(0x3A);
  IO.writeDataTransaction(0x55);  // use 16 bits per pixel color
  IO.writeCommandTransaction(0x36);
  IO.writeDataTransaction(0x48);  // MX, BGR == rotation 0
  // PGAMCTRL(Positive Gamma Control)
  IO.writeCommandTransaction(0xE0);
  IO.writeDataTransaction(0x0F);
  IO.writeDataTransaction(0x1F);
  IO.writeDataTransaction(0x1C);
  IO.writeDataTransaction(0x0C);
  IO.writeDataTransaction(0x0F);
  IO.writeDataTransaction(0x08);
  IO.writeDataTransaction(0x48);
  IO.writeDataTransaction(0x98);
  IO.writeDataTransaction(0x37);
  IO.writeDataTransaction(0x0A);
  IO.writeDataTransaction(0x13);
  IO.writeDataTransaction(0x04);
  IO.writeDataTransaction(0x11);
  IO.writeDataTransaction(0x0D);
  IO.writeDataTransaction(0x00);
  // NGAMCTRL(Negative Gamma Control)
  IO.writeCommandTransaction(0xE1);
  IO.writeDataTransaction(0x0F);
  IO.writeDataTransaction(0x32);
  IO.writeDataTransaction(0x2E);
  IO.writeDataTransaction(0x0B);
  IO.writeDataTransaction(0x0D);
  IO.writeDataTransaction(0x05);
  IO.writeDataTransaction(0x47);
  IO.writeDataTransaction(0x75);
  IO.writeDataTransaction(0x37);
  IO.writeDataTransaction(0x06);
  IO.writeDataTransaction(0x10);
  IO.writeDataTransaction(0x03);
  IO.writeDataTransaction(0x24);
  IO.writeDataTransaction(0x20);
  IO.writeDataTransaction(0x00);
  // Digital Gamma Control 1
  IO.writeCommandTransaction(0xE2);
  IO.writeDataTransaction(0x0F);
  IO.writeDataTransaction(0x32);
  IO.writeDataTransaction(0x2E);
  IO.writeDataTransaction(0x0B);
  IO.writeDataTransaction(0x0D);
  IO.writeDataTransaction(0x05);
  IO.writeDataTransaction(0x47);
  IO.writeDataTransaction(0x75);
  IO.writeDataTransaction(0x37);
  IO.writeDataTransaction(0x06);
  IO.writeDataTransaction(0x10);
  IO.writeDataTransaction(0x03);
  IO.writeDataTransaction(0x24);
  IO.writeDataTransaction(0x20);
  IO.writeDataTransaction(0x00);
  IO.writeCommandTransaction(0x11);  // Sleep OUT
  delay(150);   // wait some time
  IO.writeCommandTransaction(0x29);  // Display ON
  //Serial.println("GxCTRL_ILI9486::init() done..");
}

void GxCTRL_ILI9486::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  // this controller seems to need separate transactions; reason speed on SPI ?
  IO.writeCommandTransaction(ILI9486_CASET);  // Column addr set
  IO.writeDataTransaction(x0 >> 8);
  IO.writeDataTransaction(x0 & 0xFF); // XSTART
  IO.writeDataTransaction(x1 >> 8);
  IO.writeDataTransaction(x1 & 0xFF); // XEND
  IO.writeCommandTransaction(ILI9486_PASET);  // Row addr set
  IO.writeDataTransaction(y0 >> 8);
  IO.writeDataTransaction(y0);        // YSTART
  IO.writeDataTransaction(y1 >> 8);
  IO.writeDataTransaction(y1);        // YEND
  IO.writeCommandTransaction(ILI9486_RAMWR);  // write to RAM
}

void GxCTRL_ILI9486::setWindowKeepTransaction(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  // this controller seems to need separate transactions; reason speed on SPI ?
  IO.writeCommandTransaction(ILI9486_CASET);  // Column addr set
  IO.writeDataTransaction(x0 >> 8);
  IO.writeDataTransaction(x0 & 0xFF); // XSTART
  IO.writeDataTransaction(x1 >> 8);
  IO.writeDataTransaction(x1 & 0xFF); // XEND
  IO.writeCommandTransaction(ILI9486_PASET);  // Row addr set
  IO.writeDataTransaction(y0 >> 8);
  IO.writeDataTransaction(y0);        // YSTART
  IO.writeDataTransaction(y1 >> 8);
  IO.writeDataTransaction(y1);        // YEND
  IO.writeCommandTransaction(ILI9486_RAMWR);  // write to RAM
  IO.startTransaction();
}

void GxCTRL_ILI9486::setRotation(uint8_t r)
{
  // this controller seems to need separate transactions; reason speed on SPI ?
  IO.writeCommandTransaction(ILI9486_MADCTL);
  switch (r & 3) 
  {
    case 0:
      IO.writeDataTransaction(MADCTL_MX | MADCTL_BGR);
      break;
    case 1:
      IO.writeDataTransaction(MADCTL_MV | MADCTL_BGR);
      break;
    case 2:
      IO.writeDataTransaction(MADCTL_MY | MADCTL_BGR);
      break;
    case 3:
      IO.writeDataTransaction(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      break;
  }
}

