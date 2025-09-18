/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3

  BarChartH(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc);
  void init(const char *Title, uint16_t TextColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );
  void setBarColor(uint16_t val = 0xF800);
  void plot(float val);
  void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);
  void showTitle(bool val);
  void showScale(bool val);

*/

/*

  implementation is 4 steps

*/

#include <ILI9341_t3.h>
// step 1 include the library
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>

#define FONT_TITLE Arial_24
#define FONT_DATA Arial_10

#define TFT_CS 9
#define TFT_DC 2
#define LED_PIN A9
#define B_XORIGIN 20
#define B_YORIGIN 50
#define B_WIDE 200
#define B_HEIGHT 50
#define B_LOWSCALE 0.0
#define B_HIGHSCALE 4.0
#define B_SCALEINC .5

#define V_XORIGIN 40
#define V_YORIGIN 150
#define V_WIDE 200
#define V_HEIGHT 50
#define V_LOWSCALE 0.0
#define V_HIGHSCALE 4.0
#define V_SCALEINC .5

#define BORDERCOLOR ILI9341_WHITE
#define BACKCOLOR ILI9341_BLACK

#define TEXT_COLOR ILI9341_WHITE
#define BORDER_COLOR ILI9341_WHITE
#define BAR_COLOR ILI9341_BLUE
#define NULL_COLOR ILI9341_BLACK
#define BACK_COLOR ILI9341_BLACK
#define WARNING_COLOR ILI9341_RED

ILI9341_t3 Display(TFT_CS, TFT_DC);

int a7Bits, a8Bits;
float a7Volts, a8Volts;

// step 2 create an object for each bar chart
BarChartH A7Volts(&Display);
BarChartH A8Volts(&Display);

void setup() {
  Serial.begin(115200);

  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(ILI9341_BLACK);
  digitalWrite(LED_PIN, HIGH);

  // step 3 initialize
  A7Volts.init(B_XORIGIN, B_YORIGIN, B_WIDE, B_HEIGHT, B_LOWSCALE, B_HIGHSCALE, B_SCALEINC, "A7 Voltage", ILI9341_WHITE, BORDERCOLOR, ILI9341_RED, ILI9341_BLACK, BACKCOLOR, FONT_TITLE, FONT_DATA);

  // optional hide scales
  A7Volts.showScale(false);
  //A7Volts.showTitle(false);
  // optional draw bars with segments (looks like a segment UV meter)
  A7Volts.useSegmentBars(true);
  // set colors for 3 color segments and 4th for null color
  A7Volts.setSectionColors(ILI9341_GREEN, ILI9341_YELLOW, ILI9341_RED, 0x2104);
  // how many bars for each section, where sections based on the scale
  A7Volts.setSectionSize(2.5, 3);
  // you should set the bars for the scale, number of bars, bar thickness, and gap between bars
  // I opted for manually defining the bars so multiple scales will look the same (as opposed to computing bar thickness)
  A7Volts.setBars(40, 5, 1);

  // initalize another for a different example

  A8Volts.init(V_XORIGIN, V_YORIGIN, V_WIDE, V_HEIGHT, V_LOWSCALE, V_HIGHSCALE, V_SCALEINC, "A8 Voltage", TEXT_COLOR, BORDER_COLOR, BAR_COLOR, 
  NULL_COLOR, BACK_COLOR, FONT_TITLE, FONT_DATA);
}

void loop() {

  // step 4 get some data
  a7Bits = analogRead(A7);
  a8Bits = analogRead(A8);

  a7Volts = a7Bits * 3.3 / 1024.0f;
  a8Volts = a8Bits * 3.3 / 1024.0f;

  // optional but shows how to change the color after init
  if (a8Volts > 0.87) {
    A8Volts.setBarColor(WARNING_COLOR);
  } else {
    A8Volts.setBarColor(BAR_COLOR);
  }

  // step 5 draw
  A7Volts.draw(a7Volts);
  A8Volts.draw(a8Volts);
  delay(50);
}
