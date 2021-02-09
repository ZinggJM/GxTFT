// created by Jean-Marc Zingg to be the GxIO_STM32F407ZET6_P16 io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a wiring for the TFT connector of the black STM32F407ZET6 board, e.g.
// https://www.aliexpress.com/item/32689262341.html
// for details of the board see https://github.com/mcauser/BLACK_F407ZE
//
// e.g. for direct matching display
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), 
// board "Generic STM32F4 series" part "BLACK F407VE", or "BLACK F407VG", or "BLACK F407ZE", or "BLACK F407ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F407xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_BLACK_F407ZE) || defined(ARDUINO_BLACK_F407ZG)

#include "GxIO_STM32F407ZET6_P16.h"

// TFT connector uses FSMC pins
// D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
// PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

// connector pins
// 01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32
// GND RST D15 D14 D13 D12 D11 D10 D9  D8  D7  D6  D5  D4  D3  D2  D1  D0  RD  WR  RS  CS  SCK SCS SI  SO  INT BLK SET GND 3.3 GND
//         D10 D9  D8  E15 E14 E13 E12 E11 E10 E9  E8  E7  D1  D0  D15 D14 D4  D5  F12 G12                     B15

// D used : 15,14,..,..,..,10, 9, 8,..,.., 5, 4,..,.., 1, 0, // 9
// D data : 15,14,..,..,..,10, 9, 8,..,..,..,..,..,.., 1, 0, // 7
//         |           |           |           |           |
// E used : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9
// E data : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9

#define PD_USED_BITS 0xC733
#define PD_DATA_BITS 0xC703

#define PE_USED_BITS 0xFF80
#define PE_DATA_BITS 0xFF80

// PD_MODE_MASK      0q3300033300330033 // quaternary notation, 2bit per digit
#define PD_MODE_MASK 0xF03F0F0F // all used bits, 0q'33'00'03'33'00'33'00'33
#define PD_MODE_BITS 0x50150505 // 01 : general purpose output mode
#define PE_MODE_MASK 0xFFFFC000 // all used bits
#define PE_MODE_BITS 0x55554000 // 01 : general purpose output mode

#define PD_MODE_DATA 0xF03F000F // all data bits
#define PD_MODE_OUTP 0x50150005 // 01 : general purpose output mode
#define PD_MODE_INP  0x00000000 // 00 : input floating mode
#define PE_MODE_DATA 0xFFFFC000 // all data bits
#define PE_MODE_OUTP 0x55554000 // 01 : general purpose output mode
#define PE_MODE_INP  0x00000000 // 00 : input floating mode

GxIO_STM32F407ZET6_P16::GxIO_STM32F407ZET6_P16(bool bl_active_high)
{
  _cs   = PG12;
  _rs   = PF12;
  _rst  = 0; // not available, driven from NRST
  _wr   = PD5;
  _rd   = PD4;
  _bl   = PB15;
  _bl_active_high = bl_active_high;
}

void GxIO_STM32F407ZET6_P16::reset()
{
  // _rst pin not available
}

void GxIO_STM32F407ZET6_P16::init()
{
  RCC->AHB1ENR |= 0x00000078; // enable GPIOD, GPIOE, GPIOF and GPIOG interface clock
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); // delay after an RCC peripheral clock enabling
  GPIOD->BSRR |= PD_USED_BITS; // preset all output high
  GPIOD->MODER &= ~PD_MODE_MASK;
  GPIOD->MODER |= PD_MODE_BITS;
  GPIOD->OTYPER &= ~PD_USED_BITS; // 0 : output push-pull
  GPIOD->OSPEEDR &= ~PD_MODE_MASK; // 0 : low speed
  GPIOD->PUPDR &= ~PD_MODE_MASK; // 0 : no pull-up, no pull-down

  GPIOE->BSRR |= PE_USED_BITS; // preset all output high
  GPIOE->MODER &= ~PE_MODE_MASK;
  GPIOE->MODER |= PE_MODE_BITS;
  GPIOE->OTYPER &= ~PE_USED_BITS; // 0 : output push-pull
  GPIOE->OSPEEDR &= ~PE_MODE_MASK; // 0 : low speed
  GPIOE->PUPDR &= ~PE_MODE_MASK; // 0 : no pull-up, no pull-down

  digitalWrite(_cs, HIGH);
  pinMode(_cs, OUTPUT);
  digitalWrite(_rs, HIGH);
  pinMode(_rs, OUTPUT);
  digitalWrite(_bl, LOW);
  pinMode(_bl, OUTPUT);

  reset();
}

