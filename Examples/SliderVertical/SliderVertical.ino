/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  touchscreen lib           https://github.com/dgolda/UTouch

    // required
  SliderV(ILI9341_t3 *Display); // class constructor
  void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderH, float ScaleLow, float ScaleHi, float scale, float snap, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);  // initializer
  void draw(float val);           // method to draw complete slider
  float slide(uint16_t ScreenX, uint16_t ScreenY);     // method to move handle as user drags finger over handle, 
  // automatically looks for a valid range press returns if control was clicked. use value property to get the selected option

  // optional
  void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor); // way to reset colors (useful for drawing enabled or disabled)
  void setHandleColor(uint16_t HandleColor);    // method to just draw the handle (useful for showing handle in green for OK value
  void setHandleSize(int val);
  void setHandleShape(byte val);
  void drawSliderColor(bool val);
  void setDisableColor(uint16_t HandleColor, uint16_t SliderColor); // method to just draw the handle (useful for showing handle in green for OK value
  void setScale(float ScaleLow, float ScaleHi, float scale = 0.0, float snap= 0.0);
  bool changed();
  void disable();
  void enable();
  void show();
  void hide();
  void setPressDebounce(byte Debounce);
  float value;

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include "UTouch.h"               // touchscreen lib
#include <ILI9341_t3_Controls.h>  // custom control define file
#include <Colors.h>

#define TFT_DC 9       // DC pin on LCD
#define TFT_CS 10      // chip select pin on LCD
#define LCD_PIN A9     // lcd pin to control brightness

#define ROW1 70
#define ROW2 110
#define ROW3 150
#define ROW4 190

#define BAND1 20
#define BAND2 60
#define BAND3 100
#define BAND4 140
#define BAND5 180
#define BAND6 220
#define BAND7 260
#define BAND8 300

#define SLIDECOLOR C_GREY
#define HANDLECOLOR C_ORANGE
#define BACKCOLOR C_BLACK
#define TEXTCOLOR C_WHITE
#define MINDB -12
#define MAXDB 12
#define TICK 2
#define SNAP 1

// note your HEIGHT / (max scale) should be an int
// otherwise you may get some draw errors
#define TOP 45
#define HEIGHT 168

// create some band variables
float Band1 = 4;
float Band2 = 3;
float Band3 = 2;
float Band4 = 1;
float Band5 = 0;
float Band6 = -1;
float Band7 = -2;
float Band8 = -3;

int BtnX, BtnY;

// create display and DS objects
ILI9341_t3 Display = ILI9341_t3(10, 9);

// create the touch screen object
// UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch  Touch( 6, 5, 4, 3, 2);

// create the slider objects
SliderV sBand1(&Display);
SliderV sBand2(&Display);
SliderV sBand3(&Display);
SliderV sBand4(&Display);
SliderV sBand5(&Display);
SliderV sBand6(&Display);
SliderV sBand7(&Display);
SliderV sBand8(&Display);

