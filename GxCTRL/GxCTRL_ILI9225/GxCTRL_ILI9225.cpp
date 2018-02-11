// created by Jean-Marc Zingg to be the GxCTRL_ILI9225 class for the GxTFT library
// code extracts taken from http://www.rinkydinkelectronics.com/download.php?f=UTFT.zip
// code extracts taken from https://github.com/adafruit/TFTLCD-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#include "GxCTRL_ILI9225.h"

uint32_t GxCTRL_ILI9225::readID()
{
  return readRegister(0x0, 0, 2);
}

uint32_t GxCTRL_ILI9225::readRegister(uint8_t nr, uint8_t index, uint8_t bytes)
{
  uint32_t rv = 0;
  bytes = min(bytes, 4);
  IO.startTransaction();
  IO.writeCommand(nr);
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

uint16_t GxCTRL_ILI9225::readPixel(uint16_t x, uint16_t y)
{
  uint16_t rv;
  readRect(x, y, 1, 1, &rv);
  return rv;
}

void GxCTRL_ILI9225::readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  uint32_t num = uint32_t(w) * uint32_t(h);
  for (uint16_t yy = y; yy <= ye; yy++)
  {
    for (uint16_t xx = x; xx <= xe; xx++)
    {
      IO.startTransaction();
      setWindowAddress(xx, yy, xe, ye);
      IO.writeCommand(0x22);
      IO.readData16(); // dummy
      *data++ = IO.readData16();
      IO.endTransaction();
    }
  }
}

void GxCTRL_ILI9225::init()
{
  _rotation = 0; // portrait is default
  IO.startTransaction();
  LCD_WriteReg(0x00D0, 0x0003);
  LCD_WriteReg(0x00EB, 0x0B00);
  LCD_WriteReg(0x00EC, 0x000F);
  LCD_WriteReg(0x00C7, 0x030F);

  LCD_WriteReg(0x0001, 0x011C);
  LCD_WriteReg(0x0002, 0x0100);
  LCD_WriteReg(0x0003, 0x1030);
  LCD_WriteReg(0x0008, 0x0808);
  LCD_WriteReg(0x000F, 0x0901);
  delay(10);
  LCD_WriteReg(0x0010, 0x0000);
  LCD_WriteReg(0x0011, 0x1B41); //The register setting is suitable for VCI=2.8V
  delay(120);
  LCD_WriteReg(0x0012, 0x200E);//The register setting is suitable for VCI=2.8V
  LCD_WriteReg(0x0013, 0x0052); //The register setting is suitable for VCI=2.8V
  LCD_WriteReg(0x0014, 0x4B5C); //The register setting is suitable for VCI=2.8V

  LCD_WriteReg(0x0030, 0x0000);
  LCD_WriteReg(0x0031, 0x00DB);
  LCD_WriteReg(0x0032, 0x0000);
  LCD_WriteReg(0x0033, 0x0000);
  LCD_WriteReg(0x0034, 0x00DB);
  LCD_WriteReg(0x0035, 0x0000);
  LCD_WriteReg(0x0036, 0x00AF);
  LCD_WriteReg(0x0037, 0x0000);
  LCD_WriteReg(0x0038, 0x00DB);
  LCD_WriteReg(0x0039, 0x0000);
  LCD_WriteReg(0x0050, 0x0000);
  LCD_WriteReg(0x0051, 0x0705);
  LCD_WriteReg(0x0052, 0x0C0A);
  LCD_WriteReg(0x0053, 0x0401);
  LCD_WriteReg(0x0054, 0x040C);
  LCD_WriteReg(0x0055, 0x0608);
  LCD_WriteReg(0x0056, 0x0000);
  LCD_WriteReg(0x0057, 0x0104);
  LCD_WriteReg(0x0058, 0x0E06);
  LCD_WriteReg(0x0059, 0x060E);;

  LCD_WriteReg(0x0020, 0x0000);
  LCD_WriteReg(0x0021, 0x0000);
  LCD_WriteReg(0x0007, 0x1017);
  IO.endTransaction();
}

void GxCTRL_ILI9225::setWindowAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  switch (_rotation)
  {
    case 0:
      LCD_Write_COM_DATA(0x50, x0); // horizontal address start
      LCD_Write_COM_DATA(0x51, x1); // horizontal address end
      LCD_Write_COM_DATA(0x52, y0); // vertical address start
      LCD_Write_COM_DATA(0x53, y1); // vertical address end
      LCD_Write_COM_DATA(0x20, x0); // horizontal GRAM address set
      LCD_Write_COM_DATA(0x21, y0); // vertical GRAM address set
      break;
    case 1:
      LCD_Write_COM_DATA(0x50, _tft_width - 1 - y1); // horizontal address start
      LCD_Write_COM_DATA(0x51, _tft_width - 1 - y0); // horizontal address end
      LCD_Write_COM_DATA(0x52, x0); // vertical address start
      LCD_Write_COM_DATA(0x53, x1); // vertical address end
      LCD_Write_COM_DATA(0x20, _tft_width - 1 - y0); // horizontal GRAM address set
      LCD_Write_COM_DATA(0x21, x0); // vertical GRAM address set
      break;
    case 2:
      LCD_Write_COM_DATA(0x50, _tft_width - 1 - x1); // horizontal address start
      LCD_Write_COM_DATA(0x51, _tft_width - 1 - x0); // horizontal address end
      LCD_Write_COM_DATA(0x52, _tft_height - 1 - y1); // vertical address start
      LCD_Write_COM_DATA(0x53, _tft_height - 1 - y0); // vertical address end
      LCD_Write_COM_DATA(0x20, _tft_width - 1 - x0); // horizontal GRAM address set
      LCD_Write_COM_DATA(0x21, _tft_height - 1 - y0); // vertical GRAM address set
      break;
    case 3:
      LCD_Write_COM_DATA(0x50, y0); // horizontal address start
      LCD_Write_COM_DATA(0x51, y1); // horizontal address end
      LCD_Write_COM_DATA(0x52, _tft_height - 1 - x1); // vertical address start
      LCD_Write_COM_DATA(0x53, _tft_height - 1 - x0); // vertical address end
      LCD_Write_COM_DATA(0x20, y0); // horizontal GRAM address set
      LCD_Write_COM_DATA(0x21, _tft_height - 1 - x0); // vertical GRAM address set
      break;
  }
  IO.writeCommand(0x22);
}

void GxCTRL_ILI9225::setRotation(uint8_t r)
{
  _rotation = r & 3;
  IO.startTransaction();
  switch (_rotation)
  {
    case 0: // Portrait
      LCD_Write_COM_DATA(0x03, 0x1030);
      break;
    case 1: // Landscape (Portrait + 90)
      LCD_Write_COM_DATA(0x03, 0x1028);
      break;
    case 2: // Inverter portrait
      LCD_Write_COM_DATA(0x03, 0x1080);
      break;
    case 3: // Inverted landscape
      LCD_Write_COM_DATA(0x03, 0x1018);
      break;
  }
  IO.endTransaction();
}

void GxCTRL_ILI9225::LCD_Write_COM_DATA(uint8_t com, uint16_t data)
{
  IO.writeCommand(com);
  IO.writeData(data >> 8);
  IO.writeData(data);
}

void GxCTRL_ILI9225::LCD_WriteReg(uint16_t com, uint16_t data)
{
  IO.writeCommand(com);
  IO.writeData(data >> 8);
  IO.writeData(data);
}

