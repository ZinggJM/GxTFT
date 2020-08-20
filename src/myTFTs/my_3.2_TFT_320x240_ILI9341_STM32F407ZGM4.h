// e.g. https://www.aliexpress.com/store/product/3-2-inch-Color-touch-screen-TFT-module-parallel-LCD-With-ILI9341-STM32-STM8-STM8S-driver/829772_32812961761.html
// on   https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F4 series" part "BLACK F407ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#include "../GxIO/STM32MICRO/GxIO_STM32F407ZGM4_P16/GxIO_STM32F407ZGM4_P16.h"
#include "../GxCTRL/GxCTRL_ILI9341/GxCTRL_ILI9341.h" // 240x320
GxIO_Class io; // #define GxIO_Class is in the selected header file
GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file
TFT_Class tft(io, controller, 320, 240); // landscape 240x320
//TFT_Class tft(io, controller, 240, 320); // portrait 240x320
