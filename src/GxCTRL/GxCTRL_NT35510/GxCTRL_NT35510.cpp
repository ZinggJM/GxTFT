// created by Jean-Marc Zingg to be the GxCTRL_NT35510 class for the GxTFT library
// code extracts taken from http://www.lcdwiki.com/res/Program/Parallel_Port/3.97inch/8_16BIT_NT35510_800x480_MRB3973_V1.2/3.97inch_8&16BIT_Module_NT35510_800x480_MRB3973_V1.2.zip (lcd.h, lcd.c)
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" display from Ruijia Industry
// e.g. https://www.aliexpress.com/item/32676929794.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// this display also matches the FSMC TFT connector of the STM32F407VxT6 board e.g. https://www.aliexpress.com/item/1005001806399987.html
//
// note: this display class uses 16bit commands, for 16 bit solder jumper settings

#include "GxCTRL_NT35510.h"

#define MADCTL     0x3600
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08

uint32_t GxCTRL_NT35510::readID()
{
  return readRegister(0x04, 0, 3);
}

uint32_t GxCTRL_NT35510::readRegister(uint8_t nr, uint8_t index, uint8_t bytes)
{
  uint32_t rv = 0;
  bytes = gx_uint8_min(bytes, 4);
  IO.startTransaction();
  IO.writeCommand16(nr << 8);
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

uint16_t GxCTRL_NT35510::readPixel(uint16_t x, uint16_t y)
{
  uint16_t rv;
  readRect(x, y, 1, 1, &rv);
  return rv;
}

#define NT35510_RAMRD_AUTO_INCREMENT_OK

#if defined(NT35510_RAMRD_AUTO_INCREMENT_OK)

void GxCTRL_NT35510::readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  uint32_t num = uint32_t(w) * uint32_t(h);
  IO.startTransaction();
  setWindowAddress(x, y, xe, ye);
  IO.writeCommand16(0x2E00);  // read from RAM
  IO.readData16(); // dummy
  for (; num > 0; num--)
  {
      uint16_t rg = IO.readData16();
      uint16_t r = rg >> 8;
      uint16_t g = rg & 0xFF;
      uint16_t b = IO.readData16() >> 8;
      *data++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
  }
  IO.endTransaction();
}

#else

void GxCTRL_NT35510::readRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  for (uint16_t yy = y; yy <= ye; yy++)
  {
    for (uint16_t xx = x; xx <= xe; xx++)
    {
      IO.startTransaction();
      setWindowAddress(xx, yy, xx, yy);
      IO.writeCommand16(0x2E00);  // read from RAM
      IO.readData16(); // dummy
      uint16_t rg = IO.readData16();
      uint16_t r = rg >> 8;
      uint16_t g = rg & 0xFF;
      uint16_t b = IO.readData16() >> 8;
      *data++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
      IO.endTransaction();
    }
  }
}

#endif

