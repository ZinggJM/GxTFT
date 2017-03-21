// created by Jean-Marc Zingg to be the GxIO_STM32F103C8T6_P16_TIKY io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html

#if defined(ARDUINO_ARCH_STM32F1)

#include "GxIO_STM32F103C8T6_P16_TIKY.h"

// reserved & unusable pins
// PA9  : BOARD_USART1_TX_PIN -> to ESP8266
// PA10 : BOARD_USART1_RX_PIN <- from ESP8266
// PA11 : USBDM : USB serial connection
// PA12 : USBDP : USB serial connection
// PA13 : SWDIO
// PA14 : SWCLK
// PA15 : JTDI, remap needed for use as GPIO
// PB2  : BOOT1, no pin
// PB3  : BOARD_JTDO_PIN, remap needed for use as GPIO
// PB4  : BOARD_JTDI_PIN, remap needed for use as GPIO
// PB8  : usable ?

// connector pins
// 11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40
// CS  RS  RW  RD  RST D0  D1  D2  D3  D4  D5  D6  D7  D8  D9  D10 D11 D12 D13 D14 D15 BSY TCS SCK MIS MOS INT BL  GND 3.3
// B11 B12 B10 B13 B1  B14 B0  B15 A7  A8  A6  A15 A5  B3  A4  B4  A3  B5  A2  B6  A1                          B7

// A used : 15,..,..,..,..,..,.., 8, 7, 6, 5, 4, 3, 2, 1,.., // 9
// A data : 15,..,..,..,..,..,.., 8, 7, 6, 5, 4, 3, 2, 1,.., // 9
//         |           |           |           |           |
// B used : 15,14,13,12,11,10,..,.., 7, 6, 5, 4, 3,.., 1, 0, // 13
// B data : 15,14,..,..,..,..,..,..,.., 6, 5, 4, 3,..,.., 0, // 6

#define PA_USED_BITS 0x81FE
#define PA_DATA_BITS 0x81FE

#define PB_USED_BITS 0xFCFB
#define PB_DATA_BITS 0xC079

#define PA_CRH_MASK 0xF000000F // all pin bits used 15..8
#define PA_CRH_CNMD 0x30000003 // 00 11 output PP, 50MHz
#define PA_CRL_MASK 0xFFFFFFF0 // all pin bits used 7..0
#define PA_CRL_CNMD 0x33333330 // 00 11 output PP, 50MHz

#define PB_CRH_MASK 0xFF000000 // all pin bits used 15..8
#define PB_CRH_CNMD 0x33000000 // 00 11 output PP, 50MHz
#define PB_CRL_MASK 0x0FFFF00F // all pin bits used 7..0
#define PB_CRL_CNMD 0x03333003 // 00 11 output PP, 50MHz

//#define PA_CRH_MASK 0xF000000F // data bits used 15..8
//#define PA_CRH_CNMD 0x30000003 // 00 11 output PP, 50MHz
//#define PA_CRL_MASK 0xFFFFFFF0 // data bits used 7..0
//#define PA_CRL_CNMD 0x33333330 // 00 11 output PP, 50MHz
//
//#define PB_CRH_MASK 0xFF000000 // data bits used 15..8
//#define PB_CRH_CNMD 0x33000000 // 00 11 output PP, 50MHz
//#define PB_CRL_MASK 0x0FFFF00F // data bits used 7..0
//#define PB_CRL_CNMD 0x03333003 // 00 11 output PP, 50MHz

GxIO_STM32F103C8T6_P16_TIKY::GxIO_STM32F103C8T6_P16_TIKY()
{
  _cs   = PB11;
  _rs   = PB12;
  _wr   = PB10;
  _rd   = PB13;
  _rst  = PB1;
  _bl   = PB7;
}

