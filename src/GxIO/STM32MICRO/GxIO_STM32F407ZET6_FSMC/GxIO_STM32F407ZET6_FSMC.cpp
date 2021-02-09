// created by Jean-Marc Zingg to be the GxIO_STM32F407ZET6_FSMC io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for the black STM32F407ZET6 board with FMSC TFT connector, e.g.
// https://www.aliexpress.com/item/32689262341.html
// for details of the board see https://github.com/mcauser/BLACK_F407ZE
//
// e.g. for display with matching connector
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F407V doc.
//
// this io class can be used with or adapted to other STM32F407V/Z processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), 
// board "Generic STM32F4 series" part "BLACK F407VE", or "BLACK F407VG", or "BLACK F407ZE", or "BLACK F407ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F407xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_BLACK_F407ZE) || defined(ARDUINO_BLACK_F407ZG)

#include "GxIO_STM32F407ZET6_FSMC.h"

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

//                    |       |        | // Ruler on 8 & 16
#define PD_DATA_BITS 0b1100011100000011
#define PD_CTRL_BITS 0b0000000000110000  // PD5(FSMC_NWE),PD4(FSMC_NOE)
#define PD_FSMC_BITS 0b1100011100110011
#define PD_AFR0_MASK         0x00FF00FF
#define PD_AFR0_FSMC         0x00CC00CC
#define PD_AFR1_MASK 0xFF000FFF
#define PD_AFR1_FSMC 0xCC000CCC
//                    |       |        |
// PD_MODE_MASK      0q3300033300330033 // quaternary notation, 2bit per digit
// PD_MODE_FSMC      0q2200022200220022 //
// PD_OSPD_FSMC      0q1100011100110011 //
#define PD_MODE_MASK 0xF03F0F0F // all FMSC MODE bits, 0q'33'00'03'33'00'33'00'33
#define PD_MODE_FSMC 0xA02A0A0A // FMSC MODE values 10 alternate function
#define PD_OSPD_FSMC 0x50150505 // FMSC OSPEED values 01 10MHz
//                    |       |        |
#define PE_DATA_BITS 0b1111111110000000
#define PE_AFR0_MASK         0xF0000000
#define PE_AFR0_FSMC         0xC0000000
#define PE_AFR1_MASK 0xFFFFFFFF
#define PE_AFR1_FSMC 0xCCCCCCCC
//                    |       |        |
#define PE_MODE_MASK 0xFFFFC000 // all FMSC MODE bits
#define PE_MODE_FSMC 0xAAAA8000 // FMSC MODE values 10 alternate function
#define PE_OSPD_FSMC 0x55554000 // FMSC OSPEED values 01 10MHz
//                    |       |        |
#define PF_CTRL_BITS 0b0001000000000000  // PF12(FSMC_A6)
#define PF_FSMC_BITS 0b0001000000000000
#define PF_AFR0_MASK         0x00000000
#define PF_AFR0_FSMC         0x00000000
#define PF_AFR1_MASK 0x000F0000
#define PF_AFR1_FSMC 0x000C0000
//                    |       |        |
#define PF_MODE_MASK 0x03000000 // all FMSC MODE bits
#define PF_MODE_FSMC 0x02000000 // FMSC MODE values 10 alternate function
#define PF_OSPD_FSMC 0x01000000 // FMSC OSPEED values 01 10MHz
//                    |       |        |
#define PG_CTRL_BITS 0b0001000000000000  // PG12(FSMC_NE4)
#define PG_FSMC_BITS 0b0001000000000000
#define PG_AFR0_MASK         0x00000000
#define PG_AFR0_FSMC         0x00000000
#define PG_AFR1_MASK 0x000F0000
#define PG_AFR1_FSMC 0x000C0000
//                    |       |        |
#define PG_MODE_MASK 0x03000000 // all FMSC MODE bits
#define PG_MODE_FSMC 0x02000000 // FMSC MODE values 10 alternate function
#define PG_OSPD_FSMC 0x01000000 // FMSC OSPEED values 01 10MHz

