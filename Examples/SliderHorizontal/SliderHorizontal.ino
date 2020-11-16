/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  touchscreen lib           https://github.com/dgolda/UTouch

    available methods

    // required
  SliderH(ILI9341_t3 *Display);   // class constructor
  void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, float ScaleLow, float ScaleHi, float Scale, float Snap, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);    // initializer
  void draw(float val);         // method to draw complete slider
  float slide(float ScreenX, float ScreenY);      // method to move handle as user drags finger over handle, this method automatically looks for a valid range press

    // optional
  bool changed();
  void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);   // way to reset colors (useful for drawing enabled or disabled)
  void setHandleColor(uint16_t HandleColor);  // method to just draw the handle (useful for showing handle in green for OK value
  void setDisableColor(uint16_t HandleColor, uint16_t SliderColor); // method to just draw the handle (useful for showing handle in green for OK value
  void setHandleSize(int size);
  void disable();
  void enable();
  void show();
  void hide();
  void setHandleShape(byte shape);
  void drawSliderColor(bool color);
  void setPressDebounce(byte Debounce);
  float value;

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include "UTouch.h"               // touchscreen lib
#include <ILI9341_t3_Controls.h>  // custom control define file
#include <font_Arial.h>

#define FONT Arial_16
#define TFT_DC 9                  // DC pin on LCD
#define TFT_CS 10                 // chip select pin on LCD
#define LCD_PIN A9                // lcd pin to control brightness

// defines for locations
#define ROW0 30
#define ROW1 70
#define ROW2 110
#define ROW3 150
#define ROW4 190

byte RedByte = 70, GreenByte = 170, BlueByte = 210, BrightByte = 255, oldBrightByte = 255;
int BtnX, BtnY;

char buf[22];
uint16_t color, oldcolor;

// create display and DS objects
ILI9341_t3 Display = ILI9341_t3(10, 9);

// create the touch screen object
// UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch  Touch( 6, 5, 4, 3, 2);

//SliderH Red(&Display, left, top, width, bar color, back color, ball color);
SliderH Bright(&Display);
SliderH Red(&Display);
SliderH Green(&Display);
SliderH Blue(&Display);


void setup() {

  Serial.begin(9600);

  //you know the drill
  pinMode(LCD_PIN, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  // initialize each slider
  // low limit, high limit, scale val (0 is off), snap value (0 is off))
  Bright.init (20, ROW1, 100, 55, 255,     0, 0, C_WHITE, C_BLACK, C_GREY);
  
  Red.init    (20, ROW2, 100, 0, 255, 127.5, 0, C_WHITE, C_BLACK, C_GREEN);
  Green.init  (20, ROW3, 100, 0, 255, 63.75, 0, C_WHITE, C_BLACK, C_BLUE);
  Blue.init   (20, ROW4, 100, 0, 255,     0, 0, C_WHITE, C_BLACK, C_RED);

  // fire up the display
  Display.begin();

  // fire up the touch display
  Touch.InitTouch(PORTRAIT);
  Touch.setPrecision(PREC_EXTREME);

  Display.invertDisplay(false);
  Display.fillScreen(C_BLACK);

  // turn on the display
  analogWrite(LCD_PIN, 255);

  // set some display settings
  Display.setRotation(1);
  Display.setTextSize(2);
  Display.setTextColor(C_WHITE, C_BLACK);
  Display.setFont(FONT);
  Display.setCursor(10 , 10 );
  Display.print(F("Color Picker"));

  Red.draw(RedByte);
  Green.draw(GreenByte);
  Blue.draw(BlueByte);
  Bright.draw(BrightByte);

  color = Display.color565(RedByte, GreenByte, BlueByte);

  // draw a color box
  Display.drawRect(140, 80, 100, 100, C_WHITE);
  Display.drawRect(141, 81, 98, 98, C_WHITE);
  Display.fillRect(142, 82, 96, 96, color);

  sprintf(buf, "(%3d),%3d,%3d,%3d", color, RedByte, GreenByte, BlueByte);
  Display.setCursor(50 , 220 );
  Display.print(buf);

}


void loop() {

  // wait for touch
  if (Touch.dataAvailable()) {
    ProcessTouch();

    // how to enable disable a slider
    // example on how to change slider colors


    BrightByte = Bright.slide(BtnX, BtnY);
    // note if touch is not in range of slider, operation will be ignored
    // meaning you don't need to test press location--the lib does that
    RedByte = Red.slide(BtnX, BtnY);
    GreenByte = Green.slide(BtnX, BtnY);
    BlueByte = Blue.slide(BtnX, BtnY);
  }
  // update the color swatch

  color = Display.color565(RedByte, GreenByte, BlueByte);

  if (color != oldcolor) {
    oldcolor = color;
    Display.drawRect(140, 80, 100, 100, C_WHITE);
    Display.drawRect(141, 81, 98, 98, C_WHITE);
    Display.fillRect(142, 82, 96, 96, color);    
    Display.fillRect(50, 220, 200, 50, C_BLACK);
    sprintf(buf, "(%3d),%3d,%3d,%3d", color, RedByte, GreenByte, BlueByte);
    Display.setCursor(50 , 220 );
    Display.print(buf);
  }
  if (BrightByte != oldBrightByte) {
    oldBrightByte = BrightByte;
    // set the display brightness
    analogWrite(LCD_PIN, BrightByte);

  }

}

void ProcessTouch() {

  // depending on the touch library you may need to change methods here
  Touch.read();

  BtnX = Touch.getX();
  BtnY = Touch.getY();

  // consistency between displays is a mess...
  // this is some debug code to help show
  // where you pressed and the resulting map

  //Serial.print("real coordinates: ");
  //Serial.print(BtnX);
  //Serial.print(",");
  //Serial.println (BtnY);
  // Display.drawPixel(BtnX, BtnY, C_RED);

  //different values depending on where touch happened

  // x  = map(x, real left, real right, 0, 480);
  // y  = map(y, real bottom, real top, 320, 0);

  // tft with yellow headers
  //BtnX  = map(BtnX, 240, 0, 320, 0);
  //BtnY  = map(BtnY, 379, 0, 240, 0);

  // tft with black headers
  BtnX  = map(BtnX, 0, 240, 320, 0);
  BtnY  = map(BtnY, 0, 380, 240, 0);

  //Serial.print(", Mapped coordinates: ");
  //Serial.print(BtnX);
  //Serial.print(",");
  //Serial.println(BtnY);
  //Display.drawPixel(BtnX, BtnY, C_GREEN);

}