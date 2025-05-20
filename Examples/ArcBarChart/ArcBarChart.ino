#include <ILI9341_t3.h>
// include the library
#include <ILI9341_t3_Controls.h>

#define TFT_CS 9
#define TFT_DC 2

// create display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

// create arc bar chart objects, don't forget the & as we are passing in a pointer to the display object
BarChartA BarGraphBits(&Display);
BarChartA BarGraphVolts(&Display);

int a7Bits;
float a7Volts;

void setup() {

  Serial.begin(9600);

  Display.begin();
  Display.setRotation(3);
  Display.fillScreen(ILI9341_BLACK);

  // default settings, scale is 0-1024
  BarGraphBits.init(800, 220, 30, 20, 10, 20, 0.0f, 1024.0f);

  // overrides
  BarGraphVolts.init(60, 80, 20, 20, 20, 40, 0.0f, 3.3f);
  // make a 270 degree arc, 15 segments, 15 pixels wide,  and 0.06 radian gap between segments
  BarGraphVolts.setBars(270, 15, 15, .06);
  // change colors for low medium high and null bar colors
  BarGraphVolts.setSectionColors(ILI9341_BLUE, ILI9341_WHITE, ILI9341_RED, 0x3186);
  // set low 0-33%, med 33-66%, and red above 66%
  BarGraphVolts.setSectionSize(.33, .66);
}

void loop() {

  // take some readings, temp, speed, etc.
  a7Bits = analogRead(A7);

  // convert bits as needed
  a7Volts = a7Bits * 3.3f / 1024.0f;

  // simply draw the data, make sure you bounds check
  BarGraphBits.draw(a7Bits);
  BarGraphVolts.draw(a7Volts);

  delay(10);
}