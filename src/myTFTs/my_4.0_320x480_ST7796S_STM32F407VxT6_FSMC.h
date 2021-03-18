// e.g. https://www.aliexpress.com/item/1899444540.html "4.0 inch HD TFT LCD Screen with PCB Board ILI9488 ST7796S Drive IC 320(RGB)*480 8Bit Parallel Interface (Touch/No Touch)" Maithoga
// on   https://www.aliexpress.com/item/1005001806399987.html STM32F407VxT6 board
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F4 series" part "Generic F407VE" or "Generic F407VG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#include "../GxIO/STM32MICRO/GxIO_STM32F407VxT6_FSMC/GxIO_STM32F407VxT6_FSMC.h"
#include "../GxCTRL/GxCTRL_ST7796S/GxCTRL_ST7796S.h" // 320x480
GxIO_Class io; // #define GxIO_Class is in the selected header file
GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file
TFT_Class tft(io, controller, 480, 320); // landscape 480x320
//TFT_Class tft(io, controller, 230, 480); // portrait 320x480
