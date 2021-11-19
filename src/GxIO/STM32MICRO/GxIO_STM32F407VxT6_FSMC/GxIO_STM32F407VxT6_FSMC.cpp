// created by Jean-Marc Zingg to be the GxIO_STM32F407VxT6_FSMC io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F407VxT6 board with FMSC TFT connector, e.g. for https://www.aliexpress.com/item/1005001806399987.html
// details available here: http://www.lcdwiki.com/STM32F407VxT6_Board
//
// and for several direct matching displays
// e.g. https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// e.g. "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" https://www.aliexpress.com/item/32676929794.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F407V doc.
//
// this io class can be used with or adapted to other STM32F407V/Z processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F4 series" part "Generic F407VE" or "Generic F407VG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F407xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F407VETX) || defined(ARDUINO_GENERIC_F407VGTX)

#include "GxIO_STM32F407VxT6_FSMC.h"

// TFT connector uses FSMC pins
// D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
// PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

// connector pins
// 01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29   30   31   32  33  34
// CS  RS  WR  RD  RST D0  D1  D2  D3  D4  D5  D6  D7  D8  D9  D10 D11 D12 D13 D14 D15 NC  LIG 3.3 3.3 GND GND NC  MISO MOSI TINT NC  TCS SCK
// D7  D11 D5  D4  RST D14 D15 D0  D1  E7  E8  E9  E10 E11 E12 E13 E14 E15 D8  D9  D10 GND B15 3.3 3.3 GND GND 5V  B2   C4   B1   NC  C13 B0

// D used : 15,14,..,..,11,10, 9, 8, 7,.., 5, 4,..,.., 1, 0, // 11
// D data : 15,14,..,..,..,10, 9, 8,..,..,..,..,..,.., 1, 0, // 7
//         |           |           |           |           |
// E used : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9
// E data : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9

//                    |       |        | // Ruler on 8 & 16
#define PD_DATA_BITS 0b1100011100000011
#define PD_CTRL_BITS 0b0000100010110000  // 
#define PD_FSMC_BITS 0b1100111110110011
#define PD_AFR0_MASK         0xF0FF00FF
#define PD_AFR0_FSMC         0xC0CC00CC
#define PD_AFR1_MASK 0xFF00FFFF
#define PD_AFR1_FSMC 0xCC00CCCC
//                    |       |        |
// PD_MODE_MASK      0q3300333330330033 // quaternary notation, 2bit per digit
// PD_MODE_FSMC      0q2200222220220022 //
// PD_OSPD_FSMC      0q1100111110110011 //
#define PD_MODE_MASK 0xF0FFCF0F // all FMSC MODE bits
#define PD_MODE_FSMC 0xA0AA8A0A // FMSC MODE values 10 alternate function
#define PD_OSPD_FSMC 0x50554505 // FMSC OSPEED values 01 10MHz
//                    |       |        |
#define PE_DATA_BITS 0b1111111110000000
#define PE_AFR0_MASK         0xF0000000
#define PE_AFR0_FSMC         0xC0000000
#define PE_AFR1_MASK 0xFFFFFFFF
#define PE_AFR1_FSMC 0xCCCCCCCC
//                    |       |        |
#define PE_MODE_MASK 0xFFFFC000 // all FMSC MODE bits
#define PE_MODE_FSMC 0xAAAA8000 // FMSC MODE values 10 alternate function
#define PE_OSPD_FSMC 0x55554000 // FMSC OSPEED values 01 10MHz

#define ADDSET 10 // (ADDSET+1)*6ns = CS to RW
#define DATAST 15 // (DATAST+1)*6ns = RW length

#define FMC_REGION ((uint32_t)0x60000000) // Bank1 FMC NOR/PSRAM

#define CommandAccess FMC_REGION
#define DataAccess (FMC_REGION + 0x20000) // FSMC_A16

GxIO_STM32F407VxT6_FSMC::GxIO_STM32F407VxT6_FSMC(bool bl_active_high)
{
  _cs   = PD7;  // FSMC_NE1
  _rs   = PD11; // FSMC_A16
  _rst  = 0;    // not available, driven from NRST
  _wr   = PD5;  // FSMC_NWE
  _rd   = PD4;  // FSMC_NOE
  _bl   = PB15;
  _bl_active_high = bl_active_high;
}