void setup() {

  Serial.begin(9600);
  pinMode(LCD_PIN, OUTPUT);

  // initialize the sliders
  sBand1.init(BAND1, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand2.init(BAND2, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand3.init(BAND3, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand4.init(BAND4, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand5.init(BAND5, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand6.init(BAND6, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand7.init(BAND7, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);
  sBand8.init(BAND8, TOP, HEIGHT, MINDB, MAXDB, TICK, SNAP, SLIDECOLOR, BACKCOLOR, HANDLECOLOR);



  // fire up the display
  Display.begin();

  // fire up the touch display
  Touch.InitTouch(PORTRAIT);
  Touch.setPrecision(PREC_EXTREME);
  Display.invertDisplay(false);
  Display.fillScreen(BACKCOLOR);
  Display.setRotation(1);

  // turn the brightness up
  analogWrite(LCD_PIN, 255);

  // draw the slider controls with initial settings
  sBand1.draw(Band1);
  sBand2.draw(Band2);
  sBand3.draw(Band3);
  sBand4.draw(Band4);
  sBand5.draw(Band5);
  sBand6.draw(Band6);
  sBand7.draw(Band7);
  sBand8.draw(Band8);

  Display.setTextColor(TEXTCOLOR, BACKCOLOR);
  Display.setTextSize(2);
  Display.setCursor(10 , 10 );
  Display.print(F("Equalizer"));
  Display.setTextSize(1);

  Display.setCursor(BAND1 , 30 ); Display.print(Band1, 0);
  Display.setCursor(BAND2 , 30 ); Display.print(Band2, 0);
  Display.setCursor(BAND3 , 30 ); Display.print(Band3, 0);
  Display.setCursor(BAND4 , 30 ); Display.print(Band4, 0);
  Display.setCursor(BAND5 , 30 ); Display.print(Band5, 0);
  Display.setCursor(BAND6 , 30 ); Display.print(Band6, 0);
  Display.setCursor(BAND7 , 30 ); Display.print(Band7, 0);
  Display.setCursor(BAND8 , 30 ); Display.print(Band8, 0);

  Display.setCursor(BAND1 , 220 ); Display.print(F("63"));
  Display.setCursor(BAND2 , 220 ); Display.print(F("125"));
  Display.setCursor(BAND3 , 220 ); Display.print(F("250"));
  Display.setCursor(BAND4 , 220 ); Display.print(F("500"));
  Display.setCursor(BAND5 , 220 ); Display.print(F("1K"));
  Display.setCursor(BAND6 , 220 ); Display.print(F("2K"));
  Display.setCursor(BAND7 , 220 ); Display.print(F("4K"));
  Display.setCursor(BAND8 , 220 ); Display.print(F("8K"));

}


void loop() {

  if (Touch.dataAvailable()) {

    ProcessTouch();


    // if you really need to monitor if the control was clicked
    // here's how, this may be needed if you have a complicated UI
    // and don't need to automatically change things--code here shows 2 cases
    // 1) automatically updates items based on slider (see if else sections, these
    // run even if slider was not pressed
    // and 2) only updating items if the slider was changed
    // do not use the return value as slider value
    // only use SliderV.value for the value
    
    bool changed = sBand1.slide(BtnX, BtnY);
    sBand2.slide(BtnX, BtnY);
    sBand3.slide(BtnX, BtnY);
    sBand4.slide(BtnX, BtnY);
    sBand5.slide(BtnX, BtnY);
    sBand6.slide(BtnX, BtnY);
    sBand7.slide(BtnX, BtnY);
    sBand8.slide(BtnX, BtnY);

    // as mentioned above, you you don't want to process code unless slider was moved
    // wrap a change test around checkbox values
    // here only the band 1 screen text updates
    if (changed) {
      Display.fillRect(BAND1, 28, BAND2 - BAND1, 10, BACKCOLOR);
      Display.setCursor(BAND1 , 30 ); Display.print(sBand1.value, 0);
    }

    // as mentioned above, you code processes regardless if clider was moved
    // all remaining band values will redraw (and cause a slight flicker
    // if you hate flicker, add a change test around each one
    Display.fillRect(BAND2, 28, BAND3 - BAND2, 10, BACKCOLOR);
    Display.setCursor(BAND2 , 30 ); Display.print(sBand2.value, 0);

    Display.fillRect(BAND3, 28, BAND4 - BAND3, 10, BACKCOLOR);
    Display.setCursor(BAND3 , 30 ); Display.print(sBand3.value, 0);

    Display.fillRect(BAND4, 28, BAND5 - BAND4, 10, BACKCOLOR);
    Display.setCursor(BAND4 , 30 ); Display.print(sBand4.value, 0);

    Display.fillRect(BAND5, 28, BAND6 - BAND5, 10, BACKCOLOR);
    Display.setCursor(BAND5 , 30 ); Display.print(sBand5.value, 0);

    Display.fillRect(BAND6, 28, BAND7 - BAND6, 10, BACKCOLOR);
    Display.setCursor(BAND6 , 30 ); Display.print(sBand6.value, 0);

    Display.fillRect(BAND7, 28, BAND8 - BAND7, 10, BACKCOLOR);
    Display.setCursor(BAND7 , 30 ); Display.print(sBand7.value, 0);

    Display.fillRect(BAND8, 28, 320 - BAND8, 10, BACKCOLOR);
    Display.setCursor(BAND8 , 30 ); Display.print(sBand8.value, 0);


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
