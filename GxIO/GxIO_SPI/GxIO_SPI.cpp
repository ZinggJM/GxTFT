// created by Jean-Marc Zingg to be the GxIO_SPI io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#if defined(__AVR) || defined(ESP8266)

#include "GxIO_SPI.h"

GxIO_SPI::GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst, int8_t bl) : IOSPI(spi)
{
  _cs   = cs;
  _dc   = dc;
  _rst  = rst;
  _bl   = bl;
}

void GxIO_SPI::reset()
{
  if (_rst >= 0)
  {
    delay(20);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(200);
  }
}

void GxIO_SPI::init()
{
  if (_cs >= 0)
  {
    digitalWrite(_cs, HIGH);
    pinMode(_cs, OUTPUT);
  }
  if (_dc >= 0)
  {
    digitalWrite(_dc, HIGH);
    pinMode(_dc, OUTPUT);
  }
  if (_rst >= 0)
  {
    digitalWrite(_rst, HIGH);
    pinMode(_rst, OUTPUT);
  }
  if (_bl >= 0)
  {
    digitalWrite(_bl, HIGH);
    pinMode(_bl, OUTPUT);
  }
  reset();
  IOSPI.begin();
  IOSPI.setDataMode(SPI_MODE0);
  IOSPI.setBitOrder(MSBFIRST);
#if defined(ESP8266)
  IOSPI.setFrequency(GxIO_SPI_defaultFrequency);
#endif
}

void GxIO_SPI::setFrequency(uint32_t freq)
{
  IOSPI.setFrequency(freq);
}
void GxIO_SPI::setClockDivider(uint32_t clockDiv)
{
  IOSPI.setClockDivider(clockDiv);
}

uint8_t GxIO_SPI::transferTransaction(uint8_t d)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint8_t rv = IOSPI.transfer(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  return rv;
}

uint16_t GxIO_SPI::transfer16Transaction(uint16_t d)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint16_t rv = IOSPI.transfer16(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  return rv;
}

uint8_t GxIO_SPI::readDataTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint8_t rv = IOSPI.transfer(0xFF);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  return rv;
}

uint16_t GxIO_SPI::readData16Transaction()
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint16_t rv = IOSPI.transfer16(0xFFFF);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  return rv;
}

uint8_t GxIO_SPI::readData()
{
  return IOSPI.transfer(0xFF);
}

uint16_t GxIO_SPI::readData16()
{
  return IOSPI.transfer16(0xFFFF);
}

void GxIO_SPI::writeCommandTransaction(uint8_t c)
{
  if (_dc >= 0) digitalWrite(_dc, LOW);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  IOSPI.transfer(c);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
}

void GxIO_SPI::writeDataTransaction(uint8_t d)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  IOSPI.transfer(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
}

void GxIO_SPI::writeData16Transaction(uint16_t d, uint32_t num)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  writeData16(d, num);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
}

void GxIO_SPI::writeCommand(uint8_t c)
{
  if (_dc >= 0) digitalWrite(_dc, LOW);
  IOSPI.transfer(c);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
}

void GxIO_SPI::writeData(uint8_t d)
{
  IOSPI.transfer(d);
}

void GxIO_SPI::writeData(uint8_t* d, uint32_t num)
{
#if defined(ESP8266)
  IOSPI.writeBytes(d, num);
#else
  while (num > 0)
  {
    IOSPI.transfer(*d);
    d++;
    num--;
  }
#endif
}

void GxIO_SPI::writeData16(uint16_t d, uint32_t num)
{
#if defined(ESP8266)
  uint8_t b[2] = {d >> 8 , d};
  IOSPI.writePattern(b, 2, num);
#else
  while (num > 0)
  {
    IOSPI.transfer16(d);
    num--;
  }
#endif
}

void GxIO_SPI::writeAddrMSBfirst(uint16_t d)
{
  IOSPI.transfer(d >> 8);
  IOSPI.transfer(d & 0xFF);
}

void GxIO_SPI::startTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
}

void GxIO_SPI::endTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, HIGH);
}

void GxIO_SPI::setBackLight(bool lit)
{
  if (_bl >= 0) digitalWrite(_bl, (lit ? HIGH : LOW));
}

#if defined(SPI_HAS_TRANSACTION)

GxIO_SPI_USING_TRANSACTION::GxIO_SPI_USING_TRANSACTION(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst, int8_t bl) : IOSPI(spi)
{
  _cs   = cs;
  _dc   = dc;
  _rst  = rst;
  _bl   = bl;
}

void GxIO_SPI_USING_TRANSACTION::reset()
{
  if (_rst >= 0)
  {
    delay(20);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(200);
  }
}