void GxIO_STM32F407VxT6_FSMC::reset()
{
  // not available, driven from NRST
  delay(20); // delay anyway, some displays need it after reset
}

void GxIO_STM32F407VxT6_FSMC::init()
{
  RCC->AHB1ENR |= 0x00000078; // enable GPIOD, GPIOE, GPIOF and GPIOG interface clock
  volatile uint32_t t = RCC->AHB1ENR; // delay

  GPIOD->AFR[0]  = ( GPIOD->AFR[0] & ~PD_AFR0_MASK) | PD_AFR0_FSMC;
  GPIOD->AFR[1]  = ( GPIOD->AFR[1] & ~PD_AFR1_MASK) | PD_AFR1_FSMC;
  GPIOD->MODER   = ( GPIOD->MODER & ~PD_MODE_MASK) | PD_MODE_FSMC;
  GPIOD->OSPEEDR = ( GPIOD->OSPEEDR & ~PD_MODE_MASK) | PD_OSPD_FSMC;
  GPIOD->OTYPER &= ~PD_MODE_MASK;
  GPIOD->PUPDR  &= ~PD_MODE_MASK;

  GPIOE->AFR[0]  = (GPIOE->AFR[0] & ~PE_AFR0_MASK) | PE_AFR0_FSMC;
  GPIOE->AFR[1]  = (GPIOE->AFR[1] & ~PE_AFR1_MASK) | PE_AFR1_FSMC;
  GPIOE->MODER   = (GPIOE->MODER & ~PE_MODE_MASK) | PE_MODE_FSMC;
  GPIOE->OSPEEDR = (GPIOE->OSPEEDR & ~PE_MODE_MASK) | PE_OSPD_FSMC;
  GPIOE->OTYPER &= ~PE_MODE_MASK;
  GPIOE->PUPDR  &= ~PE_MODE_MASK;

  RCC->AHB3ENR |= 0x00000001;
  t = RCC->AHB1ENR; // delay
  (void)(t);

  FSMC_Bank1->BTCR[0] = 0x000010D9;
  FSMC_Bank1->BTCR[1] = (DATAST << 8) | ADDSET;

  digitalWrite(_bl, LOW);
  pinMode(_bl, OUTPUT);
  reset();
}

uint8_t GxIO_STM32F407VxT6_FSMC::readDataTransaction()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GxIO_STM32F407VxT6_FSMC::readData16Transaction()
{
  return *(uint16_t*)DataAccess;
}

uint8_t GxIO_STM32F407VxT6_FSMC::readData()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GxIO_STM32F407VxT6_FSMC::readData16()
{
  return *(uint16_t*)DataAccess;
}

uint32_t GxIO_STM32F407VxT6_FSMC::readRawData32(uint8_t part)
{
  return *(uint16_t*)DataAccess;
}

void GxIO_STM32F407VxT6_FSMC::writeCommandTransaction(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GxIO_STM32F407VxT6_FSMC::writeCommand16Transaction(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GxIO_STM32F407VxT6_FSMC::writeDataTransaction(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GxIO_STM32F407VxT6_FSMC::writeData16Transaction(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GxIO_STM32F407VxT6_FSMC::writeCommand(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GxIO_STM32F407VxT6_FSMC::writeCommand16(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GxIO_STM32F407VxT6_FSMC::writeData(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GxIO_STM32F407VxT6_FSMC::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    *(uint8_t*)DataAccess = *d;
    d++;
    num--;
  }
}

void GxIO_STM32F407VxT6_FSMC::writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GxIO_STM32F407VxT6_FSMC::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_STM32F407VxT6_FSMC::startTransaction()
{
}

void GxIO_STM32F407VxT6_FSMC::endTransaction()
{
}

void GxIO_STM32F407VxT6_FSMC::selectRegister(bool rs_low)
{
}

void GxIO_STM32F407VxT6_FSMC::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit == _bl_active_high));
}

#endif
#endif
