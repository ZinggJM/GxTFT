// created by Jean-Marc Zingg to be the GxIO_STM32Nucleo144_FSMC io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM Nucleo-144 boards F767ZI and F746ZG, e.g. with my Nucleo-144 proto board, matching the pinout of the following processor board:
// https://www.aliexpress.com/item/Free-shipping-STM32F407VET6-development-board-Cortex-M4-STM32-minimum-system-learning-board-ARM-core-board/32618222721.html
//
// e.g. for direct matching display
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), 
// board "Nucleo-144" part "Nucleo F767ZI", or "Nucleo F746ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F7xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_NUCLEO_F767ZI) || defined(ARDUINO_NUCLEO_F746ZG)

#include "GxIO_STM32Nucleo144_FSMC.h"

// TFT connector uses FSMC pins
// D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
// PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

// connector pins
// 01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32
// GND RST D15 D14 D13 D12 D11 D10 D9  D8  D7  D6  D5  D4  D3  D2  D1  D0  RD  WR  RS  CS  SCK SCS SI  SO  INT BLK SET GND 3.3 GND
//         D10 D9  D8  E15 E14 E13 E12 E11 E10 E9  E8  E7  D1  D0  D15 D14 D4  D5  D13 D7                      B1

// D used : 15,14,13,..,..,10, 9, 8, 7,.., 5, 4,..,.., 1, 0, // 11
// D data : 15,14,..,..,..,10, 9, 8,..,..,..,..,..,.., 1, 0, // 7
//         |           |           |           |           |
// E used : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9
// E data : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9

//                    |       |        | // Ruler on 8 & 16
#define PD_DATA_BITS 0b1100011100000011
#define PD_CTRL_BITS 0b0010000010110000  // PD13(FSMC_A18),PD7(FSMC_NE1),PD5(FSMC_NWE),PD4(FSMC_NOE)
#define PD_FSMC_BITS 0b1110011110110011
#define PD_AFR0_MASK         0xF0FF00FF
#define PD_AFR0_FSMC         0xC0CC00CC
#define PD_AFR1_MASK 0xFFF00FFF
#define PD_AFR1_FSMC 0xCCC00CCC
//                    |       |        |
// PD_MODE_MASK      0q3330033330330033 // quaternary notation, 2bit per digit
// PD_MODE_FSMC      0q2220022220220022 //
// PD_OSPD_FSMC      0q1110011110110011 //
#define PD_MODE_MASK 0xFC3FCF0F // all FMSC MODE bits, 0q'33'30'03'33'30'33'00'33
#define PD_MODE_FSMC 0xA82A8A0A // FMSC MODE values 10 alternate function
#define PD_OSPD_FSMC 0x54154505 // FMSC OSPEED values 01 10MHz
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

#define ADDSET 15 // (ADDSET+1)*6ns = CS to RW
#define DATAST 30 // (DATAST+1)*6ns = RW length

//#define FMC_REGION ((uint32_t)0x60000000) // Bank1 FMC NOR/PSRAM
// swap FMC_REGION used to an address range that has data cache disabled
// see e.g. https://community.st.com/s/question/0D50X00009XkWQESA3/stm32h743ii-fmc-8080-lcd-spurious-writes
#define FMC_REGION ((uint32_t)0xC0000000) // Bank1 FMC NOR/PSRAM swapped with SDRAM

#define CommandAccess FMC_REGION
#define DataAccess (FMC_REGION + 0x80000)

GxIO_STM32Nucleo144_FSMC::GxIO_STM32Nucleo144_FSMC(bool bl_active_high)
{
  _cs   = PD7;  // FSMC_NE1
  _rs   = PD13; // FSMC_A18
  _rst  = 0;    // not available, driven from NRST
  _wr   = PD5;  // FSMC_NWE
  _rd   = PD4;  // FSMC_NOE
  _bl   = PB1;
  _bl_active_high = bl_active_high;
}

void GxIO_STM32Nucleo144_FSMC::reset()
{
  // _rst pin not available
}

void GxIO_STM32Nucleo144_FSMC::init()
{
  RCC->AHB1ENR   |= 0x00000078; // enable GPIOD, GPIOE, GPIOF and GPIOG interface clock
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

  RCC->AHB3ENR         |= 0x00000001;
  t = RCC->AHB1ENR; // delay
  (void)(t);

  FMC_Bank1->BTCR[0] = 0x000010D9;
  FMC_Bank1->BTCR[1] = (DATAST << 8) | ADDSET;
// swap FMC_REGION used to an address range that has data cache disabled
  SYSCFG->MEMRMP |= SYSCFG_MEMRMP_SWP_FMC_0;

  digitalWrite(_bl, LOW);
  pinMode(_bl, OUTPUT);
}

uint8_t GxIO_STM32Nucleo144_FSMC::readDataTransaction()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GxIO_STM32Nucleo144_FSMC::readData16Transaction()
{
  return *(uint16_t*)DataAccess;
}

uint8_t GxIO_STM32Nucleo144_FSMC::readData()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GxIO_STM32Nucleo144_FSMC::readData16()
{
  return *(uint16_t*)DataAccess;
}

uint32_t GxIO_STM32Nucleo144_FSMC::readRawData32(uint8_t part)
{
  return *(uint16_t*)DataAccess;
}

void GxIO_STM32Nucleo144_FSMC::writeCommandTransaction(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GxIO_STM32Nucleo144_FSMC::writeCommand16Transaction(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GxIO_STM32Nucleo144_FSMC::writeDataTransaction(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GxIO_STM32Nucleo144_FSMC::writeData16Transaction(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GxIO_STM32Nucleo144_FSMC::writeCommand(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GxIO_STM32Nucleo144_FSMC::writeCommand16(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GxIO_STM32Nucleo144_FSMC::writeData(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GxIO_STM32Nucleo144_FSMC::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    *(uint8_t*)DataAccess = *d;
    d++;
    num--;
  }
}

void GxIO_STM32Nucleo144_FSMC::writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GxIO_STM32Nucleo144_FSMC::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void GxIO_STM32Nucleo144_FSMC::startTransaction()
{
}

void GxIO_STM32Nucleo144_FSMC::endTransaction()
{
}

void GxIO_STM32Nucleo144_FSMC::selectRegister(bool rs_low)
{
}

void GxIO_STM32Nucleo144_FSMC::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit == _bl_active_high));
}

#endif
#endif
