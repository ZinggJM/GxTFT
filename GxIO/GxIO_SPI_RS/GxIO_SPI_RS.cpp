// created by Jean-Marc Zingg to be the GxIO_SPI_RS io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GxIO_SPI_RS.h"

#if defined(ESP8266)

GxIO_SPI_RS::GxIO_SPI_RS(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst, int8_t bl) : IOSPI(spi)
{
  _cs   = cs;
  _rst  = rst;
  _bl   = bl;
}

void GxIO_SPI_RS::reset()
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

void GxIO_SPI_RS::init()
{
  if (_cs >= 0)
  {
    digitalWrite(_cs, HIGH);
    pinMode(_cs, OUTPUT);
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
  setFrequency(GxIO_SPI_RS_defaultFrequency);
}

void GxIO_SPI_RS::setFrequency(uint32_t freq)
{
  IOSPI.setFrequency(freq);
}

void GxIO_SPI_RS::setClockDivider(uint32_t clockDiv)
{
  IOSPI.setClockDivider(clockDiv);
}

inline void GxIO_SPI_RS::setDataBits(uint16_t bits)
{
  const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
  bits--;
  SPI1U1 = ((SPI1U1 & mask) | ((bits << SPILMOSI) | (bits << SPILMISO)));
}

uint8_t GxIO_SPI_RS::transfer(uint8_t data, bool rs_data)
{
  while (SPI1CMD & SPIBUSY) {}
  // low byte first, ms bit first!; CMD : dc low -> first bit 0, DATA : dc high -> first bit high
  setDataBits(9);
  SPI1W0 = (data >> 1) | (data << 15) | (rs_data << 7);
  SPI1CMD |= SPIBUSY;
  while (SPI1CMD & SPIBUSY) {}
  //return 0;
  uint16_t rv = SPI1W0;
  return uint8_t(rv & 0xFF);
}

uint16_t GxIO_SPI_RS::transfer16(uint16_t data, bool rs_data)
{
  uint16_t rv = transfer(data >> 8, rs_data);
  rv <<= 8;
  rv |= transfer(data & 0xFF, rs_data);
  return rv;
}

uint8_t GxIO_SPI_RS::transferTransaction(uint8_t d)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint8_t rv = transfer(d, true);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  return rv;
}

uint16_t GxIO_SPI_RS::transfer16Transaction(uint16_t d)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint16_t rv = transfer16(d, true);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  return rv;
}

uint8_t GxIO_SPI_RS::readDataTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint8_t rv = transfer(0xFF, true);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  return rv;
}

uint16_t GxIO_SPI_RS::readData16Transaction()
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint16_t rv = transfer16(0xFFFF, true);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  return rv;
}

uint8_t GxIO_SPI_RS::readData()
{
  return transfer(0xFF, true);
}

uint16_t GxIO_SPI_RS::readData16()
{
  return transfer16(0xFFFF, true);
}

void GxIO_SPI_RS::writeCommandTransaction(uint8_t c)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  transfer(c, false);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
}

void GxIO_SPI_RS::writeDataTransaction(uint8_t d)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  transfer(d, true);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
}

void GxIO_SPI_RS::writeData16Transaction(uint16_t d, uint32_t num)
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
  writeData16(d, num);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
}

void GxIO_SPI_RS::writeCommand(uint8_t c)
{
  transfer(c, false);
}

void GxIO_SPI_RS::writeData(uint8_t d)
{
  transfer(d, true);
}

void GxIO_SPI_RS::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    transfer(*d, true);
    d++;
    num--;
  }
}

void GxIO_SPI_RS::writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    transfer16(d, true);
    num--;
  }
}

void GxIO_SPI_RS::writeAddrMSBfirst(uint16_t d)
{
  transfer(d >> 8, true);
  transfer(d & 0xFF, true);
}

void GxIO_SPI_RS::startTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, LOW);
}

void GxIO_SPI_RS::endTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, HIGH);
}

void GxIO_SPI_RS::selectRegister(bool rs_low)
{
}

void GxIO_SPI_RS::setBackLight(bool lit)
{
  if (_bl >= 0) digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif


