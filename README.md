# GxTFT for Arduino : a New Approach for a General TFT Library

This library separates the aspects IO connection, controller type and display class into separate C++ classes

The purpose of this Library and its design is to make additions easy.

This is the right place to present my ideas for this topic. I do it step-by-step.

The initial commit serves as "proof of concept". It supports:

* HVGA on MEGA
* HVGA on DUE
* HVGA on SPI (3.5inch RPI Display)

Added GxIO classes for TIKY 5inch display

Fixed TIKY GxIO classes and GxCTRL_ILI9806

Added Updates 1 :

Renamed GxIO classes for HVGA on MEGA and DUE

Added read support to GxIO classes (partially tested only, some shields are output only).

Updated GxCTRL and its subclasses; added GxCTRL_RA8875S

Updated read support; read support is controller specific, issues remain to be investigated.

Added GxTFT & RA8875P