void GxIO_SPI_USING_TRANSACTION::init()
{
  if (_cs >= 0)
  {
    digitalWrite(_cs, HIGH);
    pinMode(_cs, OUTPUT);
  }
  if (_dc >= 0)
  {
    digitalWrite(_dc, HIGH);
    pinMode(_dc, OUTPUT);
  }
  if (_rst >= 0)
  {
    digitalWrite(_rst, HIGH);
    pinMode(_rst, OUTPUT);
  }
  if (_bl >= 0)
  {
    digitalWrite(_bl, HIGH);
    pinMode(_bl, OUTPUT);
  }
  reset();
  IOSPI.begin();
}

void GxIO_SPI_USING_TRANSACTION::setFrequency(uint32_t freq)
{
  // IOSPI.setFrequency(freq); // useless
  settings._clock = freq;
}

void GxIO_SPI_USING_TRANSACTION::setClockDivider(uint32_t clockDiv)
{
  // IOSPI.setClockDivider(clockDiv); // useless
}

uint8_t GxIO_SPI_USING_TRANSACTION::transferTransaction(uint8_t d)
{
  IOSPI.beginTransaction(settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint8_t rv = IOSPI.transfer(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  IOSPI.endTransaction();
  return rv;
}

uint16_t GxIO_SPI_USING_TRANSACTION::transfer16Transaction(uint16_t d)
{
  IOSPI.beginTransaction(settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint16_t rv = IOSPI.transfer16(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  IOSPI.endTransaction();
  return rv;
}

uint8_t GxIO_SPI_USING_TRANSACTION::readDataTransaction()
{
  IOSPI.beginTransaction(settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint8_t rv = IOSPI.transfer(0xFF);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  IOSPI.endTransaction();
  return rv;
}

uint16_t GxIO_SPI_USING_TRANSACTION::readData16Transaction()
{
  IOSPI.beginTransaction(settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint16_t rv = IOSPI.transfer16(0xFFFF);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  IOSPI.endTransaction();
  return rv;
}

uint8_t GxIO_SPI_USING_TRANSACTION::readData()
{
  return IOSPI.transfer(0xFF);
}

uint16_t GxIO_SPI_USING_TRANSACTION::readData16()
{
  return IOSPI.transfer16(0xFFFF);
}

void GxIO_SPI_USING_TRANSACTION::writeCommandTransaction(uint8_t c)
{
  IOSPI.beginTransaction(settings);
  if (_dc >= 0) digitalWrite(_dc, LOW);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  IOSPI.transfer(c);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
  IOSPI.endTransaction();
}

void GxIO_SPI_USING_TRANSACTION::writeDataTransaction(uint8_t d)
{
  IOSPI.beginTransaction(settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  IOSPI.transfer(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  IOSPI.endTransaction();
}

void GxIO_SPI_USING_TRANSACTION::writeData16Transaction(uint16_t d, uint32_t num)
{
  IOSPI.beginTransaction(settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  writeData16(d, num);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  IOSPI.endTransaction();
}

void GxIO_SPI_USING_TRANSACTION::writeCommand(uint8_t c)
{
  if (_dc >= 0) digitalWrite(_dc, LOW);
  IOSPI.transfer(c);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
}

void GxIO_SPI_USING_TRANSACTION::writeData(uint8_t d)
{
  IOSPI.transfer(d);
}

void GxIO_SPI_USING_TRANSACTION::writeData(uint8_t* d, uint32_t num)
{
#if defined(ESP8266)
  IOSPI.writeBytes(d, num);
#else
  while (num > 0)
  {
    IOSPI.transfer(*d);
    d++;
    num--;
  }
#endif
}

void GxIO_SPI_USING_TRANSACTION::writeData16(uint16_t d, uint32_t num)
{
#if defined(ESP8266)
  uint8_t b[2] = {d >> 8 , d};
  IOSPI.writePattern(b, 2, num);
#else
  while (num > 0)
  {
    IOSPI.transfer16(d);
    num--;
  }
#endif
}

void GxIO_SPI_USING_TRANSACTION::writeAddrMSBfirst(uint16_t d)
{
  IOSPI.transfer(d >> 8);
  IOSPI.transfer(d & 0xFF);
}

void GxIO_SPI_USING_TRANSACTION::startTransaction()
{
  IOSPI.beginTransaction(settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
}

void GxIO_SPI_USING_TRANSACTION::endTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  IOSPI.endTransaction();
}

void GxIO_SPI_USING_TRANSACTION::setBackLight(bool lit)
{
  if (_bl >= 0) digitalWrite(_bl, (lit ? HIGH : LOW));
}

#else
#endif

#endif