uint8_t GxIO_STM32F407ZET6_P16::readDataTransaction()
{
  return readData16Transaction();
}

uint16_t GxIO_STM32F407ZET6_P16::readData16Transaction()
{
  GPIOG->BSRR = (0x1 << (12 + 16));  // PG12 CS low
  uint16_t rv = readData16();
  GPIOG->BSRR = (0x1 << 12);  // PG12 CS high
  return rv;
}

uint8_t GxIO_STM32F407ZET6_P16::readData()
{
  return readData16();
}

uint16_t GxIO_STM32F407ZET6_P16::readData16()
{
  // Set direction input
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER &= ~PE_MODE_DATA;
//  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low pulse
//  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low read
  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low read
  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low read
  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low read
  uint16_t rv = 0;
  // The compiler efficiently codes this  so it is quite quick.
  rv |= (GPIOD->IDR & (0x1 << 10)) << (15 - 10); // PD10
  rv |= (GPIOD->IDR & (0x1 << 9)) << (14 - 9); // PD9
  rv |= (GPIOD->IDR & (0x1 << 8)) << (13 - 8); // PD8
  rv |= (GPIOE->IDR & (0x1 << 15)) >> -(12 - 15); // PE15
  rv |= (GPIOE->IDR & (0x1 << 14)) >> -(11 - 14); // PE14
  rv |= (GPIOE->IDR & (0x1 << 13)) >> -(10 - 13); // PE13
  rv |= (GPIOE->IDR & (0x1 << 12)) >> -(9 - 12); // PE12
  rv |= (GPIOE->IDR & (0x1 << 11)) >> -(8 - 11); // PE11
  rv |= (GPIOE->IDR & (0x1 << 10)) >> -(7 - 10); // PE10
  rv |= (GPIOE->IDR & (0x1 << 9)) >> -(6 - 9); // PE9
  rv |= (GPIOE->IDR & (0x1 << 8)) >> -(5 - 8); // PE8
  rv |= (GPIOE->IDR & (0x1 << 7)) >> -(4 - 7); // PE7
  rv |= (GPIOD->IDR & (0x1 << 1)) << (3 - 1); // PD1
  rv |= (GPIOD->IDR & (0x1 << 0)) << (2 - 0); // PD0
  rv |= (GPIOD->IDR & (0x1 << 15)) >> -(1 - 15); // PD15
  rv |= (GPIOD->IDR & (0x1 << 14)) >> -(0 - 14); // PD14
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  // Set direction output again
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOD->MODER |= PD_MODE_OUTP;
  GPIOE->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER |= PE_MODE_OUTP;
  return rv;
}

uint32_t GxIO_STM32F407ZET6_P16::readRawData32(uint8_t part)
{
  // Set direction input
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER &= ~PE_MODE_DATA;
  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low pulse
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low read
  uint32_t rv = 0;
  if (part == 0) rv = GPIOD->IDR & PD_DATA_BITS;
  if (part == 1) rv = GPIOE->IDR & PE_DATA_BITS;
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  // Set direction output again
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOD->MODER |= PD_MODE_OUTP;
  GPIOE->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER |= PE_MODE_OUTP;
  return rv;
}

void GxIO_STM32F407ZET6_P16::writeCommandTransaction(uint8_t c)
{
  GPIOG->BSRR = (0x1 << (12 + 16));  // PG12 CS low
  GPIOF->BSRR = (0x1 << (12 + 16));  // PF12 RS low
  writeData16(c);
  GPIOF->BSRR = (0x1 << 12);  // PF12 RS high
  GPIOG->BSRR = (0x1 << 12);  // PG12 CS high
}

void GxIO_STM32F407ZET6_P16::writeCommand16Transaction(uint16_t c)
{
  GPIOG->BSRR = (0x1 << (12 + 16));  // PG12 CS low
  GPIOF->BSRR = (0x1 << (12 + 16));  // PF12 RS low
  writeData16(c);
  GPIOF->BSRR = (0x1 << 12);  // PF12 RS high
  GPIOG->BSRR = (0x1 << 12);  // PG12 CS high
}

