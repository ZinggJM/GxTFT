# GxTFT for Arduino

A New Approach for a General TFT Library.

### This library separates the aspects IO connection, controller type and display class into separate C++ classes

- The purpose of this Library and its design is to make additions easy.
- The initial commit served as "proof of concept", with HVGA TFT on MEGA, DUE, and 3.5" RPI HVGA on SPI
- This library supports a collection of TFT displays I bought to learn from and to learn from other libraries
- It supports some "exotic" displays for Arduino that were not made to use with Arduino IDE or Arduino boards
- I recommend to use other common Arduino TFT libraries whenever possible, as these are better supported
- my recommendations are MCUFRIEND_kbv, TFT_eSPI, and the whole range of Adafruit libraries

### Version 2.1.4
- added support for blue STM32F103ZET6 board
- supported by classes GxIO_STM32F103ZET6_P16 and GxIO_STM32F103ZET6_FSMC
- fixed pushColors methods
- implemented writeRect and pushRect methods (these were empty)
#### Version 2.1.3
- added FSMC/FMC support for my Nucleo-144 proto board with FSMC/FMC TFT connector
- supported by class GxIO_STM32Nucleo144_FSMC using FMC for F767ZI and F746ZG
- tested with 3.2" ILI9341 on F767ZI
#### Version 2.1.2
- added support for my Nucleo-144 proto board with FSMC/FMC TFT connector
- supported by class GxIO_STM32Nucleo144_P16 using bit-bashing for F767ZI and F746ZG
- tested with 3.2" ILI9341 on F767ZI
- FSMC/FMC version will follow, expected to be only slightly faster
#### Version 2.1.1
- added support for BLACK_F407ZE STM32 board with FSMC TFT connector
- supported by classes GxIO_STM32F407ZET6_P16 and GxIO_STM32F407ZET6_FSMC
#### Version 2.1.0
- all STM32 targets are now supported with official STM32 package from STMicroelectronics
- fixed a timing issue with OTM8009A support
#### Version 2.0.3
- fixed rotation for SSD1283A for pushColors (rendering canvas ok for all 4 rotations)
#### Version 2.0.2
- added support for 1.6" transflective SSD1283A TFT display
- added missing typecast for ESP8266 (pgm_read_dword)
#### Version 2.0.1
- added 16 bit command transfers to all GxIO classes for parallel 16 bit interfaces to support OTM8009A
- added GxIO_DUE_P16_R_SHIELD class (variant of GxIO_DUE_P16_DUESHIELD, e.g. for rDuinoScope)
#### Version 2.0.0
- new src directory structure avoids the need for .cpp includes
- only header files includes are needed
- Arduino IDE 1.8.x automatically includes needed code from .cpp files
- DO NOT include .cpp files with this version, linker would complain
- src directory structure makes GxTFT a more usual Arduino Library
#### Version 1.0.1
- Added OTM8009A support for 3.95" & 4.7" TFTs with FSMC connector on STM32 boards
#### Later Version (no number)
- Added FSMC GxIO classes for STM32 GxIO_STM32F1_FSMC and GxIO_STM32F4_FSMC and FSMC examples
- Added FSMC and P16 GxIO classes for STM32F407ZGM4 board
#### Later Version (no number)
- Renamed GxIO classes for HVGA on MEGA and DUE
- Added read support to GxIO classes (partially tested only, some shields are output only).
- Updated GxCTRL and its subclasses; added GxCTRL_RA8875S
- Updated read support; read support is controller specific, issues remain to be investigated.
- Added GxTFT & RA8875P
#### Later Version (no number)
- Added GxIO classes for TIKY 5inch display
- Fixed TIKY GxIO classes and GxCTRL_ILI9806
#### Initial Version (no number)
* HVGA on MEGA
* HVGA on DUE
* HVGA on SPI (3.5inch RPI Display)