void GxCTRL_NT35510::init()
{
  delay(20);
  //************* NT35510��ʼ��**********//
  IO.writeCommand16Transaction(0xF000); IO.writeData16Transaction(0x55);
  IO.writeCommand16Transaction(0xF001); IO.writeData16Transaction(0xAA);
  IO.writeCommand16Transaction(0xF002); IO.writeData16Transaction(0x52);
  IO.writeCommand16Transaction(0xF003); IO.writeData16Transaction(0x08);
  IO.writeCommand16Transaction(0xF004); IO.writeData16Transaction(0x01);
  //# AVDD: manual); IO.writeData16Transaction(
  IO.writeCommand16Transaction(0xB600); IO.writeData16Transaction(0x34);
  IO.writeCommand16Transaction(0xB601); IO.writeData16Transaction(0x34);
  IO.writeCommand16Transaction(0xB602); IO.writeData16Transaction(0x34);

  IO.writeCommand16Transaction(0xB000); IO.writeData16Transaction(0x0D);//09
  IO.writeCommand16Transaction(0xB001); IO.writeData16Transaction(0x0D);
  IO.writeCommand16Transaction(0xB002); IO.writeData16Transaction(0x0D);
  //# AVEE: manual); IO.writeData16Transaction( -6V
  IO.writeCommand16Transaction(0xB700); IO.writeData16Transaction(0x24);
  IO.writeCommand16Transaction(0xB701); IO.writeData16Transaction(0x24);
  IO.writeCommand16Transaction(0xB702); IO.writeData16Transaction(0x24);

  IO.writeCommand16Transaction(0xB100); IO.writeData16Transaction(0x0D);
  IO.writeCommand16Transaction(0xB101); IO.writeData16Transaction(0x0D);
  IO.writeCommand16Transaction(0xB102); IO.writeData16Transaction(0x0D);
  //#Power Control for
  //VCL
  IO.writeCommand16Transaction(0xB800); IO.writeData16Transaction(0x24);
  IO.writeCommand16Transaction(0xB801); IO.writeData16Transaction(0x24);
  IO.writeCommand16Transaction(0xB802); IO.writeData16Transaction(0x24);

  IO.writeCommand16Transaction(0xB200); IO.writeData16Transaction(0x00);

  //# VGH: Clamp Enable); IO.writeData16Transaction(
  IO.writeCommand16Transaction(0xB900); IO.writeData16Transaction(0x24);
  IO.writeCommand16Transaction(0xB901); IO.writeData16Transaction(0x24);
  IO.writeCommand16Transaction(0xB902); IO.writeData16Transaction(0x24);

  IO.writeCommand16Transaction(0xB300); IO.writeData16Transaction(0x05);
  IO.writeCommand16Transaction(0xB301); IO.writeData16Transaction(0x05);
  IO.writeCommand16Transaction(0xB302); IO.writeData16Transaction(0x05);

  ///IO.writeCommand16Transaction(0xBF00); IO.writeData16Transaction(0x01);

  //# VGL(LVGL):
  IO.writeCommand16Transaction(0xBA00); IO.writeData16Transaction(0x34);
  IO.writeCommand16Transaction(0xBA01); IO.writeData16Transaction(0x34);
  IO.writeCommand16Transaction(0xBA02); IO.writeData16Transaction(0x34);
  //# VGL_REG(VGLO)
  IO.writeCommand16Transaction(0xB500); IO.writeData16Transaction(0x0B);
  IO.writeCommand16Transaction(0xB501); IO.writeData16Transaction(0x0B);
  IO.writeCommand16Transaction(0xB502); IO.writeData16Transaction(0x0B);
  //# VGMP/VGSP:
  IO.writeCommand16Transaction(0xBC00); IO.writeData16Transaction(0X00);
  IO.writeCommand16Transaction(0xBC01); IO.writeData16Transaction(0xA3);
  IO.writeCommand16Transaction(0xBC02); IO.writeData16Transaction(0X00);
  //# VGMN/VGSN
  IO.writeCommand16Transaction(0xBD00); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xBD01); IO.writeData16Transaction(0xA3);
  IO.writeCommand16Transaction(0xBD02); IO.writeData16Transaction(0x00);
  //# VCOM=-0.1
  IO.writeCommand16Transaction(0xBE00); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xBE01); IO.writeData16Transaction(0x63);//4f
  //  VCOMH+0x01;
  //#R+
  IO.writeCommand16Transaction(0xD100); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD101); IO.writeData16Transaction(0x37);
  IO.writeCommand16Transaction(0xD102); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD103); IO.writeData16Transaction(0x52);
  IO.writeCommand16Transaction(0xD104); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD105); IO.writeData16Transaction(0x7B);
  IO.writeCommand16Transaction(0xD106); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD107); IO.writeData16Transaction(0x99);
  IO.writeCommand16Transaction(0xD108); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD109); IO.writeData16Transaction(0xB1);
  IO.writeCommand16Transaction(0xD10A); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD10B); IO.writeData16Transaction(0xD2);
  IO.writeCommand16Transaction(0xD10C); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD10D); IO.writeData16Transaction(0xF6);
  IO.writeCommand16Transaction(0xD10E); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD10F); IO.writeData16Transaction(0x27);
  IO.writeCommand16Transaction(0xD110); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD111); IO.writeData16Transaction(0x4E);
  IO.writeCommand16Transaction(0xD112); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD113); IO.writeData16Transaction(0x8C);
  IO.writeCommand16Transaction(0xD114); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD115); IO.writeData16Transaction(0xBE);
  IO.writeCommand16Transaction(0xD116); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD117); IO.writeData16Transaction(0x0B);
  IO.writeCommand16Transaction(0xD118); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD119); IO.writeData16Transaction(0x48);
  IO.writeCommand16Transaction(0xD11A); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD11B); IO.writeData16Transaction(0x4A);
  IO.writeCommand16Transaction(0xD11C); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD11D); IO.writeData16Transaction(0x7E);
  IO.writeCommand16Transaction(0xD11E); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD11F); IO.writeData16Transaction(0xBC);
  IO.writeCommand16Transaction(0xD120); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD121); IO.writeData16Transaction(0xE1);
  IO.writeCommand16Transaction(0xD122); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD123); IO.writeData16Transaction(0x10);
  IO.writeCommand16Transaction(0xD124); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD125); IO.writeData16Transaction(0x31);
  IO.writeCommand16Transaction(0xD126); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD127); IO.writeData16Transaction(0x5A);
  IO.writeCommand16Transaction(0xD128); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD129); IO.writeData16Transaction(0x73);
  IO.writeCommand16Transaction(0xD12A); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD12B); IO.writeData16Transaction(0x94);
  IO.writeCommand16Transaction(0xD12C); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD12D); IO.writeData16Transaction(0x9F);
  IO.writeCommand16Transaction(0xD12E); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD12F); IO.writeData16Transaction(0xB3);
  IO.writeCommand16Transaction(0xD130); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD131); IO.writeData16Transaction(0xB9);
  IO.writeCommand16Transaction(0xD132); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD133); IO.writeData16Transaction(0xC1);
  //#G+
  IO.writeCommand16Transaction(0xD200); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD201); IO.writeData16Transaction(0x37);
  IO.writeCommand16Transaction(0xD202); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD203); IO.writeData16Transaction(0x52);
  IO.writeCommand16Transaction(0xD204); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD205); IO.writeData16Transaction(0x7B);
  IO.writeCommand16Transaction(0xD206); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD207); IO.writeData16Transaction(0x99);
  IO.writeCommand16Transaction(0xD208); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD209); IO.writeData16Transaction(0xB1);
  IO.writeCommand16Transaction(0xD20A); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD20B); IO.writeData16Transaction(0xD2);
  IO.writeCommand16Transaction(0xD20C); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD20D); IO.writeData16Transaction(0xF6);
  IO.writeCommand16Transaction(0xD20E); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD20F); IO.writeData16Transaction(0x27);
  IO.writeCommand16Transaction(0xD210); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD211); IO.writeData16Transaction(0x4E);
  IO.writeCommand16Transaction(0xD212); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD213); IO.writeData16Transaction(0x8C);
  IO.writeCommand16Transaction(0xD214); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD215); IO.writeData16Transaction(0xBE);
  IO.writeCommand16Transaction(0xD216); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD217); IO.writeData16Transaction(0x0B);
  IO.writeCommand16Transaction(0xD218); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD219); IO.writeData16Transaction(0x48);
  IO.writeCommand16Transaction(0xD21A); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD21B); IO.writeData16Transaction(0x4A);
  IO.writeCommand16Transaction(0xD21C); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD21D); IO.writeData16Transaction(0x7E);
  IO.writeCommand16Transaction(0xD21E); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD21F); IO.writeData16Transaction(0xBC);
  IO.writeCommand16Transaction(0xD220); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD221); IO.writeData16Transaction(0xE1);
  IO.writeCommand16Transaction(0xD222); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD223); IO.writeData16Transaction(0x10);
  IO.writeCommand16Transaction(0xD224); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD225); IO.writeData16Transaction(0x31);
  IO.writeCommand16Transaction(0xD226); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD227); IO.writeData16Transaction(0x5A);
  IO.writeCommand16Transaction(0xD228); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD229); IO.writeData16Transaction(0x73);
  IO.writeCommand16Transaction(0xD22A); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD22B); IO.writeData16Transaction(0x94);
  IO.writeCommand16Transaction(0xD22C); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD22D); IO.writeData16Transaction(0x9F);
  IO.writeCommand16Transaction(0xD22E); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD22F); IO.writeData16Transaction(0xB3);
  IO.writeCommand16Transaction(0xD230); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD231); IO.writeData16Transaction(0xB9);
  IO.writeCommand16Transaction(0xD232); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD233); IO.writeData16Transaction(0xC1);
  //#B+
  IO.writeCommand16Transaction(0xD300); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD301); IO.writeData16Transaction(0x37);
  IO.writeCommand16Transaction(0xD302); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD303); IO.writeData16Transaction(0x52);
  IO.writeCommand16Transaction(0xD304); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD305); IO.writeData16Transaction(0x7B);
  IO.writeCommand16Transaction(0xD306); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD307); IO.writeData16Transaction(0x99);
  IO.writeCommand16Transaction(0xD308); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD309); IO.writeData16Transaction(0xB1);
  IO.writeCommand16Transaction(0xD30A); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD30B); IO.writeData16Transaction(0xD2);
  IO.writeCommand16Transaction(0xD30C); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD30D); IO.writeData16Transaction(0xF6);
  IO.writeCommand16Transaction(0xD30E); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD30F); IO.writeData16Transaction(0x27);
  IO.writeCommand16Transaction(0xD310); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD311); IO.writeData16Transaction(0x4E);
  IO.writeCommand16Transaction(0xD312); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD313); IO.writeData16Transaction(0x8C);
  IO.writeCommand16Transaction(0xD314); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD315); IO.writeData16Transaction(0xBE);
  IO.writeCommand16Transaction(0xD316); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD317); IO.writeData16Transaction(0x0B);
  IO.writeCommand16Transaction(0xD318); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD319); IO.writeData16Transaction(0x48);
  IO.writeCommand16Transaction(0xD31A); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD31B); IO.writeData16Transaction(0x4A);
  IO.writeCommand16Transaction(0xD31C); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD31D); IO.writeData16Transaction(0x7E);
  IO.writeCommand16Transaction(0xD31E); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD31F); IO.writeData16Transaction(0xBC);
  IO.writeCommand16Transaction(0xD320); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD321); IO.writeData16Transaction(0xE1);
  IO.writeCommand16Transaction(0xD322); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD323); IO.writeData16Transaction(0x10);
  IO.writeCommand16Transaction(0xD324); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD325); IO.writeData16Transaction(0x31);
  IO.writeCommand16Transaction(0xD326); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD327); IO.writeData16Transaction(0x5A);
  IO.writeCommand16Transaction(0xD328); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD329); IO.writeData16Transaction(0x73);
  IO.writeCommand16Transaction(0xD32A); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD32B); IO.writeData16Transaction(0x94);
  IO.writeCommand16Transaction(0xD32C); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD32D); IO.writeData16Transaction(0x9F);
  IO.writeCommand16Transaction(0xD32E); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD32F); IO.writeData16Transaction(0xB3);
  IO.writeCommand16Transaction(0xD330); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD331); IO.writeData16Transaction(0xB9);
  IO.writeCommand16Transaction(0xD332); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD333); IO.writeData16Transaction(0xC1);

  //#R-///////////////////////////////////////////
  IO.writeCommand16Transaction(0xD400); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD401); IO.writeData16Transaction(0x37);
  IO.writeCommand16Transaction(0xD402); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD403); IO.writeData16Transaction(0x52);
  IO.writeCommand16Transaction(0xD404); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD405); IO.writeData16Transaction(0x7B);
  IO.writeCommand16Transaction(0xD406); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD407); IO.writeData16Transaction(0x99);
  IO.writeCommand16Transaction(0xD408); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD409); IO.writeData16Transaction(0xB1);
  IO.writeCommand16Transaction(0xD40A); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD40B); IO.writeData16Transaction(0xD2);
  IO.writeCommand16Transaction(0xD40C); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD40D); IO.writeData16Transaction(0xF6);
  IO.writeCommand16Transaction(0xD40E); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD40F); IO.writeData16Transaction(0x27);
  IO.writeCommand16Transaction(0xD410); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD411); IO.writeData16Transaction(0x4E);
  IO.writeCommand16Transaction(0xD412); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD413); IO.writeData16Transaction(0x8C);
  IO.writeCommand16Transaction(0xD414); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD415); IO.writeData16Transaction(0xBE);
  IO.writeCommand16Transaction(0xD416); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD417); IO.writeData16Transaction(0x0B);
  IO.writeCommand16Transaction(0xD418); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD419); IO.writeData16Transaction(0x48);
  IO.writeCommand16Transaction(0xD41A); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD41B); IO.writeData16Transaction(0x4A);
  IO.writeCommand16Transaction(0xD41C); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD41D); IO.writeData16Transaction(0x7E);
  IO.writeCommand16Transaction(0xD41E); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD41F); IO.writeData16Transaction(0xBC);
  IO.writeCommand16Transaction(0xD420); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD421); IO.writeData16Transaction(0xE1);
  IO.writeCommand16Transaction(0xD422); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD423); IO.writeData16Transaction(0x10);
  IO.writeCommand16Transaction(0xD424); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD425); IO.writeData16Transaction(0x31);
  IO.writeCommand16Transaction(0xD426); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD427); IO.writeData16Transaction(0x5A);
  IO.writeCommand16Transaction(0xD428); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD429); IO.writeData16Transaction(0x73);
  IO.writeCommand16Transaction(0xD42A); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD42B); IO.writeData16Transaction(0x94);
  IO.writeCommand16Transaction(0xD42C); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD42D); IO.writeData16Transaction(0x9F);
  IO.writeCommand16Transaction(0xD42E); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD42F); IO.writeData16Transaction(0xB3);
  IO.writeCommand16Transaction(0xD430); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD431); IO.writeData16Transaction(0xB9);
  IO.writeCommand16Transaction(0xD432); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD433); IO.writeData16Transaction(0xC1);

  //#G-//////////////////////////////////////////////
  IO.writeCommand16Transaction(0xD500); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD501); IO.writeData16Transaction(0x37);
  IO.writeCommand16Transaction(0xD502); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD503); IO.writeData16Transaction(0x52);
  IO.writeCommand16Transaction(0xD504); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD505); IO.writeData16Transaction(0x7B);
  IO.writeCommand16Transaction(0xD506); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD507); IO.writeData16Transaction(0x99);
  IO.writeCommand16Transaction(0xD508); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD509); IO.writeData16Transaction(0xB1);
  IO.writeCommand16Transaction(0xD50A); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD50B); IO.writeData16Transaction(0xD2);
  IO.writeCommand16Transaction(0xD50C); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD50D); IO.writeData16Transaction(0xF6);
  IO.writeCommand16Transaction(0xD50E); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD50F); IO.writeData16Transaction(0x27);
  IO.writeCommand16Transaction(0xD510); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD511); IO.writeData16Transaction(0x4E);
  IO.writeCommand16Transaction(0xD512); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD513); IO.writeData16Transaction(0x8C);
  IO.writeCommand16Transaction(0xD514); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD515); IO.writeData16Transaction(0xBE);
  IO.writeCommand16Transaction(0xD516); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD517); IO.writeData16Transaction(0x0B);
  IO.writeCommand16Transaction(0xD518); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD519); IO.writeData16Transaction(0x48);
  IO.writeCommand16Transaction(0xD51A); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD51B); IO.writeData16Transaction(0x4A);
  IO.writeCommand16Transaction(0xD51C); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD51D); IO.writeData16Transaction(0x7E);
  IO.writeCommand16Transaction(0xD51E); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD51F); IO.writeData16Transaction(0xBC);
  IO.writeCommand16Transaction(0xD520); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD521); IO.writeData16Transaction(0xE1);
  IO.writeCommand16Transaction(0xD522); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD523); IO.writeData16Transaction(0x10);
  IO.writeCommand16Transaction(0xD524); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD525); IO.writeData16Transaction(0x31);
  IO.writeCommand16Transaction(0xD526); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD527); IO.writeData16Transaction(0x5A);
  IO.writeCommand16Transaction(0xD528); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD529); IO.writeData16Transaction(0x73);
  IO.writeCommand16Transaction(0xD52A); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD52B); IO.writeData16Transaction(0x94);
  IO.writeCommand16Transaction(0xD52C); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD52D); IO.writeData16Transaction(0x9F);
  IO.writeCommand16Transaction(0xD52E); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD52F); IO.writeData16Transaction(0xB3);
  IO.writeCommand16Transaction(0xD530); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD531); IO.writeData16Transaction(0xB9);
  IO.writeCommand16Transaction(0xD532); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD533); IO.writeData16Transaction(0xC1);
  //#B-///////////////////////////////
  IO.writeCommand16Transaction(0xD600); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD601); IO.writeData16Transaction(0x37);
  IO.writeCommand16Transaction(0xD602); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD603); IO.writeData16Transaction(0x52);
  IO.writeCommand16Transaction(0xD604); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD605); IO.writeData16Transaction(0x7B);
  IO.writeCommand16Transaction(0xD606); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD607); IO.writeData16Transaction(0x99);
  IO.writeCommand16Transaction(0xD608); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD609); IO.writeData16Transaction(0xB1);
  IO.writeCommand16Transaction(0xD60A); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD60B); IO.writeData16Transaction(0xD2);
  IO.writeCommand16Transaction(0xD60C); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xD60D); IO.writeData16Transaction(0xF6);
  IO.writeCommand16Transaction(0xD60E); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD60F); IO.writeData16Transaction(0x27);
  IO.writeCommand16Transaction(0xD610); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD611); IO.writeData16Transaction(0x4E);
  IO.writeCommand16Transaction(0xD612); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD613); IO.writeData16Transaction(0x8C);
  IO.writeCommand16Transaction(0xD614); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xD615); IO.writeData16Transaction(0xBE);
  IO.writeCommand16Transaction(0xD616); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD617); IO.writeData16Transaction(0x0B);
  IO.writeCommand16Transaction(0xD618); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD619); IO.writeData16Transaction(0x48);
  IO.writeCommand16Transaction(0xD61A); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD61B); IO.writeData16Transaction(0x4A);
  IO.writeCommand16Transaction(0xD61C); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD61D); IO.writeData16Transaction(0x7E);
  IO.writeCommand16Transaction(0xD61E); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD61F); IO.writeData16Transaction(0xBC);
  IO.writeCommand16Transaction(0xD620); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xD621); IO.writeData16Transaction(0xE1);
  IO.writeCommand16Transaction(0xD622); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD623); IO.writeData16Transaction(0x10);
  IO.writeCommand16Transaction(0xD624); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD625); IO.writeData16Transaction(0x31);
  IO.writeCommand16Transaction(0xD626); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD627); IO.writeData16Transaction(0x5A);
  IO.writeCommand16Transaction(0xD628); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD629); IO.writeData16Transaction(0x73);
  IO.writeCommand16Transaction(0xD62A); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD62B); IO.writeData16Transaction(0x94);
  IO.writeCommand16Transaction(0xD62C); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD62D); IO.writeData16Transaction(0x9F);
  IO.writeCommand16Transaction(0xD62E); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD62F); IO.writeData16Transaction(0xB3);
  IO.writeCommand16Transaction(0xD630); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD631); IO.writeData16Transaction(0xB9);
  IO.writeCommand16Transaction(0xD632); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xD633); IO.writeData16Transaction(0xC1);



  //#Enable Page0
  IO.writeCommand16Transaction(0xF000); IO.writeData16Transaction(0x55);
  IO.writeCommand16Transaction(0xF001); IO.writeData16Transaction(0xAA);
  IO.writeCommand16Transaction(0xF002); IO.writeData16Transaction(0x52);
  IO.writeCommand16Transaction(0xF003); IO.writeData16Transaction(0x08);
  IO.writeCommand16Transaction(0xF004); IO.writeData16Transaction(0x00);
  //# RGB I/F Setting
  IO.writeCommand16Transaction(0xB000); IO.writeData16Transaction(0x08);
  IO.writeCommand16Transaction(0xB001); IO.writeData16Transaction(0x05);
  IO.writeCommand16Transaction(0xB002); IO.writeData16Transaction(0x02);
  IO.writeCommand16Transaction(0xB003); IO.writeData16Transaction(0x05);
  IO.writeCommand16Transaction(0xB004); IO.writeData16Transaction(0x02);
  //## SDT:
  IO.writeCommand16Transaction(0xB600); IO.writeData16Transaction(0x08);
  IO.writeCommand16Transaction(0xB500); IO.writeData16Transaction(0x50);//0x6b ???? 480x854       0x50 ???? 480x800

  //## Gate EQ:
  IO.writeCommand16Transaction(0xB700); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xB701); IO.writeData16Transaction(0x00);

  //## Source EQ:
  IO.writeCommand16Transaction(0xB800); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xB801); IO.writeData16Transaction(0x05);
  IO.writeCommand16Transaction(0xB802); IO.writeData16Transaction(0x05);
  IO.writeCommand16Transaction(0xB803); IO.writeData16Transaction(0x05);

  //# Inversion: Column inversion (NVT)
  IO.writeCommand16Transaction(0xBC00); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xBC01); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xBC02); IO.writeData16Transaction(0x00);

  //# BOE's Setting(default)
  IO.writeCommand16Transaction(0xCC00); IO.writeData16Transaction(0x03);
  IO.writeCommand16Transaction(0xCC01); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0xCC02); IO.writeData16Transaction(0x00);

  //# Display Timing:
  IO.writeCommand16Transaction(0xBD00); IO.writeData16Transaction(0x01);
  IO.writeCommand16Transaction(0xBD01); IO.writeData16Transaction(0x84);
  IO.writeCommand16Transaction(0xBD02); IO.writeData16Transaction(0x07);
  IO.writeCommand16Transaction(0xBD03); IO.writeData16Transaction(0x31);
  IO.writeCommand16Transaction(0xBD04); IO.writeData16Transaction(0x00);

  IO.writeCommand16Transaction(0xBA00); IO.writeData16Transaction(0x01);

  IO.writeCommand16Transaction(0xFF00); IO.writeData16Transaction(0xAA);
  IO.writeCommand16Transaction(0xFF01); IO.writeData16Transaction(0x55);
  IO.writeCommand16Transaction(0xFF02); IO.writeData16Transaction(0x25);
  IO.writeCommand16Transaction(0xFF03); IO.writeData16Transaction(0x01);

  IO.writeCommand16Transaction(0x3500); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0x3600); IO.writeData16Transaction(0x00);
  IO.writeCommand16Transaction(0x3a00); IO.writeData16Transaction(0x55);  ////55=16?/////66=18?
  IO.writeCommand16Transaction(0x1100);
  delay(120);
  IO.writeCommand16Transaction(0x2900 );
  IO.writeCommand16Transaction(0x2c00);
  setWindowAddress(0, 0, 0x01e0, 0x0320);
}