void GxIO_STM32F407ZET6_P16::writeDataTransaction(uint8_t d)
{
  GPIOG->BSRR = (0x1 << (12 + 16));  // PG12 CS low
  writeData16(d);
  GPIOG->BSRR = (0x1 << 12);  // PG12 CS high
}

void GxIO_STM32F407ZET6_P16::writeData16Transaction(uint16_t d, uint32_t num)
{
  GPIOG->BSRR = (0x1 << (12 + 16));  // PG12 CS low
  writeData16(d, num);
  GPIOG->BSRR = (0x1 << 12);  // PG12 CS high
}

void GxIO_STM32F407ZET6_P16::writeCommand(uint8_t c)
{
  GPIOF->BSRR = (0x1 << (12 + 16));  // PF12 RS low
  writeData16(c);
  GPIOF->BSRR = (0x1 << 12);  // PF12 RS high
}

void GxIO_STM32F407ZET6_P16::writeCommand16(uint16_t c)
{
  GPIOF->BSRR = (0x1 << (12 + 16));  // PF12 RS low
  writeData16(c);
  GPIOF->BSRR = (0x1 << 12);  // PF12 RS high
}

void GxIO_STM32F407ZET6_P16::writeData(uint8_t d)
{
  writeData16(d);
}

void GxIO_STM32F407ZET6_P16::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void GxIO_STM32F407ZET6_P16::writeData16(uint16_t d, uint32_t num)
{
  GPIOD->BSRR = PD_DATA_BITS << 16; // clear bits
  GPIOE->BSRR = PE_DATA_BITS << 16; // clear bits

  // TFT connector uses FSMC pins
  // D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
  // PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

  // The compiler efficiently codes this  so it is quite quick.
  if (d & 0x8000) GPIOD->BSRR = 0x1 << 10; // PD10
  if (d & 0x4000) GPIOD->BSRR = 0x1 << 9; // PD9
  if (d & 0x2000) GPIOD->BSRR = 0x1 << 8; // PD8
  if (d & 0x1000) GPIOE->BSRR = 0x1 << 15; // PE15
  if (d & 0x0800) GPIOE->BSRR = 0x1 << 14; // PE14
  if (d & 0x0400) GPIOE->BSRR = 0x1 << 13; // PE13
  if (d & 0x0200) GPIOE->BSRR = 0x1 << 12; // PE12
  if (d & 0x0100) GPIOE->BSRR = 0x1 << 11; // PE11

  if (d & 0x0080) GPIOE->BSRR = 0x1 << 10; // PE10
  if (d & 0x0040) GPIOE->BSRR = 0x1 << 9; // PE9
  if (d & 0x0020) GPIOE->BSRR = 0x1 << 8; // PE8
  if (d & 0x0010) GPIOE->BSRR = 0x1 << 7; // PE7
  if (d & 0x0008) GPIOD->BSRR = 0x1 << 1; // PD1
  if (d & 0x0004) GPIOD->BSRR = 0x1 << 0; // PD0
  if (d & 0x0002) GPIOD->BSRR = 0x1 << 15; // PD15
  if (d & 0x0001) GPIOD->BSRR = 0x1 << 14; // PD14
  while (num)
  {
    GPIOD->BSRR = (0x1 << (5 + 16));  // PD5 WR low
    GPIOD->BSRR = (0x1 << (5 + 16));  // PD5 WR low
    GPIOD->BSRR = (0x1 << (5 + 16));  // PD5 WR low
    GPIOD->BSRR = (0x1 << (5 + 16));  // PD5 WR low
    GPIOD->BSRR = (0x1 << 5); // PD5 WR high
    //digitalWrite(_wr, LOW);
    //digitalWrite(_wr, HIGH);
    num--;
  }
}

void GxIO_STM32F407ZET6_P16::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_STM32F407ZET6_P16::startTransaction()
{
  GPIOG->BSRR = (0x1 << (12 + 16));  // PG12 CS low
}

void GxIO_STM32F407ZET6_P16::endTransaction()
{
  GPIOG->BSRR = (0x1 << 12);  // PG12 CS high
}

void GxIO_STM32F407ZET6_P16::selectRegister(bool rs_low)
{
  digitalWrite(_rs, (rs_low ? LOW : HIGH));
}

void GxIO_STM32F407ZET6_P16::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit == _bl_active_high));
}

#endif
#endif
