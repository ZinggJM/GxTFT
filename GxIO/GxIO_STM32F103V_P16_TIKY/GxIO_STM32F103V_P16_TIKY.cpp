// created by Jean-Marc Zingg to be the GxIO_STM32F103V_P16_TIKY io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html

#if defined(ARDUINO_ARCH_STM32F1) && defined(MCU_STM32F103VC)

#include "GxIO_STM32F103V_P16_TIKY.h"

GxIO_STM32F103V_P16_TIKY::GxIO_STM32F103V_P16_TIKY()
{
  _cs   = PD7;  // FSMC_NE1
  _rs   = PD11; // FSMC_A16
  _rst  = PD13;
  _wr   = PD5;  // FSMC_NWE
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
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, HIGH);

  pinMode(_rs, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_wr, OUTPUT);
  pinMode(_bl, OUTPUT);

  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_wr, HIGH);
  digitalWrite(_bl, LOW);

  reset();

  pinMode(PD14, OUTPUT);
  pinMode(PD15, OUTPUT);
  pinMode(PD0, OUTPUT);
  pinMode(PD1, OUTPUT);
  for (uint8_t pin = PE7; pin <= PE15; pin++)
  {
    pinMode(pin, OUTPUT);
  }
  pinMode(PD8, OUTPUT);
  pinMode(PD9, OUTPUT);
  pinMode(PD10, OUTPUT);
}

void GxIO_STM32F103V_P16_TIKY::writeCommandTransaction(uint8_t c)
{
  digitalWrite(_cs, LOW);
  digitalWrite(_rs, LOW);
  writeData16(c);
  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
}

void GxIO_STM32F103V_P16_TIKY::writeDataTransaction(uint8_t d)
{
  digitalWrite(_cs, LOW);
  writeData16(d);
  digitalWrite(_cs, HIGH);
}

void GxIO_STM32F103V_P16_TIKY::writeData16Transaction(uint16_t d, uint32_t num)
{
  digitalWrite(_cs, LOW);
  writeData16(d, num);
  digitalWrite(_cs, HIGH);
}

void GxIO_STM32F103V_P16_TIKY::writeCommand(uint8_t c)
{
  digitalWrite(_rs, LOW);
  writeData16(c);
  digitalWrite(_rs, HIGH);
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
  GPIOD_BASE->BRR = 0b1100011100000011; // Clear bits
  //                 |       |        |
  GPIOE_BASE->BRR = 0b1111111110000000; // Clear bits

  // The compiler efficiently codes this
  // so it is quite quick.                    Port.bit
  if (d & 0x8000) GPIOD_BASE->BSRR = 0x1 << 10; // PD10
  if (d & 0x4000) GPIOD_BASE->BSRR = 0x1 << 9; // PD9
  if (d & 0x2000) GPIOD_BASE->BSRR = 0x1 << 8; // PD8
  if (d & 0x1000) GPIOE_BASE->BSRR = 0x1 << 15; // PE15
  if (d & 0x0800) GPIOE_BASE->BSRR = 0x1 << 14; // PE14
  if (d & 0x0400) GPIOE_BASE->BSRR = 0x1 << 13; // PE13
  if (d & 0x0200) GPIOE_BASE->BSRR = 0x1 << 12; // PE12
  if (d & 0x0100) GPIOE_BASE->BSRR = 0x1 << 11; // PE11

  // so it is quite quick.                    Port.bit
  if (d & 0x0080) GPIOE_BASE->BSRR = 0x1 << 10; // PE10
  if (d & 0x0040) GPIOE_BASE->BSRR = 0x1 << 9; // PE9
  if (d & 0x0020) GPIOE_BASE->BSRR = 0x1 << 8; // PE8
  if (d & 0x0010) GPIOE_BASE->BSRR = 0x1 << 7; // PE7
  if (d & 0x0008) GPIOD_BASE->BSRR = 0x1 << 1; // PD1
  if (d & 0x0004) GPIOD_BASE->BSRR = 0x1 << 0; // PD0
  if (d & 0x0002) GPIOD_BASE->BSRR = 0x1 << 15; // PD15
  if (d & 0x0001) GPIOD_BASE->BSRR = 0x1 << 14; // PD14
  strobes(num);
}

void GxIO_STM32F103V_P16_TIKY::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_STM32F103V_P16_TIKY::startTransaction()
{
  digitalWrite(_cs, LOW);
}

void GxIO_STM32F103V_P16_TIKY::endTransaction()
{
  digitalWrite(_cs, HIGH);
}

void GxIO_STM32F103V_P16_TIKY::strobes(uint32_t num)
{
  while (num > 0)
  {
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
    num--;
  }
}

void GxIO_STM32F103V_P16_TIKY::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif


