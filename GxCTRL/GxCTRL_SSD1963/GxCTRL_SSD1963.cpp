// created by Jean-Marc Zingg to be the GxCTRL_SSD1963 class for the GxTFT library
// code extracts taken from http://www.rinkydinkelectronics.com/download.php?f=UTFT.zip
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#include "GxCTRL_SSD1963.h"

void GxCTRL_SSD1963::init()
{
  rotation = 1; // landscape is default
  IO.startTransaction();
  IO.writeCommand(0xE2);    //PLL multiplier, set PLL clock to 120M
  IO.writeData(0x23);     //N=0x36 for 6.5M, 0x23 for 10M crystal
  IO.writeData(0x02);
  IO.writeData(0x04); // original SSD1963_800ALT
  IO.writeCommand(0xE0);    // PLL enable
  IO.writeData(0x01);
  delay(10);
  IO.writeCommand(0xE0);
  IO.writeData(0x03);
  delay(10);
  IO.writeCommand(0x01);    // software reset
  delay(100);
  IO.writeCommand(0xE6);    //PLL setting for PCLK, depends on resolution
  IO.writeData(0x04);
  IO.writeData(0x93);
  IO.writeData(0xE0);

  IO.writeCommand(0xB0);    //LCD SPECIFICATION
  IO.writeData(0x00); // 0x24
  IO.writeData(0x00);
  IO.writeAddrMSBfirst(physical_width - 1); // Set HDP
  //IO.writeData(0x03);   //Set HDP 799
  //IO.writeData(0x1F);
  IO.writeAddrMSBfirst(physical_height - 1); // Set VDP
  //IO.writeData(0x01);   //Set VDP 479
  //IO.writeData(0xDF);
  IO.writeData(0x00);

  IO.writeCommand(0xB4);    //HSYNC
  IO.writeData(0x03);   //Set HT  928
  IO.writeData(0xA0);
  IO.writeData(0x00);   //Set HPS 46
  IO.writeData(0x2E);
  IO.writeData(0x30);   //Set HPW 48
  IO.writeData(0x00);   //Set LPS 15
  IO.writeData(0x0F);
  IO.writeData(0x00);

  IO.writeCommand(0xB6);    //VSYNC
  IO.writeData(0x02);   //Set VT  525
  IO.writeData(0x0D);
  IO.writeData(0x00);   //Set VPS 16
  IO.writeData(0x10);
  IO.writeData(0x10);   //Set VPW 16
  IO.writeData(0x00);   //Set FPS 8
  IO.writeData(0x08);

  IO.writeCommand(0xBA);
  //IO.writeData(0x05);   //GPIO[3:0] out 1
  IO.writeData(0x07);   //GPIO[3:0] out 1

  IO.writeCommand(0xB8);
  IO.writeData(0x07);     //GPIO3=input, GPIO[2:0]=output
  IO.writeData(0x01);   //GPIO0 normal

  IO.writeCommand(0x36);    //rotation
  IO.writeData(0x22);   // -- Set to 0x21 to rotate 180 degrees

  IO.writeCommand(0xF0);    //pixel data interface
  IO.writeData(0x03);
  IO.endTransaction();

  delay(10);

  IO.startTransaction();
  // SetWindow, physical addresses, even if default rotation is changed
  IO.writeCommand(0x2a);
  IO.writeAddrMSBfirst(0);
  IO.writeAddrMSBfirst(physical_width - 1);
  IO.writeCommand(0x2b);
  IO.writeAddrMSBfirst(0);
  IO.writeAddrMSBfirst(physical_height - 1);

  IO.writeCommand(0x29);    //display on

  IO.writeCommand(0xBE);    //set PWM for B/L
  IO.writeData(0x06);
  IO.writeData(0xF0);
  IO.writeData(0x01);
  IO.writeData(0xF0);
  IO.writeData(0x00);
  IO.writeData(0x00);

  IO.writeCommand(0xD0);
  IO.writeData(0x0D);

  IO.writeCommand(0x2C);
  IO.endTransaction();
}

void GxCTRL_SSD1963::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  if (rotation & 1) // landscape
  {
    IO.startTransaction();
    IO.writeCommand(0x2a);
    IO.writeAddrMSBfirst(x0);
    IO.writeAddrMSBfirst(x1);
    IO.writeCommand(0x2b);
    IO.writeAddrMSBfirst(y0);
    IO.writeAddrMSBfirst(y1);
    IO.writeCommand(0x2c);
  }
  else // portrait
  {
    // transform to physical addresses
    IO.startTransaction();
    IO.writeCommand(0x2b); // by switching address
    IO.writeAddrMSBfirst(x0);
    IO.writeAddrMSBfirst(x1);
    IO.writeCommand(0x2a); // by switching address
    IO.writeAddrMSBfirst(y0);
    IO.writeAddrMSBfirst(y1);
    IO.writeCommand(0x2c);
  }
  IO.writeCommand(0x2c);
  IO.endTransaction();
}

void GxCTRL_SSD1963::setWindowKeepTransaction(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  if (rotation & 1) // landscape
  {
    IO.startTransaction();
    IO.writeCommand(0x2a);
    IO.writeAddrMSBfirst(x0);
    IO.writeAddrMSBfirst(x1);
    IO.writeCommand(0x2b);
    IO.writeAddrMSBfirst(y0);
    IO.writeAddrMSBfirst(y1);
    IO.writeCommand(0x2c);
  }
  else // portrait
  {
    // transform to physical addresses
    IO.startTransaction();
    IO.writeCommand(0x2b); // by switching address
    IO.writeAddrMSBfirst(x0);
    IO.writeAddrMSBfirst(x1);
    IO.writeCommand(0x2a); // by switching address
    IO.writeAddrMSBfirst(y0);
    IO.writeAddrMSBfirst(y1);
    IO.writeCommand(0x2c);
  }
}

void GxCTRL_SSD1963::setRotation(uint8_t r)
{
  rotation = r;
  IO.startTransaction();
  IO.writeCommand(0x36);
  switch (r & 3)
  {
    case 0:
      IO.writeData(0x33);
      break;
    case 1:
      IO.writeData(0x2);
      break;
    case 2:
      IO.writeData(0x20);
      break;
    case 3:
      IO.writeData(0x11);
      break;
  }
  IO.endTransaction();
}

