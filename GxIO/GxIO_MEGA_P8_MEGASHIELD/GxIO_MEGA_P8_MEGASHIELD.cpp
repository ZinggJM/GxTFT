// created by Jean-Marc Zingg to be the GxIO_MEGA_P8_MEGASHIELD io class for the GxTFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// for 16 bit parallel displays on shields or on adapter shields for MEGA/DUE, e.g. HVGA MEGA or MEGA Shield V2.2

// read functions untested, waiting for CTE TFT LCD/SD Shield for MEGA to arrive

#if defined(__AVR_ATmega2560__)

#include "GxIO_MEGA_P8_MEGASHIELD.h"

GxIO_MEGA_P8_MEGASHIELD::GxIO_MEGA_P8_MEGASHIELD()
{
//  _cs   = 40; //PORT G bit _BV(1)
//  _rs   = 38; //PORT D bit _BV(7)
//  _rst  = 41; //PORT G bit _BV(0)
//  _wr   = 39; //PORT G bit _BV(2)
//  _rd   = 43; //PORT L bit _BV(6)

// INHAOS LCD-2000-9225 on INHAOS MEGA shield (info is wrong)
  _cs   = 40; //PORT G bit _BV(1)
  _rs   = 39; //PORT D bit _BV(2)
  _rst  = 41; //PORT G bit _BV(0)
  _wr   = 38; //PORT G bit _BV(7)
  _rd   = 43; //PORT L bit _BV(6)
}

//#define RS_L PORTD &= ~_BV(7);
//#define RS_H PORTD |= _BV(7);
//#define WR_L PORTG &= ~_BV(2);
//#define WR_H PORTG |= _BV(2);
//#define WR_STB PORTG &= ~_BV(2); PORTG &= ~_BV(2); PORTG |= _BV(2);

// INHAOS LCD-2000-9225 on INHAOS MEGA shield (info is wrong)
#define WR_L PORTD &= ~_BV(7);
#define WR_H PORTD |= _BV(7);
#define RS_L PORTG &= ~_BV(2);
#define RS_H PORTG |= _BV(2);
#define WR_STB WR_L WR_L WR_H

void GxIO_MEGA_P8_MEGASHIELD::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GxIO_MEGA_P8_MEGASHIELD::init()
{
  digitalWrite(_rst, HIGH);
  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_wr, HIGH);
  digitalWrite(_rd, HIGH);
  pinMode(_rst, OUTPUT);
  pinMode(_rs, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_wr, OUTPUT);
  pinMode(_rd, OUTPUT);
  reset();
  DDRC = 0xFF; // Set direction for the 8 bit data port
}

uint8_t GxIO_MEGA_P8_MEGASHIELD::readDataTransaction()
{
  DDRC = 0x00; // Set direction input
  PORTG &= ~_BV(1); // CS_L;
  PORTL |= _BV(6); // RD_H
  PORTL &= ~_BV(6); // RD_L
  PORTL &= ~_BV(6); // RD_L
  PORTL &= ~_BV(6); // RD_L
  uint8_t rv = PINC;
  PORTL |= _BV(6); // RD_H
  PORTG |= _BV(1); // CS_H;
  DDRC = 0xFF; // Set direction output again
  return rv;
}

uint16_t GxIO_MEGA_P8_MEGASHIELD::readData16Transaction()
{
  uint16_t rv = readDataTransaction() << 8;
  rv |= readDataTransaction();
  return rv;
}

uint8_t GxIO_MEGA_P8_MEGASHIELD::readData()
{
  DDRC = 0x00; // Set direction input
  PORTL |= _BV(6); // RD_H
  PORTL &= ~_BV(6); // RD_L
  PORTL &= ~_BV(6); // RD_L
  PORTL &= ~_BV(6); // RD_L
  uint8_t rv = PINC;
  PORTL |= _BV(6); // RD_H
  DDRC = 0xFF; // Set direction output again
  return rv;
}

uint16_t GxIO_MEGA_P8_MEGASHIELD::readData16()
{
  uint16_t rv = readData() << 8;
  rv |= readData();
  return rv;
}

void GxIO_MEGA_P8_MEGASHIELD::writeCommandTransaction(uint8_t c)
{
  PORTG &= ~_BV(1); // CS_L;
  RS_L;
  PORTC = c;
  WR_STB;
  RS_H;
  PORTG |= _BV(1); // CS_H;
}

void GxIO_MEGA_P8_MEGASHIELD::writeDataTransaction(uint8_t d)
{
  PORTG &= ~_BV(1); // CS_L;
  PORTC = d;
  WR_STB;
  PORTG |= _BV(1); // CS_H;
}

void GxIO_MEGA_P8_MEGASHIELD::writeData16Transaction(uint16_t d, uint32_t num)
{
  PORTG &= ~_BV(1); // CS_L;
  writeData16(d, num);
  PORTG |= _BV(1); // CS_H;
}

void GxIO_MEGA_P8_MEGASHIELD::writeCommand(uint8_t c)
{
  RS_L;
  PORTC = c;
  WR_STB;
  RS_H;
}

void GxIO_MEGA_P8_MEGASHIELD::writeData(uint8_t d)
{
  PORTC = d;
  WR_STB;
}

void GxIO_MEGA_P8_MEGASHIELD::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData(*d);
    d++;
    num--;
  }
}

void GxIO_MEGA_P8_MEGASHIELD::writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    writeData(d >> 8);
    writeData(d);
    num--;
  }
}

void GxIO_MEGA_P8_MEGASHIELD::writeAddrMSBfirst(uint16_t d)
{
  PORTC = d >> 8;
  WR_STB;
  PORTC = d;
  WR_STB;
}

void GxIO_MEGA_P8_MEGASHIELD::startTransaction()
{
  PORTG &= ~_BV(1);
}

void GxIO_MEGA_P8_MEGASHIELD::endTransaction()
{
  PORTG |= _BV(1);
}

void GxIO_MEGA_P8_MEGASHIELD::selectRegister(bool rs_low)
{
  digitalWrite(_rs, (rs_low ? LOW : HIGH));
}

void GxIO_MEGA_P8_MEGASHIELD::setBackLight(bool lit)
{
  if (_bl >= 0) digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif

