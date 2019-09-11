/*

  Revision table
  ________________________________
  rev   author      date        desc


  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  font library              https://github.com/PaulStoffregen/ILI9341_fonts
  touchscreen lib           https://github.com/dgolda/UTouch

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include "UTouch.h"               // touchscreen lib
#include <ILI9341_t3_Controls.h>  // custom control define file

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

// create some band variables
int Band1 = 0;
int Band2 = 0;
int Band3 = 0;
int Band4 = 0;
int Band5 = 0;
int Band6 = 0;
int Band7 = 0;
int Band8 = 0;

int BtnX, BtnY;

// create display and DS objects
ILI9341_t3 Display = ILI9341_t3(10, 9);

// create the touch screen object
// UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch  Touch( 6, 5, 4, 3, 2);

// create the slider objects
SliderV sBand1(&Display, BAND1, 45, 160, C_WHITE, C_BLACK, C_ORANGE);
SliderV sBand2(&Display, BAND2, 45, 160, C_WHITE, C_BLACK, C_ORANGE);
SliderV sBand3(&Display, BAND3, 45, 160, C_WHITE, C_BLACK, C_ORANGE);
SliderV sBand4(&Display, BAND4, 45, 160, C_WHITE, C_BLACK, C_ORANGE);
SliderV sBand5(&Display, BAND5, 45, 160, C_WHITE, C_BLACK, C_ORANGE);
SliderV sBand6(&Display, BAND6, 45, 160, C_WHITE, C_BLACK, C_ORANGE);
SliderV sBand7(&Display, BAND7, 45, 160, C_WHITE, C_BLACK, C_ORANGE);
SliderV sBand8(&Display, BAND8, 45, 160, C_WHITE, C_BLACK, C_ORANGE);

void setup() {

  Serial.begin(9600);
  pinMode(LCD_PIN, OUTPUT);

  // initialize the sliders
  sBand1.init(-12, 12, 2, 1);
  sBand2.init(-12, 12, 2, 1);
  sBand3.init(-12, 12, 2, 1);
  sBand4.init(-12, 12, 2, 1);
  sBand5.init(-12, 12, 2, 1);
  sBand6.init(-12, 12, 2, 1);
  sBand7.init(-12, 12, 2, 1);
  sBand8.init(-12, 12, 2, 1);

  // fire up the display
  Display.begin();

  // fire up the touch display
  Touch.InitTouch(PORTRAIT);
  Touch.setPrecision(PREC_EXTREME);
  Display.invertDisplay(false);
  Display.fillScreen(C_BLACK);
  Display.setRotation(1);

  // turn the brightness up
  analogWrite(LCD_PIN, 255);

  // draw the slider controls
  sBand1.draw(Band1);
  sBand2.draw(Band2);
  sBand3.draw(Band3);
  sBand4.draw(Band4);
  sBand5.draw(Band5);
  sBand6.draw(Band6);
  sBand7.draw(Band7);
  sBand8.draw(Band8);

  Display.setTextColor(C_WHITE, C_BLACK);
  Display.setTextSize(2);
  Display.setCursor(10 , 10 );
  Display.print(F("Equalizer"));
  Display.setTextSize(1);


  Display.setCursor(BAND1 , 30 ); Display.print(Band1);
  Display.setCursor(BAND2 , 30 ); Display.print(Band2);
  Display.setCursor(BAND3 , 30 ); Display.print(Band3);
  Display.setCursor(BAND4 , 30 ); Display.print(Band4);
  Display.setCursor(BAND5 , 30 ); Display.print(Band5);
  Display.setCursor(BAND6 , 30 ); Display.print(Band6);
  Display.setCursor(BAND7 , 30 ); Display.print(Band7);
  Display.setCursor(BAND8 , 30 ); Display.print(Band8);

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

    Touch.read();

    BtnX = Touch.getX();
    BtnY = Touch.getY();
    BtnX  = map(BtnX, 240, 0, 320, 0);
    BtnY  = map(BtnY, 379, 0, 240, 0);

    Band1 = sBand1.slide(BtnX, BtnY);
    Band2 = sBand2.slide(BtnX, BtnY);
    Band3 = sBand3.slide(BtnX, BtnY);
    Band4 = sBand4.slide(BtnX, BtnY);
    Band5 = sBand5.slide(BtnX, BtnY);
    Band6 = sBand6.slide(BtnX, BtnY);
    Band7 = sBand7.slide(BtnX, BtnY);
    Band8 = sBand8.slide(BtnX, BtnY);


    if (sBand1.changed()) {
      Display.fillRect(BAND1, 28, BAND2 - BAND1, 10, C_BLACK);
      Display.setCursor(BAND1 , 30 ); Display.print(Band1);
    }
    if (sBand2.changed()) {
      Display.fillRect(BAND2, 28, BAND3 - BAND2, 10, C_BLACK);
      Display.setCursor(BAND2 , 30 ); Display.print(Band2);
    }
    if (sBand3.changed()) {
      Display.fillRect(BAND3, 28, BAND4 - BAND3, 10, C_BLACK);
      Display.setCursor(BAND3 , 30 ); Display.print(Band3);
    }
    if (sBand4.changed()) {
      Display.fillRect(BAND4, 28, BAND5 - BAND4, 10, C_BLACK);
      Display.setCursor(BAND4 , 30 ); Display.print(Band4);
    }
    if (sBand5.changed()) {
      Display.fillRect(BAND5, 28, BAND6 - BAND5, 10, C_BLACK);
      Display.setCursor(BAND5 , 30 ); Display.print(Band5);
    }
    if (sBand6.changed()) {
      Display.fillRect(BAND6, 28, BAND7 - BAND6, 10, C_BLACK);
      Display.setCursor(BAND6 , 30 ); Display.print(Band6);
    }
    if (sBand7.changed()) {
      Display.fillRect(BAND7, 28, BAND8 - BAND7, 10, C_BLACK);
      Display.setCursor(BAND7 , 30 ); Display.print(Band7);
    }
    if (sBand8.changed()) {
      Display.fillRect(BAND8, 28, 320 - BAND8, 10, C_BLACK);
      Display.setCursor(BAND8 , 30 ); Display.print(Band8);
    }


    Serial.println("___________________________");
    Serial.print("Band 1 "); Serial.println(Band1);
    Serial.print("Band 2 "); Serial.println(Band2);
    Serial.print("Band 3 "); Serial.println(Band3);
    Serial.print("Band 4 "); Serial.println(Band4);
    Serial.print("Band 5 "); Serial.println(Band5);
    Serial.print("Band 6 "); Serial.println(Band6);
    Serial.print("Band 7 "); Serial.println(Band7);
    Serial.print("Band 8 "); Serial.println(Band8);

  }
}
