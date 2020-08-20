// e.g. https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html
// e.g. https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-with-capacitance-touch-panel-support-8-16-24-RGB/32666849087.html without touch panel
//
// this version is for use with Arduino package "STM32F1 Boards (STM32Duino.com)" Roger Clark
// Board: "Generic STMF103C series", Variant: "STM32F103C8 (xxx)"

#include "GxIO/STM32DUINO/GxIO_STM32F103BluePill_P16_TIKY/GxIO_STM32F103BluePill_P16_TIKY.h"
#include "GxCTRL/GxCTRL_ILI9806/GxCTRL_ILI9806.h" // 854x480 e.g. Tiky 5" TFT from Ruijia Industry
GxIO_Class io; // #define GxIO_Class is in the selected header file
GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file
TFT_Class tft(io, controller, 854, 480); // landscape 854x480 e.g. Tiky 5" TFT from Ruijia Industry
