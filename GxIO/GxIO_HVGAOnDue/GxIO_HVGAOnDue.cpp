// created by Jean-Marc Zingg to be the GxIO_HVGAOnMEGA io class for the GxTFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357_Due
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#if defined(ARDUINO_ARCH_SAM)

#include "GxIO_HVGAOnDue.h"

GxIO_HVGAOnDue::GxIO_HVGAOnDue()
{
  _cs   = 40;
  _rs   = 38;
  _rst  = 41;
  _wr   = 39;
}

void GxIO_HVGAOnDue::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GxIO_HVGAOnDue::init()
{
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, HIGH);

  pinMode(_rs, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_wr, OUTPUT);

  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_wr, HIGH);

  reset();

  // Set 16 bit port pins to output on Due
  pinMode(29, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
}

void GxIO_HVGAOnDue::writeCommandTransaction(uint8_t c)
{
  digitalWrite(_cs, LOW);
  digitalWrite(_rs, LOW);
  writeData16(c);
  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
}

void GxIO_HVGAOnDue::writeDataTransaction(uint8_t d)
{
  digitalWrite(_cs, LOW);
  writeData16(d);
  digitalWrite(_cs, HIGH);
}

void GxIO_HVGAOnDue::writeData16Transaction(uint16_t d, uint32_t num)
{
  digitalWrite(_cs, LOW);
  writeData16(d, num);
  digitalWrite(_cs, HIGH);
}

void GxIO_HVGAOnDue::writeCommand(uint8_t c)
{
  digitalWrite(_rs, LOW);
  writeData16(c);
  digitalWrite(_rs, HIGH);
}

void GxIO_HVGAOnDue::writeData(uint8_t d)
{
  writeData16(d);
}

void GxIO_HVGAOnDue::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void GxIO_HVGAOnDue::writeData16(uint16_t d, uint32_t num)
{
  //                |       |       |       |         Ruler for byte MS bits 31, 23, 15 and 7
  //                     B          AA   DD AD  DDDD  Marker for register bits used
  REG_PIOA_CODR = 0b00000000000000001100000010000000; // Clear bits
  REG_PIOB_CODR = 0b00000100000000000000000000000000; // Clear bits
  //                                        W CCCCC   // WR bit
  REG_PIOC_CODR = 0b00000000000000000000000010111110; // Clear WR bit as well
  REG_PIOD_CODR = 0b00000000000000000000011001001111; // Clear bits

  // The compiler efficiently codes this
  // so it is quite quick.                    Port.bit
  if (d & 0x8000) REG_PIOD_SODR = 0x1 << 6; // D.6
  if (d & 0x4000) REG_PIOD_SODR = 0x1 << 3; // D.3
  if (d & 0x2000) REG_PIOD_SODR = 0x1 << 2; // D.2
  if (d & 0x1000) REG_PIOD_SODR = 0x1 << 1; // D.1
  if (d & 0x0800) REG_PIOD_SODR = 0x1 << 0; // D.0
  if (d & 0x0400) REG_PIOA_SODR = 0x1 << 15; // A.15
  if (d & 0x0200) REG_PIOA_SODR = 0x1 << 14; // A.14
  if (d & 0x0100) REG_PIOB_SODR = 0x1 << 26; // B.26

  // so it is quite quick.                    Port.bit
  if (d & 0x0080) REG_PIOD_SODR = 0x1 << 9; // D.9
  if (d & 0x0040) REG_PIOA_SODR = 0x1 << 7; // A.7
  if (d & 0x0020) REG_PIOD_SODR = 0x1 << 10; // D.10
  if (d & 0x0010) REG_PIOC_SODR = 0x1 << 1; // C.1
  if (d & 0x0008) REG_PIOC_SODR = 0x1 << 2; // C.2
  if (d & 0x0004) REG_PIOC_SODR = 0x1 << 3; // C.3
  if (d & 0x0002) REG_PIOC_SODR = 0x1 << 4; // C.4
  if (d & 0x0001) REG_PIOC_SODR = 0x1 << 5; // C.5
  strobes(num);
}

void GxIO_HVGAOnDue::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_HVGAOnDue::startTransaction()
{
  digitalWrite(_cs, LOW);
}

void GxIO_HVGAOnDue::endTransaction()
{
  digitalWrite(_cs, HIGH);
}

void GxIO_HVGAOnDue::strobes(uint32_t num)
{
  while (num)
  {
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
    num--;
  }
}

void GxIO_HVGAOnDue::setBackLight(bool lit)
{
  if (_bl >= 0) digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif

