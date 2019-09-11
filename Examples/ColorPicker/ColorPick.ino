/*

  Revision table
  ________________________________
  rev   author      date        desc


  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  font library              https://github.com/PaulStoffregen/ILI9341_fonts
  touchscreen lib           https://github.com/dgolda/UTouch


  connectivity map
  _______________________________

  Teensy 3.2
  A0        voltage divider input
  A1        amp meter pin
  A2
  A3
  A4
  A5
  A9        LCD on LCD to control
  1
  2         connect to LCD  touch section
  3         connect to LCD  touch section
  4         connect to LCD  touch section
  5         connect to LCD  touch section
  6         connect to LCD  touch section
  7         SD card chip select
  8         output to MOSFET switch
  9         DC for LCD
  10        CS to SD card
  11        MOSI to SD card
  12        MISO to SD card
  13        SCK to SD card
  SDA
  SLC

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include "UTouch.h"               // touchscreen lib
#include <ILI9341_t3_Controls.h>  // custom control define file
#include <PatriotRacing_Utilities.h>  // custom control define file


#define TFT_DC 9       // DC pin on LCD
#define TFT_CS 10      // chip select pin on LCD
#define LCD_PIN A9     // lcd pin to control brightness

#define ROW1 70
#define ROW2 110
#define ROW3 150
#define ROW4 190


// create display and DS objects
ILI9341_t3 Display = ILI9341_t3(10, 9);

// create the touch screen object
// UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch  Touch( 6, 5, 4, 3, 2);

SliderV Bright(&Display, 285, 70, 120, C_WHITE, C_BLACK, C_GREY);
SliderH Red(&Display, 20, ROW2, 100, C_WHITE, C_BLACK, C_RED);
SliderH Green(&Display,20, ROW3, 100, C_WHITE, C_BLACK, C_GREEN);
SliderH Blue(&Display,20, ROW4, 100, C_WHITE, C_BLACK, C_BLUE);

SliderOnOff Enable(&Display, 190, 20, 50, 30, C_WHITE, C_BLACK, C_GREEN, C_DKGREY);
SliderOnOff Lock(&Display, 70, 50, 50, 30, C_WHITE, C_BLACK, C_GREEN, C_DKGREY);

byte RedByte = 0, GreenByte = 0, BlueByte = 0;
byte BrightByte = 255;
int BtnX, BtnY;
bool enableSliders = true;
bool lockSliders = false;

void setup() {

  Serial.begin(9600);

  pinMode(LCD_PIN, OUTPUT);
  
  Bright.init(0, 255);
  Red.init(0, 255);
  Green.init(0, 255);
  Blue.init(0, 255);

  // fire up the display
  Display.begin();
  
  // fire up the touch display
  Touch.InitTouch(PORTRAIT);
  Touch.setPrecision(PREC_EXTREME);

  Display.invertDisplay(false);
  Display.fillScreen(C_BLACK);
  analogWrite(LCD_PIN, 255);
  Display.setRotation(1);
  Display.setTextSize(2);
  Display.setTextColor(C_WHITE, C_BLACK);

  Display.setTextColor(C_WHITE, C_BLACK);
  Display.setCursor(10 , 10 );
  Display.print(F("Color Picker"));

   Display.setCursor(10 , 55 );
  Display.print(F("Lock"));
  

  Bright.draw(BrightByte);
  Red.draw(RedByte);
  Green.draw(GreenByte);
  Blue.draw(BlueByte);
  Enable.draw(enableSliders);
  Lock.draw(lockSliders);

}


void loop() {

  if (Touch.dataAvailable()) {

    Touch.read();

    BtnX = Touch.getX();
    BtnY = Touch.getY();
    BtnX  = map(BtnX, 240, 0, 320, 0);
    BtnY  = map(BtnY, 379, 0, 240, 0);

    enableSliders = Enable.slide(BtnX, BtnY);
    lockSliders = Lock.slide(BtnX, BtnY);

    if (enableSliders) {
      BrightByte = Bright.slide(BtnX, BtnY);
    }

    RedByte = Red.slide(BtnX, BtnY);
    GreenByte = Green.slide(BtnX, BtnY);
    BlueByte = Blue.slide(BtnX, BtnY);

    if (lockSliders) {
      if (Red.changed()) {
        Green.draw(RedByte);
        Blue.draw(RedByte);
      }
      if (Green.changed()) {
        Red.draw(GreenByte);
        Blue.draw(GreenByte);
      }
      if (Blue.changed()) {
        Red.draw(BlueByte);
        Green.draw(BlueByte);
      }
    }

    if (BrightByte > 200) {
      Bright.setHandleColor(C_RED);
    }
    else {
      Bright.setHandleColor(C_GREY);
    }

    if (Enable.changed()) {
      if (enableSliders) {
        Bright.setColors(C_WHITE, C_BLACK, C_GREY);
        Bright.draw(BrightByte);
      }
      else {
        Bright.setColors(C_GREY, C_BLACK, C_DKGREY);
        Bright.draw(BrightByte);
      }
    }
  }


  Display.drawRect(140, 80, 100, 100, C_WHITE);
  Display.drawRect(141, 81, 98, 98, C_WHITE);
  Display.fillRect(142, 82, 96, 96, Display.color565(RedByte, GreenByte, BlueByte));

  analogWrite(LCD_PIN, BrightByte);

  Display.setCursor(140 , 200 );
  Display.print(RedByte); Display.print(F(",")); Display.print(GreenByte); Display.print(F(",")); Display.print(BlueByte);

}
