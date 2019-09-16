/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  touchscreen lib           https://github.com/dgolda/UTouch

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include "UTouch.h"               // touchscreen lib
#include <ILI9341_t3_Controls.h>  // custom control define file

#define TFT_DC 9       // DC pin on LCD
#define TFT_CS 10      // chip select pin on LCD
#define LCD_PIN A9     // lcd pin to control brightness

#define ROW1 70
#define ROW2 110
#define ROW3 150
#define ROW4 190

#define C_BLACK       0x0000
#define C_BLUE        0x001F
#define C_RED         0xF800
#define C_GREEN       0x07E0
#define C_CYAN        0x07FF
#define C_MAGENTA     0xF81F
#define C_YELLOW      0xFFE0
#define C_WHITE       0xFFFF
#define C_LTBLUE      0xB6DF
#define C_LTTEAL      0xBF5F
#define C_LTGREEN     0xBFF7
#define C_LTCYAN      0xC7FF
#define C_LTRED       0xFD34
#define C_LTMAGENTA   0xFD5F
#define C_LTYELLOW    0xFFF8
#define C_LTORANGE    0xFE73
#define C_LTPINK      0xFDDF
#define C_LTPURPLE    0xCCFF
#define C_LTGREY      0xE71C
#define C_TEAL        0x0438
#define C_ORANGE      0xFD20
#define C_PINK        0xF81F
#define C_PURPLE      0x801F
#define C_GREY        0xC618
#define C_DKBLUE      0x0069
#define C_DKTEAL      0x020C
#define C_DKGREEN     0x03E0
#define C_DKCYAN      0x03EF
#define C_DKRED       0x6000
#define C_DKMAGENTA   0x8008
#define C_DKYELLOW    0x8400
#define C_DKORANGE    0x8200
#define C_DKPINK      0x9009
#define C_DKPURPLE    0x4010
#define C_DKGREY      0x4A49

byte RedByte = 70, GreenByte = 170, BlueByte = 210;
byte BrightByte = 255;
int BtnX, BtnY;
bool enableSliders = true;
bool lockSliders = false;
char buf[14];

// create display and DS objects
ILI9341_t3 Display = ILI9341_t3(10, 9);

// create the touch screen object
// UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch  Touch( 6, 5, 4, 3, 2);

SliderV Bright(&Display, 285, 70, 120, C_WHITE, C_BLACK, C_GREY);
SliderH Red(&Display, 20, ROW2, 100, C_WHITE, C_BLACK, C_RED);
SliderH Green(&Display, 20, ROW3, 100, C_WHITE, C_BLACK, C_GREEN);
SliderH Blue(&Display, 20, ROW4, 100, C_WHITE, C_BLACK, C_BLUE);

SliderOnOff Enable(&Display, 190, 20, 50, 30, C_WHITE, C_BLACK, C_GREEN, C_DKGREY);
SliderOnOff Lock(&Display, 70, 50, 50, 30, C_WHITE, C_BLACK, C_GREEN, C_DKGREY);

void setup() {

  Serial.begin(9600);

  pinMode(LCD_PIN, OUTPUT);

  Bright.init (  55, 255, 0, 0);
  Red.init    ( 0, 255, 127.5, 1);
  Green.init  ( 0, 255, 63.75, 1);
  Blue.init   ( 0, 255, 0, 1);

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

  Red.drawSliderColor(true);
  Green.drawSliderColor(true);
  Blue.drawSliderColor(true);

  Bright.draw(BrightByte);
  Red.draw(RedByte);
  Green.draw(GreenByte);
  Blue.draw(BlueByte);
  Enable.draw(enableSliders);
  Lock.draw(lockSliders);

  Display.drawRect(140, 80, 100, 100, C_WHITE);
  Display.drawRect(141, 81, 98, 98, C_WHITE);
  Display.fillRect(142, 82, 96, 96, Display.color565(RedByte, GreenByte, BlueByte));

  sprintf(buf, "%3d,%3d,%3d", RedByte, GreenByte, BlueByte);
  Display.setCursor(140 , 200 );
  Display.print(buf);

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

    if (Lock.changed()) {
      if (lockSliders) {
        Red.drawSliderColor(false);
        Green.drawSliderColor(false);
        Blue.drawSliderColor(false);
        Red.draw(RedByte);
        Green.draw(RedByte);
        Blue.draw(RedByte);
      }
      else {
        Red.drawSliderColor(true);
        Green.drawSliderColor(true);
        Blue.drawSliderColor(true);
        Red.draw(RedByte);
        Green.draw(RedByte);
        Blue.draw(RedByte);
      }
    }

    if (lockSliders) {
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
  sprintf(buf, "%3d,%3d,%3d", RedByte, GreenByte, BlueByte);
  Display.setCursor(140 , 200 );
  Display.print(buf);

}
