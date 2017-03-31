/* Modified by Bodmer to be an example for TFT_HX8357 library.
   This sketch uses the GLCD font only.

   The performance for each test is reported to the serial
   port at 38400 baud.

   This test occupies the whole of the display therefore
   will take twice as long as it would on a 320 x 240
   display. Bear this in mind when making performance
   comparisons.

   Make sure all the required font is loaded by editting the
   User_Setup.h file in the GxTFT library folder.

   Original header is at the end of the sketch, some text in it is
   not applicable to the HX8357 display supported by this example.
*/

// modified by Jean-Marc Zingg to be an example for the GxTFT library
// original source taken from https://github.com/Bodmer/TFT_HX8357

#include <GxTFT_GFX.h> // Hardware-specific library

// select one display class
#define TFT_Class GxTFT_GFX

// select one GxIO class
#include <GxIO/GxIO_DUE_P16_DUESHIELD/GxIO_DUE_P16_DUESHIELD.h>
//#include <GxIO/GxIO_DUE_P16_TIKY/GxIO_DUE_P16_TIKY.h>
//#include <GxIO/GxIO_DUE_P16_WIRED/GxIO_DUE_P16_WIRED.h>
//#include <GxIO/GxIO_MEGA_P16_MEGASHIELD/GxIO_MEGA_P16_MEGASHIELD.h>
//#include <GxIO/GxIO_SPI/GxIO_SPI.h>
//#include <GxIO/GxIO_STM32F103C8T6_P16_TIKY/GxIO_STM32F103C8T6_P16_TIKY.h>
//#include <GxIO/GxIO_STM32F103V_P16_TIKY/GxIO_STM32F103V_P16_TIKY.h>
//#include <GxIO/GxIO_STM32F407V_P16/GxIO_STM32F407V_P16.h>
//#include <GxIO/GxIO_UNO_P8_SHIELD/GxIO_UNO_P8_SHIELD.h>


// select one GxCTRL class
//#include <GxCTRL/GxCTRL_HX8357B/GxCTRL_HX8357B.h>
//#include <GxCTRL/GxCTRL_HX8357C/GxCTRL_HX8357C.h>
//#include <GxCTRL/GxCTRL_ILI9341/GxCTRL_ILI9341.h> // 240x320
//#include <GxCTRL/GxCTRL_ILI9481/GxCTRL_ILI9481.h> // HVGA 320x480
//#include <GxCTRL/GxCTRL_ILI9486/GxCTRL_ILI9486.h> // 320x480 e.g. 3.5inch RPI Display
//#include <GxCTRL/GxCTRL_ILI9806/GxCTRL_ILI9806.h> // 854x480 e.g. Tiky 5" TFT from Ruijia Industry
#include <GxCTRL/GxCTRL_SSD1963/GxCTRL_SSD1963.h> // 800x480 e.g. 7inch Display

GxIO_Class io; // #define GxIO_Class is in the selected header file

// for SPI, the constructor needs parameters
//GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
//GxIO_Class io(SPI, SS, D4, D3); // 480x320 3.5inch RPI Display on Wemos D1 (ESP8266)

GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file

// select one or adapt
//TFT_Class tft(io, controller, 240, 320); // portrait 240x320
//TFT_Class tft(io, controller, 320, 240); // landscape 240x320
//TFT_Class tft(io, controller, 320, 480); // portrait HVGA 320x480 or 3.5inch RPI Display
//TFT_Class tft(io, controller, 480, 320); // landscape HVGA 320x480 or 3.5inch RPI Display
//TFT_Class tft(io, controller, 480, 800); // portrait 800x480 7inch Display
TFT_Class tft(io, controller, 800, 480); // landscape 800x480 7inch Display
//TFT_Class tft(io, controller, 854, 480); // 854x480 e.g. Tiky 5" TFT from Ruijia Industry
//TFT_Class tft(io, controller, 480, 854); // 854x480 e.g. Tiky 5" TFT from Ruijia Industry

