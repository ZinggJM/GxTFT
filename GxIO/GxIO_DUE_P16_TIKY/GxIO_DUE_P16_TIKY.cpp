// created by Jean-Marc Zingg to be the GxIO_DUE_P16_TIKY io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html
//
// pin mapping description not yet done
//

#if defined(ARDUINO_ARCH_SAM)

#include "GxIO_DUE_P16_TIKY.h"

GxIO_DUE_P16_TIKY::GxIO_DUE_P16_TIKY()
{
  _cs   = 22;
  _rs   = 23;
  _rst  = 26;
  _wr   = 24;
  _rd   = 25;
  _bl   = 49;
}

void GxIO_DUE_P16_TIKY::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GxIO_DUE_P16_TIKY::init()
{
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, HIGH);

  pinMode(_rs, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_wr, OUTPUT);
  pinMode(_rd, OUTPUT);
  pinMode(_bl, OUTPUT);

  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_wr, HIGH);
  digitalWrite(_rd, HIGH);
  digitalWrite(_bl, LOW);

  reset();

  // Set 16 bit port pins to output on Due
  for (uint8_t pin = 27; pin <=42; pin++)
  {
    pinMode(pin, OUTPUT);
  }
}

void GxIO_DUE_P16_TIKY::writeCommandTransaction(uint8_t c)
{
  digitalWrite(_cs, LOW);
  digitalWrite(_rs, LOW);
  writeData16(c);
  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
}

void GxIO_DUE_P16_TIKY::writeDataTransaction(uint8_t d)
{
  digitalWrite(_cs, LOW);
  writeData16(d);
  digitalWrite(_cs, HIGH);
}

void GxIO_DUE_P16_TIKY::writeData16Transaction(uint16_t d, uint32_t num)
{
  digitalWrite(_cs, LOW);
  writeData16(d, num);
  digitalWrite(_cs, HIGH);
}

void GxIO_DUE_P16_TIKY::writeCommand(uint8_t c)
{
  digitalWrite(_rs, LOW);
  writeData16(c);
  digitalWrite(_rs, HIGH);
}

void GxIO_DUE_P16_TIKY::writeData(uint8_t d)
{
  writeData16(d);
}

void GxIO_DUE_P16_TIKY::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void GxIO_DUE_P16_TIKY::writeData16(uint16_t d, uint32_t num)
{
  //                |       |       |       |       ; // Ruler for byte MS bits 31, 23, 15 and 7
  REG_PIOA_CODR = 0b00000000000110000000000010000000; // Clear bits A7,A19,A20
  //                |       |       |       |       ;  
  REG_PIOB_CODR = 0b00000000000000000000000000000000; // Clear bits
  //                |       |       |       |       ;  
  REG_PIOC_CODR = 0b00000000000000000000001111111110; // Clear bits C1..C9
  //                |       |       |       |       ;  
  REG_PIOD_CODR = 0b00000000000000000000011001001100; // Clear bits D2,D3,D6,D9,D10
  //                |       |       |       |         
  // The compiler efficiently codes this
  // so it is quite quick.                    Port.bit
  if (d & 0x8000) REG_PIOA_SODR = 0x1 << 19; // A19
  if (d & 0x4000) REG_PIOC_SODR = 0x1 << 9; // C9
  if (d & 0x2000) REG_PIOC_SODR = 0x1 << 8; // C8
  if (d & 0x1000) REG_PIOC_SODR = 0x1 << 7; // C7
  if (d & 0x0800) REG_PIOC_SODR = 0x1 << 6; // C6
  if (d & 0x0400) REG_PIOC_SODR = 0x1 << 5; // C5
  if (d & 0x0200) REG_PIOC_SODR = 0x1 << 4; // C4
  if (d & 0x0100) REG_PIOC_SODR = 0x1 << 3; // C3

  // so it is quite quick.                    Port.bit
  if (d & 0x0080) REG_PIOC_SODR = 0x1 << 2; // C2
  if (d & 0x0040) REG_PIOC_SODR = 0x1 << 1; // C1
  if (d & 0x0020) REG_PIOD_SODR = 0x1 << 10; // D10
  if (d & 0x0010) REG_PIOA_SODR = 0x1 << 7; // A7
  if (d & 0x0008) REG_PIOD_SODR = 0x1 << 9; // D9
  if (d & 0x0004) REG_PIOD_SODR = 0x1 << 6; // D6
  if (d & 0x0002) REG_PIOD_SODR = 0x1 << 3; // D3
  if (d & 0x0001) REG_PIOD_SODR = 0x1 << 2; // D2
  strobes(num);
}

void GxIO_DUE_P16_TIKY::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_DUE_P16_TIKY::startTransaction()
{
  digitalWrite(_cs, LOW);
}

void GxIO_DUE_P16_TIKY::endTransaction()
{
  digitalWrite(_cs, HIGH);
}

void GxIO_DUE_P16_TIKY::strobes(uint32_t num)
{
  while (num)
  {
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
    num--;
  }
}

void GxIO_DUE_P16_TIKY::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif

