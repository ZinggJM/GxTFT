// created by Jean-Marc Zingg to be the GxIO_STM32F103ZET6M3_FSMC io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F103ZE boards with FMSC TFT connector, e.g. for 
// https://www.aliexpress.com/item/32720494327.html
// for details of the board see https://stm32-base.org/boards/STM32F103ZET6-Euse-M3-DEMO
//
// the above board has matched TFT connector for this display
// https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// or e.g. https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F103Z doc.
//
// this io class can be used with or adapted to other STM32F103Z processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "Generic F103ZE".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_Generic_F103ZE)

#include "GxIO_STM32F103ZET6M3_FSMC.h"

// TFT connector uses FSMC pins
// D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
// PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

// connector pins
// 01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29   30   31   32  33  34
// CS  RS  WR  RD  RST D0  D1  D2  D3  D4  D5  D6  D7  D8  D9  D10 D11 D12 D13 D14 D15 NC  LIG 3.3 3.3 GND GND NC  MISO MOSI TINT NC  TCS SCK
// G12 G0  D5  D4  C5  D14 D15 D0  D1  E7  E8  E9  E10 E11 E12 E13 E14 E15 D8  D9  D10 C2  B0  3.3 3.3 GND GND NC  F8   F9   F10  C3  B2  B1

// D used : 15,14,..,..,..,10, 9, 8,..,.., 5, 4,..,.., 1, 0, // 9
// D data : 15,14,..,..,..,10, 9, 8,..,..,..,..,..,.., 1, 0, // 7
//         |           |           |           |           |
// E used : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9
// E data : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9

//                    |       |        | // Ruler on 8 & 16
#define PD_DATA_BITS 0b1100011100000011
#define PD_CTRL_BITS 0b0000000000110000  // PD5(FSMC_NWE),PD4(FSMC_NOE)
#define PD_CRH_MASK  0xFF000FFF
#define PD_CRH_FSMC  0xBB000BBB          // AF PP 10MHz
#define PD_CRL_MASK          0x00FF00FF
#define PD_CRL_FSMC          0x00BB00BB  // AF PP 10MHz
//                    |       |        |
#define PE_DATA_BITS 0b1111111110000000
//                    |       |        |
#define PE_CRH_MASK  0xFFFFFFFF          // data bits used 15..8
#define PE_CRH_FSMC  0xBBBBBBBB          // AF PP 10MHz
#define PE_CRL_MASK          0xF0000000  // data bits used 7..0
#define PE_CRL_FSMC          0xB0000000  // AF PP 10MHz
//                    |       |        |
//                    |       |        |
#define PG_CTRL_BITS 0b0001000000000001  // PG12(FSMC_NE4),PG0(FSMC_A10)
#define PG_FSMC_BITS 0b0001000000000001
#define PG_CRH_MASK  0x000F0000          // data bits used 15..8
#define PG_CRH_FSMC  0x000B0000          // AF PP 10MHz
#define PG_CRL_MASK          0x0000000F  // data bits used 7..0
#define PG_CRL_FSMC          0x0000000B  // AF PP 10MHz

// 80 MHz processor clock
#define ADDSET 7 // 100ns (ADDSET+1)*12.5ns = CS to RW
#define DATAST 3 // 50ns  (DATAST+1)*12.5ns = RW length

#define CommandAccess FSMC_BANK1_4 // FSMC_NE4
#define DataAccess (FSMC_BANK1_4 + 0x800) // FSMC_A10

GxIO_STM32F103ZET6M3_FSMC::GxIO_STM32F103ZET6M3_FSMC(bool bl_active_high)
{
  _cs   = PG12; // FSMC_NE4
  _rs   = PG0;  // FSMC_A10
  _rst  = PC5;  //
  _wr   = PD5;  // FSMC_NWE
  _rd   = PD4;  // FSMC_NOE
  _bl   = PB0;
  _bl_active_high = bl_active_high;
}

void GxIO_STM32F103ZET6M3_FSMC::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GxIO_STM32F103ZET6M3_FSMC::init()
{
  RCC->AHBENR |= 0x00000100; // enable FSMC clock
  uint32_t t = RCC->AHBENR; // delay
  RCC->APB2ENR |= 0x000001E0; // enable port G,F,E,D clocks
  t = RCC->APB2ENR; // delay
  (void)(t);
  GPIOD->CRH = (GPIOD->CRH & ~PD_CRH_MASK) | PD_CRH_FSMC;
  GPIOD->CRL = (GPIOD->CRL & ~PD_CRL_MASK) | PD_CRL_FSMC;
  GPIOE->CRH = (GPIOE->CRH & ~PE_CRH_MASK) | PE_CRH_FSMC;
  GPIOE->CRL = (GPIOE->CRL & ~PE_CRL_MASK) | PE_CRL_FSMC;
  GPIOG->CRH = (GPIOG->CRH & ~PG_CRH_MASK) | PG_CRH_FSMC;
  GPIOG->CRL = (GPIOG->CRL & ~PG_CRL_MASK) | PG_CRL_FSMC;
  FSMC_Bank1->BTCR[6] = 0x000010D9;
  FSMC_Bank1->BTCR[7] = (DATAST << 8) | ADDSET;
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, HIGH);
  pinMode(_bl, OUTPUT);
  digitalWrite(_bl, HIGH);
  reset();
}

uint8_t GxIO_STM32F103ZET6M3_FSMC::readDataTransaction()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GxIO_STM32F103ZET6M3_FSMC::readData16Transaction()
{
  return *(uint16_t*)DataAccess;
}

uint8_t GxIO_STM32F103ZET6M3_FSMC::readData()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GxIO_STM32F103ZET6M3_FSMC::readData16()
{
  return *(uint16_t*)DataAccess;
}

uint32_t GxIO_STM32F103ZET6M3_FSMC::readRawData32(uint8_t part)
{
  return *(uint16_t*)DataAccess;
}

void GxIO_STM32F103ZET6M3_FSMC::writeCommandTransaction(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GxIO_STM32F103ZET6M3_FSMC::writeCommand16Transaction(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GxIO_STM32F103ZET6M3_FSMC::writeDataTransaction(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GxIO_STM32F103ZET6M3_FSMC::writeData16Transaction(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GxIO_STM32F103ZET6M3_FSMC::writeCommand(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GxIO_STM32F103ZET6M3_FSMC::writeCommand16(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GxIO_STM32F103ZET6M3_FSMC::writeData(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GxIO_STM32F103ZET6M3_FSMC::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    *(uint8_t*)DataAccess = *d;
    d++;
    num--;
  }
}

void GxIO_STM32F103ZET6M3_FSMC::writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GxIO_STM32F103ZET6M3_FSMC::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_STM32F103ZET6M3_FSMC::startTransaction()
{
}

void GxIO_STM32F103ZET6M3_FSMC::endTransaction()
{
}

void GxIO_STM32F103ZET6M3_FSMC::selectRegister(bool rs_low)
{
}

void GxIO_STM32F103ZET6M3_FSMC::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif
#endif
