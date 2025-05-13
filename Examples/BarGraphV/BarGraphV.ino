/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3

  BarChartV(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc);
  void init(const char *Title, uint16_t TextColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );
  void setBarColor(uint16_t val = 0xF800);
  void draw(float val);
  void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);
  void showTitle(bool val);
  void showScale(bool val);

*/

/*

  implementation is 4 steps

*/

#include <ILI9341_t3.h>  // fast display driver lib
// step 1 implement the library
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>  // custom control define file
#define FONT_TITLE Arial_24
#define FONT_DATA Arial_16

#define TFT_CS 9
#define TFT_DC 2

ILI9341_t3 Display(TFT_CS, TFT_DC);

int a7Bits, a8Bits;
float a7Volts, a8Volts;

// step 2 create the bar chart objects
BarChartV A7Volts(&Display);
BarChartV A8Volts(&Display);

void setup() {
  Serial.begin(115200);

  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(ILI9341_BLACK);

  // step 3 initialize the bar chart objects
  A7Volts.init(10, 230, 100, 198, 0.0, 3.3, 0.1, "A7", ILI9341_WHITE, ILI9341_WHITE, ILI9341_RED, ILI9341_BLACK, ILI9341_BLACK, FONT_TITLE, FONT_DATA);

  // optional hide scales
  A7Volts.showScale(false);
  A7Volts.showTitle(false);

  // optional draw bars with segments (looks like a segment UV meter)
  A7Volts.useSegmentBars(true);
  // set colors for 3 color segments and 4th for null color
  A7Volts.setSectionColors(ILI9341_GREEN, ILI9341_YELLOW, ILI9341_RED, 0x2104);
  // how many bars for each (these are percentages (70% for green, 20% for yellow, and rest red))
  A7Volts.setSectionSize(.70, .90);
  // independent scale for bars (so you can have say 50 bars but numerical scale of 1-2-3-4-5)
  A7Volts.setBars(.1, 1);
  // caution you will need to do some math to ensure the resulting total bar height is within you set height
  // bar height is computed and if not and integer, we'll make it one. Makes guarenteeting no odd dividers but height may be off
  // of you must have fixed height and same divider bars, make sure barheight is integer

  // bars = (ScaleHigh - ScaleLow) / BarInc;
  // barheight = GraphHeight / bars;
  // note: uint16_t used for barheight, but this will return a float so you can verify graph hight and bar inc scaler
  Serial.println(A7Volts.getBars());
  Serial.println(A7Volts.getBarHeight());

  // initalize another for a different example
  A8Volts.init(200, 230, 50, 180, 0, 4.0, 0.5, "A8", ILI9341_WHITE, ILI9341_BLACK, ILI9341_GREEN, ILI9341_BLACK, ILI9341_BLACK, FONT_TITLE, FONT_DATA);
}

void loop() {

  // step 4 get some data
  a7Bits = analogRead(A7);
  a8Bits = analogRead(A8);

  a7Volts = a7Bits * 3.3 / 1024.0f;
  a8Volts = a8Bits * 3.3 / 1024.0f;

  // optional but shows how to change the color after init
  if (a8Volts > 2) {
    A8Volts.setBarColor(ILI9341_RED);
  } else {
    A8Volts.setBarColor(ILI9341_GREEN);
  }

  // step 5 draw the bar chart objects
  A7Volts.draw(a7Volts);
  A8Volts.draw(a8Volts);

  delay(50);
}