// include the selected code
#include "GxTFT_IncludeCpp.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(String(controller.name) + " Test on " + String(io.name));

  tft.init();

  reportID();
  delay(2000);
  reportReg();
  delay(5000);
  //while(1);
  tft.init(); // reset, in case read disturbed the controller

  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);

  //return;

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(3000);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(RED, BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(YELLOW, MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println(F("Done!"));

}

void loop(void)
{
  for (uint8_t rotation = 0; rotation < 4; rotation++)
  {
    tft.setRotation(rotation);
    testText();
    delay(10000);
  }
}

uint16_t readReg(uint16_t reg)
{
  io.startTransaction();
  io.writeCommand(reg);
  uint16_t rv = io.readData16();
  io.endTransaction();
  return rv;
}

uint32_t readReg40(uint16_t reg)
{
  uint16_t h, m, l;
  io.startTransaction();
  io.writeCommand(reg);
  h = io.readData16();
  m = io.readData16();
  l = io.readData16();
  io.endTransaction();
  return ((uint32_t) h << 24) | (m << 8) | (l >> 8);
}

void reportID()
{
  // To Do : generalize handling of read controller ID
  if (String(controller.name) == String("GxCTRL_SSD1963"))
  {
    // SSD1963 interprets any command as command, not as parameter addess!
    // Read DDB
    io.startTransaction();
    io.writeCommand(0xA1);
    uint8_t p1 = io.readData();
    uint8_t p2 = io.readData();
    uint8_t p3 = io.readData();
    uint8_t p4 = io.readData();
    uint8_t p5 = io.readData();
    io.endTransaction();
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);  tft.setTextSize(2);
    tft.print("SSD1963 DDB p1 : 0x"); tft.println(p1, HEX);
    tft.print("SSD1963 DDB p2 : 0x"); tft.println(p2, HEX);
    tft.print("SSD1963 DDB p3 : 0x"); tft.println(p3, HEX);
    tft.print("SSD1963 DDB p4 : 0x"); tft.println(p4, HEX);
    tft.print("SSD1963 DDB p4 : 0x"); tft.println(p5, HEX);
    tft.println();
    Serial.print("SSD1963 DDB p1 : 0x"); Serial.println(p1, HEX);
    Serial.print("SSD1963 DDB p2 : 0x"); Serial.println(p2, HEX);
    Serial.print("SSD1963 DDB p3 : 0x"); Serial.println(p3, HEX);
    Serial.print("SSD1963 DDB p4 : 0x"); Serial.println(p4, HEX);
    Serial.print("SSD1963 DDB p4 : 0x"); Serial.println(p5, HEX);
  }
  else
  {
    uint32_t rr40BF = readReg40(0xBF);
    uint16_t rr0 = readReg(0);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);  tft.setTextSize(2);
    tft.print("readReg40(0xBF) :  0x"); tft.println(rr40BF, HEX);
    tft.print("readReg(0)      :  0x"); tft.println(rr0, HEX);
    tft.println();
    Serial.print("readReg40(0xBF) :  0x"); Serial.println(rr40BF, HEX);
    Serial.print("readReg(0)      :  0x"); Serial.println(rr0, HEX);
    Serial.println();
  }
}

