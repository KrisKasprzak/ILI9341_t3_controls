// #define DEBUG

#include <ILI9341_t3.h>           // fast display driver lib
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>           // custom fonts that ships with ILI9341_t3.h
#include <font_ArialBold.h>
#include "UTouch.h"

// you must create and pass fonts to the function
#define FONT_TEXT Arial_24
#define FONT_LBUTTON Arial_16
#define FONT_SBUTTON Arial_12_Bold

#define LONG_PRESS_DURATION 1000
#define NO_PRESS 0
#define SHORT_PRESS 1
#define LONG_PRESS 2


// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

int BtnX, BtnY, ct = -1;
bool rs = true, st = true;
char buf[1];
// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

UTouch  Touch( 2, 3, 4, 5, 6);

// create the cartesian coordinate graph object
Button Button1;
Button Button2;
Button Button3;
Button Button4;
Button Button5;

void setup() {

  Serial.begin(57600);

  // fire up the display
  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(C_BLACK);

  // fire up the touch display
  Touch.InitTouch(LANDSCAPE);
  Touch.setPrecision(PREC_EXTREME);

  Button1.init(&Display, 60, 20, 100, 40, C_BLUE, C_WHITE, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY, "Press", 2, 2, FONT_SBUTTON ) ;
  Button2.init(&Display, 60, 80, 100, 40, C_BLUE, C_WHITE, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY, "Disable", 1, 2, FONT_SBUTTON ) ;
  Button3.init(&Display, 180, 80, 100, 40, C_BLUE, C_WHITE, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY, "Radius", 1, 2, FONT_SBUTTON ) ;
  Button4.init(&Display, 60, 140, 100, 40, C_BLUE, C_WHITE, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY, "Move", 2, 2, FONT_SBUTTON ) ;
  Button5.init(&Display, 180, 140, 100, 40, C_BLUE, C_GREEN, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY, "ON", 3, 2, FONT_LBUTTON ) ;

  Button1.draw();
  Button2.draw();
  Button3.draw();
  Button4.draw();
  Button5.draw();

  // optional button methods
  // show()
  // hide()
  // resize()
  // setColors()
  // setFont()
  // setText()
  // setCornerRadius()
  // disable()
  // enable()
  // enabled() // returns state
  // press() // sets if pressed (mainly used in HandleButton

}

void loop() {

  if (Touch.dataAvailable()) {
    ProcessTouch();
    if (HandlePress(Button1) == SHORT_PRESS) {
      Display.setFont(FONT_TEXT);
      Display.setTextColor(C_WHITE);
      Display.setCursor(150, 5);
      Display.print("Pressed");
      delay(500);
      Display.fillRect(150, 5, 200, 40, C_BLACK);
    }
    else if (HandlePress(Button2) == SHORT_PRESS) {
      if (Button3.value == 0) {
        Button3.value = 1;
        Button3.disable();
      }
      else {
        Button3.value = 0;
        Button3.enable();
      }
    }

    if (HandlePress(Button3) == SHORT_PRESS) {
      ct++;
      if (ct > 20) {
        ct = -1;
      }
      // -1 = auto size
      // 0 = sharp
      // > 0 is radius size
      if (ct == -1) {
        Button3.setText("r=auto");
        Button3.setCornerRadius(ct);

      }
      else {
        sprintf(buf, "r=%d", ct);
        Button3.setText(buf);
        Button3.setCornerRadius(ct);
      }
    }

    if (HandlePress(Button4) == SHORT_PRESS) {
      if (rs) {
        rs = false;
        Button5.resize(180, 160, 100, 40);
      }
      else {
        rs = true;
        Button5.resize(180, 140, 100, 40);
      }

    }
    if (HandlePress(Button5) == LONG_PRESS) {
      if (st) {
        st = false;
        Button5.setText("Off");
        Button5.setColors(C_LTGREY, C_RED, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY);
      }
      else {
        st = true;
        Button5.setText("On");
        Button5.setColors(C_LTGREY, C_GREEN, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY);
      }
    }
  }
}
void ProcessTouch() {

  // depending on the touch library you may need to change methods here
  Touch.read();

  // these diaplays are all different
  // add #ifdef DEBUG to display locations
  BtnY = Touch.getX();
  BtnX = Touch.getY();
  // or....
  //BtnX = Touch.getX();
  //BtnY = Touch.getY();

#ifdef DEBUG
  Serial.print("real coordinates: ");
  Serial.print(BtnX);
  Serial.print(",");
  Serial.print (BtnY);
  Display.drawPixel(BtnX, BtnY, C_RED);
#endif

  // you may need to reverse the arguments if touch tracks opposite to press location
  // x  = map(x, real left, real right, 0, 480);
  // y  = map(y, real bottom, real top, 320, 0);
  // x  = map(y, 320, 0, 0, 320);
  // y  = map(x, 240, 0, 0, 240);

  // displays with yellow headers
  BtnX  = map(BtnX, 240, 0, 320, 0);
  BtnY  = map(BtnY, 0, 380, 240, 0);

  // displays with black headers
  // BtnX  = map(BtnX, 0, 240, 320, 0);
  // BtnY  = map(BtnY, 0, 380, 240, 0);

#ifdef DEBUG
  Serial.print(", Mapped coordinates: ");
  Serial.print(BtnX);
  Serial.print(",");
  Serial.println(BtnY);
  Display.drawPixel(BtnX, BtnY, C_GREEN);
#endif

}

byte HandlePress(Button TheButton) {

  unsigned long pt;
  byte val = NO_PRESS;

  // fancy function to handle button press and display according to press
  // supports short press and long press
  if (TheButton.contains(BtnX, BtnY)) {
    TheButton.draw(true);
    TheButton.press(true);
    pt = millis();
    while (Touch.dataAvailable()) {
      // you will need to have global x,y screen press location variables
      // you could modify and pass in as arguements
      ProcessTouch();
      if (TheButton.contains(BtnX, BtnY)) {
        if (TheButton.isPressed() == false) {
          TheButton.draw(true);
          TheButton.press(true);
        }

        if ((millis() - pt) >= LONG_PRESS_DURATION) {
          // heck they have been pressing it so
          // bail of long press
          TheButton.press(false);
          TheButton.draw(false);
          return LONG_PRESS;
        }
      }
      else {
        if (TheButton.isPressed() == true) {
          TheButton.draw(false);
          TheButton.press(false);
        }
      }
      delay(100);
    }

    TheButton.press(false);
    TheButton.draw(false);
    if (TheButton.contains(BtnX, BtnY))  {
      // button released while on button
      val = SHORT_PRESS;
    }
    else  {
      // button released off of button
      val = NO_PRESS;
    }
  }

  return val;
}
