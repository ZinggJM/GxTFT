// created by Jean-Marc Zingg to be the GxIO_STM32F103V_P16_TIKY io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html
//
// for the STM32F103V board with matching TFT connector (FSMC pins)
// https://www.aliexpress.com/item/STM32-core-development-TFT-LCD-screen-evaluation-board-with-high-speed-FSMC-SDIO-interface/32667841009.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F103V doc.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "GENERIC F103VC".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F103VC) || defined(ARDUINO_GENERIC_F103VD) || defined(ARDUINO_GENERIC_F103VE) || defined(ARDUINO_GENERIC_F103VF)

#include "GxIO_STM32F103V_P16_TIKY.h"

// TFT connector uses FSMC pins
// D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
// PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

// connector pins
// 01  02  03  04  05  06  07  08  09  10  11  12   13  14  15   16   17   18  19  20  21  22  23   24   25   26   27   28   29  30  31   32  33  34  35  36  37  38  39  40
// 5V  GND D23 D22 D21 D20 D19 D18 D17 D16 CS  RS   WR  RD  RST  D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15  SCK TCS TCK TDO TDI TIQ BL  GND 3.3
// 5V  GND                                 PD7 PD11 PD5 PD4 PD13 PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

//                    |       |        | // Ruler on 8 & 16
#define PD_DATA_BITS 0b1100011100000011;
//                    |       |        |
#define PD_CRH_DATA  0xFF000FFF          // data bits used 15..8
#define PD_CRH_OUTP  0x33000333          // 00 11 output PP, 50MHz
#define PD_CRH_INP   0x44000444          // 01 00 input floating
#define PD_CRL_DATA          0x000000FF  // data bits used 7..0
#define PD_CRL_OUTP          0x00000033  // 00 11 output PP, 50MHz
#define PD_CRL_INP           0x00000044  // 01 00 input floating
//                    |       |        |
#define PE_DATA_BITS 0b1111111110000000;
//                    |       |        |
#define PE_CRH_DATA  0xFFFFFFFF          // data bits used 15..8
#define PE_CRH_OUTP  0x33333333          // 00 11 output PP, 50MHz
#define PE_CRH_INP   0x44444444          // 01 00 input floating
#define PE_CRL_DATA          0xF0000000  // data bits used 7..0
#define PE_CRL_OUTP          0x30000000  // 00 11 output PP, 50MHz
#define PE_CRL_INP           0x40000000  // 01 00 input floating
//                    |       |        |

GxIO_STM32F103V_P16_TIKY::GxIO_STM32F103V_P16_TIKY()
{
  _cs   = PD7;  // FSMC_NE1
  _rs   = PD11; // FSMC_A16
  _rst  = PD13;
  _wr   = PD5;  // FSMC_NWE
  _rd   = PD4;  // FSMC_NOE
  _bl   = PA1;
}

void GxIO_STM32F103V_P16_TIKY::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GxIO_STM32F103V_P16_TIKY::init()
{
  RCC->APB2ENR |= 0x00000060; // enable port D & E clocks
  uint32_t t = RCC->AHBENR; // delay
  (void)(t);
  digitalWrite(_rst, HIGH);
  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_wr, HIGH);
  digitalWrite(_rd, HIGH);
  digitalWrite(_bl, LOW);
  pinMode(_rst, OUTPUT);
  pinMode(_rs, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_wr, OUTPUT);
  pinMode(_rd, OUTPUT);
  pinMode(_bl, OUTPUT);
  reset();
  setDataPinsOutput();
}

void GxIO_STM32F103V_P16_TIKY::setDataPinsOutput()
{
  GPIOD->CRH &= ~PD_CRH_DATA;
  GPIOD->CRH |= PD_CRH_OUTP;
  GPIOD->CRL &= ~PD_CRL_DATA;
  GPIOD->CRL |= PD_CRL_OUTP;
  GPIOE->CRH &= ~PE_CRH_DATA;
  GPIOE->CRH |= PE_CRH_OUTP;
  GPIOE->CRL &= ~PE_CRL_DATA;
  GPIOE->CRL |= PE_CRL_OUTP;
}