void reportReg()
{
  // To Do : generalize handling of read controller ID
  if (String(controller.name) == String("GxCTRL_SSD1963"))
  {
    readReg(0x0A, 1, "Get Power Mode");
    readReg(0x0B, 1, "Get Address Mode");
    readReg(0x0C, 1, "Get Pixel Format");
    readReg(0x0D, 1, "Get Display Mode");
    readReg(0x0E, 1, "Get Signal Mode");
  }
  else
  {
    readReg(0x00, 2, "ID: ILI9320, ILI9325, ILI9335, ...");
    readReg(0x04, 4, "Manufacturer ID");
    readReg(0x09, 5, "Status Register");
    readReg(0x0A, 2, "Get Power Mode");
    readReg(0x0C, 2, "Get Pixel Format");
    readReg(0x61, 2, "RDID1 HX8347-G");
    readReg(0x62, 2, "RDID2 HX8347-G");
    readReg(0x63, 2, "RDID3 HX8347-G");
    readReg(0x64, 2, "RDID1 HX8347-A");
    readReg(0x65, 2, "RDID2 HX8347-A");
    readReg(0x66, 2, "RDID3 HX8347-A");
    readReg(0x67, 2, "RDID Himax HX8347-A");
    readReg(0x70, 2, "Panel Himax HX8347-A");
    readReg(0xA1, 5, "RD_DDB SSD1963");
    readReg(0xB0, 2, "RGB Interface Signal Control");
    readReg(0xB4, 2, "Inversion Control");
    readReg(0xB6, 5, "Display Control");
    readReg(0xB7, 2, "Entry Mode Set");
    readReg(0xBF, 6, "ILI9481, HX8357-B");
    readReg(0xC0, 9, "Panel Control");
    readReg(0xC8, 13, "GAMMA");
    readReg(0xCC, 2, "Panel Control");
    readReg(0xD0, 3, "Power Control");
    readReg(0xD2, 5, "NVM Read");
    readReg(0xD3, 4, "ILI9341, ILI9488");
    readReg(0xDA, 2, "RDID1");
    readReg(0xDB, 2, "RDID2");
    readReg(0xDC, 2, "RDID3");
    readReg(0xE0, 16, "GAMMA-P");
    readReg(0xE1, 16, "GAMMA-N");
    readReg(0xEF, 6, "ILI9327");
    readReg(0xF2, 12, "Adjust Control 2");
    readReg(0xF6, 4, "Interface Control");
  }
}

void printhex(uint8_t val)
{
  if (val < 0x10) tft.print("0");
  tft.print(val, HEX);
  if (val < 0x10) Serial.print("0");
  Serial.print(val, HEX);
}

void readReg(uint16_t reg, uint8_t n, const char *msg)
{
  uint8_t n_max = 20;
  n = min(n, n_max);
  uint8_t val8[n_max];
  io.startTransaction();
  io.writeCommand(reg);
  for (uint8_t i = 0; i < n; i++)
  {
    val8[i] = io.readData();
  }
  io.endTransaction();
  tft.print("reg(0x");
  Serial.print("reg(0x");
  printhex(reg >> 8);
  printhex(reg);
  tft.print(")");
  Serial.print(")");
  for (uint8_t i = 0; i < n; i++)
  {
    tft.print(" ");
    Serial.print(" ");
    printhex(val8[i]);
  }
  tft.print("\t");
  tft.println(msg);
  Serial.print("\t");
  Serial.println(msg);
}

#if !defined(ESP8266)
#define yield()
#endif

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  yield();
  tft.fillScreen(RED);
  yield();
  tft.fillScreen(GREEN);
  yield();
  tft.fillScreen(BLUE);
  yield();
  tft.fillScreen(BLACK);
  yield();
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.setTextColor(GREEN);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(BLACK);
  yield();

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  t    += micros() - start;

  tft.fillScreen(BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  t    += micros() - start;

  tft.fillScreen(BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(BLACK);
  yield();
  start = micros();
  for (y = 0; y < h; y += 5) tft.drawFastHLine(0, y, w, color1);
  yield();
  for (x = 0; x < w; x += 5) tft.drawFastVLine(x, 0, h, color2);
  yield();

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for (i = 2; i < n; i += 6) {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n = min(tft.width(), tft.height());
  for (i = n; i > 0; i -= 6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx - i2, cy - i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx - i2, cy - i2, i, i, color2);
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(BLACK);
  start = micros();
  for (x = radius; x < w; x += r2) {
    for (y = radius; y < h; y += r2) {
      tft.fillCircle(x, y, radius, color);
    }
    yield();
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                      w = tft.width()  + radius,
                      h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for (x = 0; x < w; x += r2) {
    for (y = 0; y < h; y += r2) {
      tft.drawCircle(x, y, radius, color);
    }
    yield();
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n     = min(cx, cy);
  start = micros();
  for (i = 0; i < n; i += 5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for (i = min(cx, cy); i > 10; i -= 5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(i, i, 0));
    yield();
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for (i = 0; i < w; i += 6) {
    i2 = i / 2;
    tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for (i = min(tft.width(), tft.height()); i > 20; i -= 6) {
    i2 = i / 2;
    tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(0, i, 0));
    yield();
  }

  return micros() - start;
}

// Original sketch header
/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

