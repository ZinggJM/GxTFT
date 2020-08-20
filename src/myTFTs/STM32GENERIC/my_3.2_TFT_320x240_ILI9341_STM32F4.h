// e.g. https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html no longer available
//
// this version is for use with Arduino package STM32GENERIC, board "BLACK F407VE/ZE/ZG boards".
// Specific Board "BLACK F407VE (V2.0)"

#include "GxIO/STM32GENERIC/GxIO_STM32F407V_P16/GxIO_STM32F407V_P16.h"
#include "GxCTRL/GxCTRL_ILI9341/GxCTRL_ILI9341.h" // 240x320
GxIO_Class io; // #define GxIO_Class is in the selected header file
GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file
TFT_Class tft(io, controller, 320, 240); // landscape 240x320
//TFT_Class tft(io, controller, 240, 320); // portrait 240x320