void GxIO_STM32F103V_P16_TIKY::setDataPinsInput()
{
  GPIOD->CRH &= ~PD_CRH_DATA;
  GPIOD->CRH |= PD_CRH_INP;
  GPIOD->CRL &= ~PD_CRL_DATA;
  GPIOD->CRL |= PD_CRL_INP;
  GPIOE->CRH &= ~PE_CRH_DATA;
  GPIOE->CRH |= PE_CRH_INP;
  GPIOE->CRL &= ~PE_CRL_DATA;
  GPIOE->CRL |= PE_CRL_INP;
}

uint8_t GxIO_STM32F103V_P16_TIKY::readDataTransaction()
{
  return readData16Transaction();
}

uint16_t GxIO_STM32F103V_P16_TIKY::readData16Transaction()
{
  GPIOD->BRR = (0x1 << 7);  // PD7 CS low
  uint16_t rv = readData16();
  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
  return rv;
}

uint8_t GxIO_STM32F103V_P16_TIKY::readData()
{
  return readData16();
}

uint16_t GxIO_STM32F103V_P16_TIKY::readData16()
{
  setDataPinsInput();
//  GPIOD->BRR = (0x1 << 4);  // PD4 RD low pulse
//  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  GPIOD->BRR = (0x1 << 4);  // PD4 RD low read
  uint16_t rv = 0;
  // The compiler efficiently codes this
  // so it is quite quick.                    Port.bit
  rv |= (GPIOD->IDR & (0x1 << 10)) << (15 - 10); // PD10
  rv |= (GPIOD->IDR & (0x1 << 9)) << (14 - 9); // PD9
  rv |= (GPIOD->IDR & (0x1 << 8)) << (13 - 8); // PD8
  rv |= (GPIOE->IDR & (0x1 << 15)) >> -(12 - 15); // PE15
  rv |= (GPIOE->IDR & (0x1 << 14)) >> -(11 - 14); // PE14
  rv |= (GPIOE->IDR & (0x1 << 13)) >> -(10 - 13); // PE13
  rv |= (GPIOE->IDR & (0x1 << 12)) >> -(9 - 12); // PE12
  rv |= (GPIOE->IDR & (0x1 << 11)) >> -(8 - 11); // PE11
  // so it is quite quick.                    Port.bit
  rv |= (GPIOE->IDR & (0x1 << 10)) >> -(7 - 10); // PE10
  rv |= (GPIOE->IDR & (0x1 << 9)) >> -(6 - 9); // PE9
  rv |= (GPIOE->IDR & (0x1 << 8)) >> -(5 - 8); // PE8
  rv |= (GPIOE->IDR & (0x1 << 7)) >> -(4 - 7); // PE7
  rv |= (GPIOD->IDR & (0x1 << 1)) << (3 - 1); // PD1
  rv |= (GPIOD->IDR & (0x1 << 0)) << (2 - 0); // PD0
  rv |= (GPIOD->IDR & (0x1 << 15)) >> -(1 - 15); // PD15
  rv |= (GPIOD->IDR & (0x1 << 14)) >> -(0 - 14); // PD14
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  setDataPinsOutput();
  return rv;
}

uint32_t GxIO_STM32F103V_P16_TIKY::readRawData32(uint8_t part)
{
  setDataPinsInput();
  GPIOD->BRR = (0x1 << 4);  // PD4 RD low pulse
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  GPIOD->BRR = (0x1 << 4);  // PD4 RD low read
  uint32_t rv = 0;
  if (part == 0) rv = GPIOD->IDR & PD_DATA_BITS;
  if (part == 1) rv = GPIOE->IDR & PE_DATA_BITS;
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  setDataPinsOutput();
  return rv;
}

void GxIO_STM32F103V_P16_TIKY::writeCommandTransaction(uint8_t c)
{
  GPIOD->BRR = (0x1 << 7);  // PD7 CS low
  GPIOD->BRR = (0x1 << 11);  // PD11 RS low
  writeData16(c);
  GPIOD->BSRR = (0x1 << 11);  // PD11 RS high
  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
}

