// created by Jean-Marc Zingg to be the GxCTRL_ST7796S class for the GxTFT library
// code extracts taken from http://www.lcdwiki.com/res/Program/Parallel_Port/4.0inch/8_16BIT_ST7796S_MRB3951_V1.0/4.0inch_8&16BIT_Module_ST7796S_MRB3951_V1.0.zip (lcd.h, lcd.c)
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "4.0 inch HD TFT LCD Screen with PCB Board ILI9488 ST7796S Drive IC 320(RGB)*480 8Bit Parallel Interface (Touch/No Touch)" display from Maithoga
// e.g. https://www.aliexpress.com/item/1899444540.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// this display also matches the FSMC TFT connector of the STM32F407VxT6 board e.g. https://www.aliexpress.com/item/1005001806399987.html

#include "GxCTRL_ST7796S.h"

#define MADCTL     0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08

uint32_t GxCTRL_ST7796S::readID()
{
  return readRegister(0xD3, 2, 2);
}

uint32_t GxCTRL_ST7796S::readRegister(uint8_t nr, uint8_t index, uint8_t bytes)
{
  uint32_t rv = 0;
  bytes = gx_uint8_min(bytes, 4);
  IO.startTransaction();
  IO.writeCommand(nr);
  //IO.readData(); // dummy
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

uint16_t GxCTRL_ST7796S::readPixel(uint16_t x, uint16_t y)
{
  uint16_t rv;
  readRect(x, y, 1, 1, &rv);
  return rv;
}

#define NT35510_RAMRD_AUTO_INCREMENT_OK

#if defined(NT35510_RAMRD_AUTO_INCREMENT_OK)

void GxCTRL_ST7796S::readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  uint32_t num = uint32_t(w) * uint32_t(h);
  IO.startTransaction();
  setWindowAddress(x, y, xe, ye);
  IO.writeCommand(0x2E);  // read from RAM
  IO.readData16(); // dummy
  for (; num > 0; num--)
  {
    *data++ = IO.readData16();
  }
  IO.endTransaction();
}

#else

void GxCTRL_ST7796S::readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  for (uint16_t yy = y; yy <= ye; yy++)
  {
    for (uint16_t xx = x; xx <= xe; xx++)
    {
      IO.startTransaction();
      setWindowAddress(xx, yy, xx, yy);
      IO.writeCommand(0x2E);  // read from RAM
      IO.readData16(); // dummy
      *data++ = IO.readData16();
      IO.endTransaction();
    }
  }
}

#endif