#define ADDSET 15 // (ADDSET+1)*6ns = CS to RW
#define DATAST 30 // (DATAST+1)*6ns = RW length

// these are defined for STM32F103V in
// ...\Arduino\hardware\STM32GENERIC\STM32\system\STM32F1\CMSIS_Inc\stm32f103xe.h
// but not for STM32F407V/Z, maybe because relocatable
#define FSMC_BASE             ((uint32_t)0x60000000) /*!< FSMC base address */
#define FSMC_BANK1            (FSMC_BASE)               /*!< FSMC Bank1 base address */
#define FSMC_BANK1_1          (FSMC_BANK1)              /*!< FSMC Bank1_1 base address */
#define FSMC_BANK1_2          (FSMC_BANK1 + 0x04000000) /*!< FSMC Bank1_2 base address */
#define FSMC_BANK1_3          (FSMC_BANK1 + 0x08000000) /*!< FSMC Bank1_3 base address */
#define FSMC_BANK1_4          (FSMC_BANK1 + 0x0C000000) /*!< FSMC Bank1_4 base address */

#define FSMC_BANK2            (FSMC_BASE + 0x10000000)  /*!< FSMC Bank2 base address */
#define FSMC_BANK3            (FSMC_BASE + 0x20000000)  /*!< FSMC Bank3 base address */
#define FSMC_BANK4            (FSMC_BASE + 0x30000000)  /*!< FSMC Bank4 base address */

#define CommandAccess FSMC_BANK1_4 // FSMC_NE4
//#define DataAccess (FSMC_BANK1 + 0x80000) // FSMC_A18 // BLACK_F407VE
//#define DataAccess (FSMC_BANK1_4 + 0x800) // FSMC_A10 // STM32F407ZG M4 DEMO
#define DataAccess (FSMC_BANK1_4 + 0x00080) // FSMC_A6  // BLACK_F407ZE

GxIO_STM32F407ZET6_FSMC::GxIO_STM32F407ZET6_FSMC(bool bl_active_high)
{
  _cs   = PG12; // FSMC_NE4
  _rs   = PF12; // FSMC_A6
  _rst  = 0;    // not available, driven from NRST
  _wr   = PD5;  // FSMC_NWE
  _rd   = PD4;  // FSMC_NOE
  _bl   = PB1;
  _bl_active_high = bl_active_high;
}

void GxIO_STM32F407ZET6_FSMC::reset()
{
  // _rst pin not available
}