void GxIO_STM32F103V_P16_TIKY::writeCommand16Transaction(uint16_t c)
{
  GPIOD->BRR = (0x1 << 7);  // PD7 CS low
  GPIOD->BRR = (0x1 << 11);  // PD11 RS low
  writeData16(c);
  GPIOD->BSRR = (0x1 << 11);  // PD11 RS high
  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
}

void GxIO_STM32F103V_P16_TIKY::writeDataTransaction(uint8_t d)
{
  GPIOD->BRR = (0x1 << 7);  // PD7 CS low
  writeData16(d);
  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
}

void GxIO_STM32F103V_P16_TIKY::writeData16Transaction(uint16_t d, uint32_t num)
{
  GPIOD->BRR = (0x1 << 7);  // PD7 CS low
  writeData16(d, num);
  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
}

void GxIO_STM32F103V_P16_TIKY::writeCommand(uint8_t c)
{
  GPIOD->BRR = (0x1 << 11);  // PD11 RS low
  writeData16(c);
  GPIOD->BSRR = (0x1 << 11);  // PD11 RS high
}

void GxIO_STM32F103V_P16_TIKY::writeCommand16(uint16_t c)
{
  GPIOD->BRR = (0x1 << 11);  // PD11 RS low
  writeData16(c);
  GPIOD->BSRR = (0x1 << 11);  // PD11 RS high
}

void GxIO_STM32F103V_P16_TIKY::writeData(uint8_t d)
{
  writeData16(d);
}

void GxIO_STM32F103V_P16_TIKY::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void GxIO_STM32F103V_P16_TIKY::writeData16(uint16_t d, uint32_t num)
{
  //                 |       |        | // Ruler on 8 & 16 (hopefully)
  GPIOD->BRR = 0b1100011100000011; // Clear bits
  //                 |       |        |
  GPIOE->BRR = 0b1111111110000000; // Clear bits

  // The compiler efficiently codes this
  // so it is quite quick.                         Port.bit
  if (d & 0x8000) GPIOD->BSRR = 0x1 << 10; // PD10
  if (d & 0x4000) GPIOD->BSRR = 0x1 << 9;  // PD9
  if (d & 0x2000) GPIOD->BSRR = 0x1 << 8;  // PD8
  if (d & 0x1000) GPIOE->BSRR = 0x1 << 15; // PE15
  if (d & 0x0800) GPIOE->BSRR = 0x1 << 14; // PE14
  if (d & 0x0400) GPIOE->BSRR = 0x1 << 13; // PE13
  if (d & 0x0200) GPIOE->BSRR = 0x1 << 12; // PE12
  if (d & 0x0100) GPIOE->BSRR = 0x1 << 11; // PE11

  // so it is quite quick.                         Port.bit
  if (d & 0x0080) GPIOE->BSRR = 0x1 << 10; // PE10
  if (d & 0x0040) GPIOE->BSRR = 0x1 << 9;  // PE9
  if (d & 0x0020) GPIOE->BSRR = 0x1 << 8;  // PE8
  if (d & 0x0010) GPIOE->BSRR = 0x1 << 7;  // PE7
  if (d & 0x0008) GPIOD->BSRR = 0x1 << 1;  // PD1
  if (d & 0x0004) GPIOD->BSRR = 0x1 << 0;  // PD0
  if (d & 0x0002) GPIOD->BSRR = 0x1 << 15; // PD15
  if (d & 0x0001) GPIOD->BSRR = 0x1 << 14; // PD14
  while (num > 0)
  {
    GPIOD->BRR = (0x1 << 5);  // PD5 WR low
    GPIOD->BRR = (0x1 << 5);  // PD5 WR low
    GPIOD->BSRR = (0x1 << 5); // PD5 WR high
    num--;
  }
}

void GxIO_STM32F103V_P16_TIKY::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_STM32F103V_P16_TIKY::startTransaction()
{
  GPIOD->BRR = (0x1 << 7);  // PD7 CS low
}

void GxIO_STM32F103V_P16_TIKY::endTransaction()
{
  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
}

void GxIO_STM32F103V_P16_TIKY::selectRegister(bool rs_low)
{
  digitalWrite(_rs, (rs_low ? LOW : HIGH));
}

void GxIO_STM32F103V_P16_TIKY::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif
#endif
