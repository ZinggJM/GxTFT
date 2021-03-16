// e.g. https://www.aliexpress.com/item/32676929794.html "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" display from Ruijia Industry
// on   https://www.aliexpress.com/item/1005001806399987.html STM32F407VxT6 board
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F4 series" part "Generic F407VE" or "Generic F407VG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#include "../GxIO/STM32MICRO/GxIO_STM32F407VxT6_P16/GxIO_STM32F407VxT6_P16.h"
#include "../GxCTRL/GxCTRL_NT35510/GxCTRL_NT35510.h" // 800x480
GxIO_Class io; // #define GxIO_Class is in the selected header file
GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file
TFT_Class tft(io, controller, 800, 480); // landscape 800x480
//TFT_Class tft(io, controller, 480, 800); // portrait 480x800
