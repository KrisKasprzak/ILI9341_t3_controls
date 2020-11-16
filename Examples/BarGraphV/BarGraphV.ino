/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  touchscreen lib           https://github.com/dgolda/UTouch


  BarChartV(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc);
  void init(const char *Title, uint16_t TextColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );
  void setBarColor(uint16_t val = 0xF800);
  void draw(float val);
  void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);
  void showTitle(bool val);
  void showScale(bool val);

*/

/*

  implementation is 4 lines of code, see step xxx below

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include "UTouch.h"               // touchscreen lib
// step 1 implement the library
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>  // custom control define file

#define FONT_TITLE Arial_24
#define FONT_DATA Arial_16

#define TFT_CS        10
#define TFT_DC        9
#define LED_PIN       A9


ILI9341_t3 Display(TFT_CS, TFT_DC);

int a7Bits, a8Bits;
float a7Volts, a8Volts;

// step 2 create the bar chart objects
BarChartV A7Volts(&Display );
BarChartV A8Volts(&Display );

void setup() {
  Serial.begin(9600);

  pinMode(TFT_CS,  OUTPUT);
  pinMode(TFT_DC,  OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  while (!Serial); // used for leonardo debugging

  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(ILI9341_BLACK);
  digitalWrite(LED_PIN, HIGH);

  // step 3 initialize the bar chart objects
  A7Volts.init(   0, 230, 50, 180, 0, 4.0, 1.0, "A7", C_WHITE, C_WHITE, C_RED, C_BLACK, C_BLACK, FONT_TITLE, FONT_DATA);
  A8Volts.init(120,  230, 50, 180, 0, 4.0, 0.5, "A8", C_WHITE, C_BLACK, C_GREEN, C_DKGREEN,  C_BLACK, FONT_TITLE, FONT_DATA);

  // optional but shows how you can hide the scale
  A8Volts.showScale(false);
}

void loop() {

  a7Bits = analogRead(A7);
  a8Bits = analogRead(A8);

  a7Volts = a7Bits * 3.3 / 1024;
  a8Volts = a8Bits * 3.3 / 1024;

  // optional but shows how to change the color after init
  if (a7Volts > 2) {
    A7Volts.setBarColor(C_RED);
  }
  else {
    A7Volts.setBarColor(C_BLUE);
  }

  if (a8Volts > 2) {
    A8Volts.setBarColor(C_RED);
  }
  else {
    A8Volts.setBarColor(C_GREEN);
  }

  // step 4 draw the bar chart objects
  A7Volts.draw(a7Volts);
  A8Volts.draw(a8Volts);

  delay(50);


}
