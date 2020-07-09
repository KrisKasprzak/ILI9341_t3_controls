/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  touchscreen lib           https://github.com/dgolda/UTouch

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include "UTouch.h"               // touchscreen lib
#include <ILI9341_t3_Controls.h>  // custom control define file
#define TFT_DC 9                  // DC pin on LCD
#define TFT_CS 10                 // chip select pin on LCD
#define LCD_PIN A9                // lcd pin to control brightness

// defines for locations
#define ROW1 70
#define ROW2 110
#define ROW3 150
#define ROW4 190

byte RedByte = 70, GreenByte = 170, BlueByte = 210;
byte BrightByte = 255;
int BtnX, BtnY;
bool lockSliders = false;
char buf[22];
uint16_t color;

// create display and DS objects
ILI9341_t3 Display = ILI9341_t3(10, 9);

// create the touch screen object
// UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch  Touch( 6, 5, 4, 3, 2);

// SliderV Bright(&Display, Left, bottom, height, bar color, back color, ball color);
SliderV Bright(&Display, 285, 70, 120, C_WHITE, C_BLACK, C_GREY);

//SliderH Red(&Display, left, top, width, bar color, back color, ball color);
SliderH Red(&Display, 20, ROW2, 100, C_WHITE, C_BLACK, C_RED);
SliderH Green(&Display, 20, ROW3, 100, C_WHITE, C_BLACK, C_GREEN);
SliderH Blue(&Display, 20, ROW4, 100, C_WHITE, C_BLACK, C_BLUE);

// SliderOnOff Enable(&Display, left, top, width, height, line color, back color, on color, off color);
SliderOnOff Lock(&Display, 70, 50, 50, 30, C_WHITE, C_BLACK, C_GREEN, C_DKGREY);

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
  Bright.init ( 55, 255,     0, 0);
  Red.init    (  0, 255, 127.5, 1);
  Green.init  (  0, 255, 63.75, 1);
  Blue.init   (  0, 255,     0, 0);

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

  Display.setTextColor(C_WHITE, C_BLACK);
  Display.setCursor(10 , 10 );
  Display.print(F("Color Picker"));

  Display.setCursor(10 , 55 );
  Display.print(F("Lock"));

  // option to show the slider bar in a color
  Red.drawSliderColor(false);
  Green.drawSliderColor(true);
  Blue.drawSliderColor(true);

  // mandatory call to draw the slider with initial setting
  // hint: store value in EERPOM for persisting settings
  Bright.draw(BrightByte);
  Red.draw(RedByte);
  Green.draw(GreenByte);
  Blue.draw(BlueByte);

  // draw some on / off buttons
  Lock.draw(lockSliders);
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

    Touch.read();

    BtnX = Touch.getX();
    BtnY = Touch.getY();

    // this is a mess... displays act different and return different values depending on where touch happened
    //Serial.print("real coordinates: ");
    //Serial.print(BtnX);
    //Serial.print(",");
    //Serial.println (BtnY);
    //Display.drawPixel(BtnX, BtnY, C_GREEN);




    //Serial.print("mapped coordinates: ");
    //Serial.print(BtnX);
    //Serial.print(",");
    //Serial.println (BtnY);
    //Display.drawPixel(BtnX, BtnY, C_RED);

    // these displays are a mess... displays act different and return
    //different values depending on where touch happened

    // x  = map(x, real left, real right, 0, 480);
    // y  = map(y, real bottom, real top, 320, 0);

    // tft with yellow headers
    //BtnX  = map(BtnX, 240, 0, 320, 0);
    //BtnY  = map(BtnY, 379, 0, 240, 0);

    // tht with black headers
    BtnX  = map(BtnX, 0, 240, 320, 0);
    BtnY  = map(BtnY, 0, 380, 240, 0);

    // process touch values
    // how to enable disable a slider
    // example on how to change slider colors


    BrightByte = Bright.slide(BtnX, BtnY);

    // example on how to change the handle color
    if (BrightByte > 200) {
      Bright.setHandleColor(C_RED);
    }
    else {
      Bright.setHandleColor(C_GREY);
    }

    lockSliders = Lock.slide(BtnX, BtnY);

    // note if touch is not in range of slider, operation will be ignored
    // meaning you don't need to test press location--the lib does that
    RedByte = Red.slide(BtnX, BtnY);
    GreenByte = Green.slide(BtnX, BtnY);
    BlueByte = Blue.slide(BtnX, BtnY);

    // examples of how to lock things, enable, disable stuff
    if (Lock.getValue()) {
      if (Red.changed()) {
        Red.draw(RedByte);
        Green.draw(RedByte);
        Blue.draw(RedByte);
      }
      if (Green.changed()) {
        Red.draw(GreenByte);
        Green.draw(GreenByte);
        Blue.draw(GreenByte);
      }
      if (Blue.changed()) {
        Red.draw(BlueByte);
        Green.draw(BlueByte);
        Blue.draw(BlueByte);
      }
    }

  }
  // update the color swatch

  color = Display.color565(RedByte, GreenByte, BlueByte);
  
  Display.drawRect(140, 80, 100, 100, C_WHITE);
  Display.drawRect(141, 81, 98, 98, C_WHITE);
  Display.fillRect(142, 82, 96, 96, color);

  // set the display brightness
  analogWrite(LCD_PIN, BrightByte);
  sprintf(buf, "(%3d),%3d,%3d,%3d", color, RedByte, GreenByte, BlueByte);
  Display.setCursor(50 , 220 );
  Display.print(buf);

}