void GxIO_STM32F103C8T6_P16_TIKY::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GxIO_STM32F103C8T6_P16_TIKY::init()
{
  uint32_t mapr = AFIO_BASE->MAPR;
  mapr &= ~AFIO_MAPR_SWJ_CFG;
  mapr |= AFIO_MAPR_SWJ_CFG_NO_JTAG_SW;
  AFIO_BASE->MAPR = mapr; // remap JTAG pins as GPIO

  digitalWrite(_cs, HIGH);
  digitalWrite(_rs, HIGH);
  digitalWrite(_wr, HIGH);
  digitalWrite(_rd, HIGH);
  digitalWrite(_rst, HIGH);
  digitalWrite(_bl, LOW);

  pinMode(_cs, OUTPUT);
  pinMode(_rs, OUTPUT);
  pinMode(_wr, OUTPUT);
  pinMode(_rd, OUTPUT);
  pinMode(_rst, OUTPUT);
  pinMode(_bl, OUTPUT);


  reset();

  GPIOA_BASE->BRR = PA_DATA_BITS; // Clear bits
  GPIOB_BASE->BRR = PB_DATA_BITS; // Clear bits
  pinMode(PB14, OUTPUT);
  pinMode(PB0, OUTPUT);
  pinMode(PB15, OUTPUT);
  for (uint8_t pin = PA1; pin <= PA8; pin++)
  {
    pinMode(pin, OUTPUT);
  }
  pinMode(PA15, OUTPUT);
  pinMode(PB0, OUTPUT);
  pinMode(PB3, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(PB5, OUTPUT);
  pinMode(PB6, OUTPUT);

  GPIOA_BASE->CRH &= ~PA_CRH_MASK;
  GPIOA_BASE->CRH |= PA_CRH_CNMD;
  GPIOA_BASE->CRL &= ~PA_CRL_MASK;
  GPIOA_BASE->CRL |= PA_CRL_CNMD;

  GPIOB_BASE->CRH &= ~PB_CRH_MASK;
  GPIOB_BASE->CRH |= PB_CRH_CNMD;
  GPIOB_BASE->CRL &= ~PB_CRL_MASK;
  GPIOB_BASE->CRL |= PB_CRL_CNMD;
}

void GxIO_STM32F103C8T6_P16_TIKY::writeCommandTransaction(uint8_t c)
{
  digitalWrite(_cs, LOW);
  digitalWrite(_rs, LOW);
  writeData16(c);
  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
}

void GxIO_STM32F103C8T6_P16_TIKY::writeDataTransaction(uint8_t d)
{
  digitalWrite(_cs, LOW);
  writeData16(d);
  digitalWrite(_cs, HIGH);
}

void GxIO_STM32F103C8T6_P16_TIKY::writeData16Transaction(uint16_t d, uint32_t num)
{
  digitalWrite(_cs, LOW);
  writeData16(d, num);
  digitalWrite(_cs, HIGH);
}

void GxIO_STM32F103C8T6_P16_TIKY::writeCommand(uint8_t c)
{
  digitalWrite(_rs, LOW);
  writeData16(c);
  digitalWrite(_rs, HIGH);
}

void GxIO_STM32F103C8T6_P16_TIKY::writeData(uint8_t d)
{
  writeData16(d);
}

void GxIO_STM32F103C8T6_P16_TIKY::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void GxIO_STM32F103C8T6_P16_TIKY::writeData16(uint16_t d, uint32_t num)
{
  GPIOA_BASE->BRR = PA_DATA_BITS; // Clear bits
  GPIOB_BASE->BRR = PB_DATA_BITS; // Clear bits

  // The compiler efficiently codes this
  // so it is quite quick.                    Port.bit
  if (d & 0x8000) GPIOA_BASE->BSRR = 0x1 << 1; // PA1
  if (d & 0x4000) GPIOB_BASE->BSRR = 0x1 << 6; // PB6
  if (d & 0x2000) GPIOA_BASE->BSRR = 0x1 << 2; // PA2
  if (d & 0x1000) GPIOB_BASE->BSRR = 0x1 << 5; // PB5
  if (d & 0x0800) GPIOA_BASE->BSRR = 0x1 << 3; // PA3
  if (d & 0x0400) GPIOB_BASE->BSRR = 0x1 << 4; // PB4
  if (d & 0x0200) GPIOA_BASE->BSRR = 0x1 << 4; // PA4
  if (d & 0x0100) GPIOB_BASE->BSRR = 0x1 << 3; // PB3

  // so it is quite quick.                    Port.bit
  if (d & 0x0080) GPIOA_BASE->BSRR = 0x1 << 5; // PA5
  if (d & 0x0040) GPIOA_BASE->BSRR = 0x1 << 15; // PA15
  if (d & 0x0020) GPIOA_BASE->BSRR = 0x1 << 6; // PA6
  if (d & 0x0010) GPIOA_BASE->BSRR = 0x1 << 8; // PA8
  if (d & 0x0008) GPIOA_BASE->BSRR = 0x1 << 7; // PA7
  if (d & 0x0004) GPIOB_BASE->BSRR = 0x1 << 15; // PB15
  if (d & 0x0002) GPIOB_BASE->BSRR = 0x1 << 0; // PB0
  if (d & 0x0001) GPIOB_BASE->BSRR = 0x1 << 14; // PB14
  strobes(num);
}

void GxIO_STM32F103C8T6_P16_TIKY::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_STM32F103C8T6_P16_TIKY::startTransaction()
{
  digitalWrite(_cs, LOW);
}

void GxIO_STM32F103C8T6_P16_TIKY::endTransaction()
{
  digitalWrite(_cs, HIGH);
}

void GxIO_STM32F103C8T6_P16_TIKY::strobes(uint32_t num)
{
  while (num > 0)
  {
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
    num--;
  }
}

void GxIO_STM32F103C8T6_P16_TIKY::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit ? HIGH : LOW));
}

#endif