void GxCTRL_NT35510::setWindowAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  uint16_t t_delay = 0;
  IO.writeCommand16(0x2A00);
  IO.writeData16(x0 >> 8);
  //t_delay++;
  IO.writeCommand16(0x2A01);
  IO.writeData16(x0 & 0x00ff);
  //t_delay++;
  IO.writeCommand16(0x2A02);
  IO.writeData16(x1 >> 8);
  //t_delay++;
  IO.writeCommand16(0x2A03);
  IO.writeData16(x1 & 0x00ff);
  //t_delay++;
  IO.writeCommand16(0x2B00);
  IO.writeData16(y0 >> 8);
  //t_delay++;
  IO.writeCommand16(0x2B01);
  IO.writeData16(y0 & 0x00ff);
  //t_delay++;
  IO.writeCommand16(0x2B02);
  IO.writeData16(y1 >> 8);
  //t_delay++;
  IO.writeCommand16(0x2B03);
  IO.writeData16(y1 & 0x00ff);
  //t_delay++;
  IO.writeCommand16(0x2C00);
  t_delay++;
}

void GxCTRL_NT35510::setRotation(uint8_t r)
{
  IO.startTransaction();
  IO.writeCommand16(MADCTL);
  switch (r & 3)
  {
    case 0:
      IO.writeData(0);
      break;
    case 1:
      IO.writeData(MADCTL_MX | MADCTL_MV);
      break;
    case 2:
      IO.writeData(MADCTL_MX | MADCTL_MY);
      break;
    case 3:
      IO.writeData(MADCTL_MY | MADCTL_MV);
      break;
  }
  IO.endTransaction();
}
