// e.g. https://www.aliexpress.com/item/New-7-inch-TFT-LCD-module-800x480-SSD1963-Touch-PWM-For-Arduino-AVR-STM32-ARM/32667404985.html
//
// this version is for use with Arduino package "STM32F1 Boards (STM32Duino.com)" Roger Clark
// Board: "Generic STMF103C series", Variant: "STM32F103C8 (xxx)"

#include "GxIO/STM32DUINO/GxIO_STM32F103BluePill_P16/GxIO_STM32F103BluePill_P16.h"
#include "GxCTRL/GxCTRL_SSD1963/GxCTRL_SSD1963.h" // 800x480 e.g. 7inch Display
GxIO_Class io; // #define GxIO_Class is in the selected header file
GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file
TFT_Class tft(io, controller, 800, 480); // landscape 800x480 7inch Display
