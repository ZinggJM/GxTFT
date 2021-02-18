//FSMC_PinTest by Jean-Marc Zingg

// TFT connector uses FSMC pins
// D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
// PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

uint8_t fsmc_pins[] = {PD14, PD15, PD0, PD1, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15, PD8, PD9, /*PD10*/};

#define PD_USED_BITS 0xE7B3
//#define PD_DATA_BITS 0xC703

#define PE_USED_BITS 0xFF80
//#define PE_DATA_BITS 0xFF80

#define PD_MODE_MASK 0xFC3FCF0F // all used bits
#define PD_MODE_BITS 0x54154505 // 01 : general purpose output mode
#define PE_MODE_MASK 0xFFFFC000 // all used bits
#define PE_MODE_BITS 0x55554000 // 01 : general purpose output mode

#define PD_MODE_DATA 0xF03F000F // all data bits
#define PD_MODE_OUTP 0x50150005 // 01 : general purpose output mode
#define PD_MODE_INP  0x00000000 // 00 : input floating mode
#define PE_MODE_DATA 0xFFFFC000 // all data bits
#define PE_MODE_OUTP 0x55554000 // 01 : general purpose output mode
#define PE_MODE_INP  0x00000000 // 00 : input floating mode


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
#define PD_MODE_MASK 0xFC3FCF0F // all FMSC MODE bits
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


void setup()
{
  //setup_P16();
  setup_FSMC();
}

void loop()
{
  //loop_P16();
  loop_FSMC();
}

void setup_P16()
{
  RCC->AHB1ENR |= 0x00000078; // enable GPIOD, GPIOE, GPIOF and GPIOG interface clock
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); // delay after an RCC peripheral clock enabling
  GPIOD->BSRR |= PD_USED_BITS; // preset all output high
  GPIOD->MODER &= ~PD_MODE_MASK;
  GPIOD->MODER |= PD_MODE_BITS;
  GPIOD->OTYPER &= ~PD_USED_BITS; // 0 : output push-pull
  GPIOD->OSPEEDR &= ~PD_MODE_MASK; // 0 : low speed
  GPIOD->PUPDR &= ~PD_MODE_MASK; // 0 : no pull-up, no pull-down

  GPIOE->BSRR |= PE_USED_BITS; // preset all output high
  GPIOE->MODER &= ~PE_MODE_MASK;
  GPIOE->MODER |= PE_MODE_BITS;
  GPIOE->OTYPER &= ~PE_USED_BITS; // 0 : output push-pull
  GPIOE->OSPEEDR &= ~PE_MODE_MASK; // 0 : low speed
  GPIOE->PUPDR &= ~PE_MODE_MASK; // 0 : no pull-up, no pull-down

  for (int i = 0; i < sizeof(fsmc_pins); i ++)
  {
    digitalWrite(fsmc_pins[i], LOW);
    //pinMode(fsmc_pins[i], OUTPUT);
  }
  GPIOD->BSRR = (0x1 << (10 + 16));  // PD10 low
  GPIOD->BSRR = (0x1 << (7 + 16));  // PD7 CS low
  GPIOD->BSRR = (0x1 << (13 + 16));  // PD13 RS low
  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low
  GPIOD->BSRR = (0x1 << (5 + 16));  // PD5 WR low
}

void loop_P16()
{
#if 0
  for (int i = 0; i < sizeof(fsmc_pins); i ++)
  {
    digitalWrite(fsmc_pins[i], HIGH);
  }
  GPIOD->BSRR = (0x1 << 10);  // PD10 high
  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
  GPIOD->BSRR = (0x1 << 13);  // PD13 RS high
  GPIOD->BSRR = (0x1 << 4);  // PD4 RD high
  GPIOD->BSRR = (0x1 << 5);  // PD5 WR high
  delay(2000);
  for (int i = 0; i < sizeof(fsmc_pins); i ++)
  {
    digitalWrite(fsmc_pins[i], LOW);
  }
  GPIOD->BSRR = (0x1 << (10 + 16));  // PD10 low
  GPIOD->BSRR = (0x1 << (7 + 16));  // PD7 CS low
  GPIOD->BSRR = (0x1 << (13 + 16));  // PD13 RS low
  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low
  GPIOD->BSRR = (0x1 << (5 + 16));  // PD5 WR low
  delay(2000);
  for (int i = 0; i < sizeof(fsmc_pins); i ++)
  {
    digitalWrite(fsmc_pins[i], HIGH);
    delay(1000);
    digitalWrite(fsmc_pins[i], LOW);
    delay(1000);
  }
#endif
  //  GPIOD->BSRR = (0x1 << 10);  // PD10 high
  //  delay(1000);
  //  GPIOD->BSRR = (0x1 << (10 + 16));  // PD10 low
  //  delay(1000);
  //  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
  //  delay(1000);
  //  GPIOD->BSRR = (0x1 << (7 + 16));  // PD7 CS low
  //  delay(1000);
  //  GPIOD->BSRR = (0x1 << 13);  // PD13 RS high
  //  delay(1000);
  //  GPIOD->BSRR = (0x1 << (13 + 16));  // PD13 RS low
  //  delay(1000);
  //  GPIOD->BSRR = (0x1 << 4);  // PD4 RD high
  //  delay(1000);
  //  GPIOD->BSRR = (0x1 << (4 + 16));  // PD4 RD low
  //  delay(1000);
  GPIOD->BSRR = (0x1 << 5);  // PD5 WR high
  delay(1000);
  GPIOD->BSRR = (0x1 << (5 + 16));  // PD5 WR low
  delay(1000);
}

void setup_FSMC()
{
  RCC->AHB1ENR   |= 0x00000078;
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
}

void loop_FSMC()
{
  GxIO_STM32Nucleo144_FSMC_writeData16(0xFFFF, 10000);
  delay(1000);
  GxIO_STM32Nucleo144_FSMC_writeData16(0x0000, 10000);
  delay(1000);
  //GxIO_STM32Nucleo144_FSMC_writeData16(0xFFFF, 1);
  //delay(1000);
  //GxIO_STM32Nucleo144_FSMC_writeData16(0x0000, 1);
  //delay(1000);
}

void GxIO_STM32Nucleo144_FSMC_writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}