void GxIO_STM32F407ZET6_FSMC::init()
{
  RCC->AHB1ENR   |= 0x00000078; // add G,F,E,D, to (C,B,A)
  volatile uint32_t t = RCC->AHB1ENR; // delay

  GPIOD->AFR[0]  = ( GPIOD->AFR[0] & ~PD_AFR0_MASK) | PD_AFR0_FSMC;
  GPIOD->AFR[1]  = ( GPIOD->AFR[1] & ~PD_AFR1_MASK) | PD_AFR1_FSMC;
  GPIOD->MODER   = ( GPIOD->MODER & ~PD_MODE_MASK) | PD_MODE_FSMC;
  GPIOD->OSPEEDR = ( GPIOD->OSPEEDR & ~PD_MODE_MASK) | PD_OSPD_FSMC;
  GPIOD->OTYPER &= ~PD_MODE_MASK;
  GPIOD->PUPDR  &= ~PD_MODE_MASK;

  GPIOE->AFR[0]  = (GPIOE->AFR[0] & ~PE_AFR0_MASK) | PE_AFR0_FSMC;
  GPIOE->AFR[1]  = (GPIOE->AFR[1] & ~PE_AFR1_MASK) | PE_AFR1_FSMC;
  GPIOE->MODER   = (GPIOE->MODER & ~PE_MODE_MASK) | PE_MODE_FSMC;
  GPIOE->OSPEEDR = (GPIOE->OSPEEDR & ~PE_MODE_MASK) | PE_OSPD_FSMC;
  GPIOE->OTYPER &= ~PE_MODE_MASK;
  GPIOE->PUPDR  &= ~PE_MODE_MASK;

  GPIOF->AFR[0]  = (GPIOF->AFR[0] & ~PF_AFR0_MASK) | PF_AFR0_FSMC;
  GPIOF->AFR[1]  = (GPIOF->AFR[1] & ~PF_AFR1_MASK) | PF_AFR1_FSMC;
  GPIOF->MODER   = (GPIOF->MODER & ~PF_MODE_MASK) | PF_MODE_FSMC;
  GPIOF->OSPEEDR = (GPIOF->OSPEEDR & ~PF_MODE_MASK) | PF_OSPD_FSMC;
  GPIOF->OTYPER &= ~PF_MODE_MASK;
  GPIOF->PUPDR  &= ~PF_MODE_MASK;

  GPIOG->AFR[0]  = (GPIOG->AFR[0] & ~PG_AFR0_MASK) | PG_AFR0_FSMC;
  GPIOG->AFR[1]  = (GPIOG->AFR[1] & ~PG_AFR1_MASK) | PG_AFR1_FSMC;
  GPIOG->MODER   = (GPIOG->MODER & ~PG_MODE_MASK) | PG_MODE_FSMC;
  GPIOG->OSPEEDR = (GPIOG->OSPEEDR & ~PG_MODE_MASK) | PG_OSPD_FSMC;
  GPIOG->OTYPER &= ~PG_MODE_MASK;
  GPIOG->PUPDR  &= ~PG_MODE_MASK;

  RCC->AHB3ENR         |= 0x00000001;
  t = RCC->AHB1ENR; // delay
  (void)(t);

  //FSMC_Bank1->BTCR[0] = 0x000010D9;             // FSMC_BANK1_1 // BLACK_F407VE
  //FSMC_Bank1->BTCR[1] = (DATAST << 8) | ADDSET; // FSMC_BANK1_1 // BLACK_F407VE
  FSMC_Bank1->BTCR[6] = 0x000010D9;               // FSMC_BANK1_4 // BLACK_F407ZE
  FSMC_Bank1->BTCR[7] = (DATAST << 8) | ADDSET;   // FSMC_BANK1_4 // BLACK_F407ZE

  digitalWrite(_bl, LOW);
  pinMode(_bl, OUTPUT);
}

uint8_t GxIO_STM32F407ZET6_FSMC::readDataTransaction()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GxIO_STM32F407ZET6_FSMC::readData16Transaction()
{
  return *(uint16_t*)DataAccess;
}

uint8_t GxIO_STM32F407ZET6_FSMC::readData()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GxIO_STM32F407ZET6_FSMC::readData16()
{
  return *(uint16_t*)DataAccess;
}

uint32_t GxIO_STM32F407ZET6_FSMC::readRawData32(uint8_t part)
{
  return *(uint16_t*)DataAccess;
}

void GxIO_STM32F407ZET6_FSMC::writeCommandTransaction(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GxIO_STM32F407ZET6_FSMC::writeCommand16Transaction(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GxIO_STM32F407ZET6_FSMC::writeDataTransaction(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GxIO_STM32F407ZET6_FSMC::writeData16Transaction(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GxIO_STM32F407ZET6_FSMC::writeCommand(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GxIO_STM32F407ZET6_FSMC::writeCommand16(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GxIO_STM32F407ZET6_FSMC::writeData(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GxIO_STM32F407ZET6_FSMC::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    *(uint8_t*)DataAccess = *d;
    d++;
    num--;
  }
}

void GxIO_STM32F407ZET6_FSMC::writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GxIO_STM32F407ZET6_FSMC::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_STM32F407ZET6_FSMC::startTransaction()
{
}

void GxIO_STM32F407ZET6_FSMC::endTransaction()
{
}

void GxIO_STM32F407ZET6_FSMC::selectRegister(bool rs_low)
{
}

void GxIO_STM32F407ZET6_FSMC::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit == _bl_active_high));
}

#endif
#endif
