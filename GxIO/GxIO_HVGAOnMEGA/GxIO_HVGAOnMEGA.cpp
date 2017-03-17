// created by Jean-Marc Zingg to be the GxIO_HVGAOnMEGA io class for the GxTFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#if defined(__AVR_ATmega2560__)

#include "GxIO_HVGAOnMEGA.h"

GxIO_HVGAOnMEGA::GxIO_HVGAOnMEGA()
{
  _cs   = 40; //PORT G bit _BV(1)
  _rs   = 38; //PORT D bit _BV(7)
  _rst  = 41; //PORT G bit _BV(0)
  _wr   = 39; //PORT G bit _BV(2)
}

void GxIO_HVGAOnMEGA::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GxIO_HVGAOnMEGA::init()
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

  DDRA = 0xFF; // Set direction for the 2 8 bit data ports
  DDRC = 0xFF;
}

void GxIO_HVGAOnMEGA::writeCommandTransaction(uint8_t c)
{
  PORTG &= ~_BV(1); // SETUP_CS_L;
  PORTD &= ~_BV(7); // RS_L;
  PORTA = 0;
  PORTC = c;
  PORTG &= ~_BV(2); PORTG &= ~_BV(2); PORTG |= _BV(2); // WR_STB;
  PORTD |= _BV(7); // RS_H;
  PORTG |= _BV(1); // SETUP_CS_H;
}

void GxIO_HVGAOnMEGA::writeDataTransaction(uint8_t c)
{
  PORTG &= ~_BV(1); // CS_L;
  PORTA = c >> 8;
  PORTC = c;
  PORTG &= ~_BV(2); PORTG &= ~_BV(2); PORTG |= _BV(2); // WR_STB;
  PORTG |= _BV(1); // CS_H;
}

void GxIO_HVGAOnMEGA::writeData16Transaction(uint16_t d, uint32_t num)
{
  digitalWrite(_cs, LOW);
  writeData16(d, num);
  digitalWrite(_cs, HIGH);
}

void GxIO_HVGAOnMEGA::writeCommand(uint8_t c)
{
  PORTD &= ~_BV(7); // RS_L;
  PORTA = 0;
  PORTC = c;
  PORTG &= ~_BV(2); PORTG &= ~_BV(2); PORTG |= _BV(2); // WR_STB;
  PORTD |= _BV(7); // RS_H;
}

void GxIO_HVGAOnMEGA::writeData(uint8_t c)
{
  PORTA = c >> 8;
  PORTC = c;
  PORTG &= ~_BV(2); PORTG &= ~_BV(2); PORTG |= _BV(2); // WR_STB;
}

void GxIO_HVGAOnMEGA::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData(*d);
    d++;
    num--;
  }
}

void GxIO_HVGAOnMEGA::writeData16(uint16_t d, uint32_t num)
{
  PORTA = d >> 8;
  PORTC = d;
  strobes(num);
}

void GxIO_HVGAOnMEGA::writeAddrMSBfirst(uint16_t c)
{
  PORTC = c >> 8;
  PORTG &= ~_BV(2); PORTG &= ~_BV(2); PORTG |= _BV(2); // WR_STB;
  PORTC = c;
  PORTG &= ~_BV(2); PORTG &= ~_BV(2); PORTG |= _BV(2); // WR_STB;
}

void GxIO_HVGAOnMEGA::startTransaction()
{
  PORTG &= ~_BV(1);
}
void GxIO_HVGAOnMEGA::endTransaction()
{
  PORTG |= _BV(1);
}

void GxIO_HVGAOnMEGA::strobes(uint32_t num)
{
  while (num)
  {
    PORTG &= ~_BV(2); PORTG &= ~_BV(2); PORTG |= _BV(2);
    num--;
  }
}

void GxIO_HVGAOnMEGA::setBackLight(bool lit)
{
  if (_bl >= 0) digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif

