// e.g. https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
//      this display doesn't need power on pin 28 (NC)
// on   https://www.aliexpress.com/item/32720494327.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "Generic F103ZE".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#include "../GxIO/STM32MICRO/GxIO_STM32F103ZET6M3_P16/GxIO_STM32F103ZET6M3_P16.h"
#include "../GxCTRL/GxCTRL_ILI9486/GxCTRL_ILI9486.h" // 320x480
GxIO_Class io; // #define GxIO_Class is in the selected header file
GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file
TFT_Class tft(io, controller, 480, 320); // landscape 320x480
//TFT_Class tft(io, controller, 320, 480); // portrait 320x480