void GxCTRL_ST7796S::init()
{
  delay(20);
  //************* ST7796S��ʼ��**********//
  IO.writeCommandTransaction(0xF0);
  IO.writeDataTransaction(0xC3);
  IO.writeCommandTransaction(0xF0);
  IO.writeDataTransaction(0x96);
  IO.writeCommandTransaction(0x36);
  IO.writeDataTransaction(0x68);
  IO.writeCommandTransaction(0x3A);
  IO.writeDataTransaction(0x05);
  IO.writeCommandTransaction(0xB0);
  IO.writeDataTransaction(0x80);
  IO.writeCommandTransaction(0xB6);
  IO.writeDataTransaction(0x20);
  IO.writeDataTransaction(0x02);
  IO.writeCommandTransaction(0xB5);
  IO.writeDataTransaction(0x02);
  IO.writeDataTransaction(0x03);
  IO.writeDataTransaction(0x00);
  IO.writeDataTransaction(0x04);
  IO.writeCommandTransaction(0xB1);
  IO.writeDataTransaction(0x80);
  IO.writeDataTransaction(0x10);
  IO.writeCommandTransaction(0xB4);
  IO.writeDataTransaction(0x00);
  IO.writeCommandTransaction(0xB7);
  IO.writeDataTransaction(0xC6);
  IO.writeCommandTransaction(0xC5);
  IO.writeDataTransaction(0x24);
  IO.writeCommandTransaction(0xE4);
  IO.writeDataTransaction(0x31);
  IO.writeCommandTransaction(0xE8);
  IO.writeDataTransaction(0x40);
  IO.writeDataTransaction(0x8A);
  IO.writeDataTransaction(0x00);
  IO.writeDataTransaction(0x00);
  IO.writeDataTransaction(0x29);
  IO.writeDataTransaction(0x19);
  IO.writeDataTransaction(0xA5);
  IO.writeDataTransaction(0x33);
  IO.writeCommandTransaction(0xC2);
  IO.writeCommandTransaction(0xA7);

  IO.writeCommandTransaction(0xE0);
  IO.writeDataTransaction(0xF0);
  IO.writeDataTransaction(0x09);
  IO.writeDataTransaction(0x13);
  IO.writeDataTransaction(0x12);
  IO.writeDataTransaction(0x12);
  IO.writeDataTransaction(0x2B);
  IO.writeDataTransaction(0x3C);
  IO.writeDataTransaction(0x44);
  IO.writeDataTransaction(0x4B);
  IO.writeDataTransaction(0x1B);
  IO.writeDataTransaction(0x18);
  IO.writeDataTransaction(0x17);
  IO.writeDataTransaction(0x1D);
  IO.writeDataTransaction(0x21);

  IO.writeCommandTransaction(0XE1);
  IO.writeDataTransaction(0xF0);
  IO.writeDataTransaction(0x09);
  IO.writeDataTransaction(0x13);
  IO.writeDataTransaction(0x0C);
  IO.writeDataTransaction(0x0D);
  IO.writeDataTransaction(0x27);
  IO.writeDataTransaction(0x3B);
  IO.writeDataTransaction(0x44);
  IO.writeDataTransaction(0x4D);
  IO.writeDataTransaction(0x0B);
  IO.writeDataTransaction(0x17);
  IO.writeDataTransaction(0x17);
  IO.writeDataTransaction(0x1D);
  IO.writeDataTransaction(0x21);

  IO.writeCommandTransaction(0X36);
  IO.writeDataTransaction(0xEC);
  IO.writeCommandTransaction(0xF0);
  IO.writeDataTransaction(0xC3);
  IO.writeCommandTransaction(0xF0);
  IO.writeDataTransaction(0x69);
  IO.writeCommandTransaction(0X13);
  IO.writeCommandTransaction(0X11);
  IO.writeCommandTransaction(0X29);

  //LCD_direction(USE_HORIZONTAL);//����LCD��ʾ����
  //LCD_LED = 1; //��������
  //LCD_Clear(WHITE);//��ȫ����ɫ
  //setWindowAddress(0, 0, 0x01e0, 0x0320);
}

void GxCTRL_ST7796S::setWindowAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  IO.writeCommand(0x2A);
  IO.writeData(x0 >> 8);
  IO.writeData(x0 & 0x00ff);
  IO.writeData(x1 >> 8);
  IO.writeData(x1 & 0x00ff);
  IO.writeCommand(0x2B);
  IO.writeData(y0 >> 8);
  IO.writeData(y0 & 0x00ff);
  IO.writeData(y1 >> 8);
  IO.writeData(y1 & 0x00ff);
  IO.writeCommand(0x2C);
}

void GxCTRL_ST7796S::setRotation(uint8_t r)
{
  IO.startTransaction();
  IO.writeCommand16(MADCTL);
  switch (r & 3)
  {
    case 0:
      //IO.writeData((1<<3)|(1<<6)); // 0b0000'0000'0100'1000 0x0048
      IO.writeData(MADCTL_MX | MADCTL_BGR);
      break;
    case 1:
      //IO.writeData((1<<3)|(1<<5)); // 0b0000'0000'0010'1000 0x0028
      IO.writeData(MADCTL_MV | MADCTL_BGR);
      break;
    case 2:
      //IO.writeData((1<<3)|(1<<7)|(1<<4)); // 0b0000'0000'1001'1000 0x0094
      IO.writeData(MADCTL_MY | MADCTL_ML | MADCTL_BGR);
      break;
    case 3:
      //IO.writeData((1<<3)|(1<<7)|(1<<6)|(1<<5)|(1<<4)); // 0b0000'0000'1111'1000 0x00F4
      IO.writeData(MADCTL_MY | MADCTL_MX | MADCTL_MV | MADCTL_ML | MADCTL_BGR);
      break;
  }
  IO.endTransaction();
}
