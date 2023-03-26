#include <ILI9341_t3.h>             //https://github.com/PaulStoffregen/ILI9341_t3
#include "ILI9341_t3_Controls.h"    //https://github.com/KrisKasprzak/ILI9341_t3_controls
#include <SPI.h>
#include <XPT2046_Touchscreen.h>        // https://github.com/PaulStoffregen/XPT2046_Touchscreen
#define LED_PIN 2 // i've mapped my LED pin to a digital pin

int BtnX, BtnY;  // holders for screen coordinate drawing

ILI9341_t3 Display(10, 9);

SliderD Vol(&Display);
SliderD Mid(&Display);
SliderD Base(&Display);
SliderD Treb(&Display);
// create the // Touch screen object
XPT2046_Touchscreen Touch(0, 1);

TS_Point p;

void setup() {

  Serial.begin(38400);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);


  Display.begin();
  Display.setRotation(1);

  Touch.begin();
  Touch.setRotation(3);

  Vol.init (80, 70, 40, 270, 0, 100, ILI9341_WHITE, ILI9341_BLACK, ILI9341_RED);
  Base.init (240, 70, 40, 270, 0, 100, ILI9341_WHITE, ILI9341_BLACK, ILI9341_BLUE);
  Mid.init (80, 190, 40, 270, 0, 100, ILI9341_WHITE, ILI9341_BLACK, ILI9341_GREEN);
  Treb.init (240, 190, 40, 270, 0, 100, ILI9341_WHITE, ILI9341_BLACK, ILI9341_YELLOW);

  Display.fillScreen(ILI9341_BLACK);

  // draw the controls with an initial value
  // note that value must be in the scale range
  Vol.draw(0);
  Base.draw(50);
  Mid.draw(70);
  Treb.draw(28);
  Display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  // draw text if you need
  Display.setCursor (65, 55);
  Display.print("Vol");
  Display.setCursor(225, 55);
  Display.print("Bas");
  Display.setCursor  (65, 175);
  Display.print("Mid");
  Display.setCursor  (225, 175);
  Display.print("Trb");

  Display.setCursor (65, 65);
  Display.print(Vol.value, 0);
  Display.setCursor(225, 65);
  Display.print(Base.value, 0);
  Display.setCursor  (65, 185);
  Display.print(Mid.value, 0);
  Display.setCursor  (225, 185);
  Display.print(Treb.value, 0);

}

void loop() {

  if (Touch.touched()) {
    ProcessTouch();
    // if press is in range of the ring, slide will occur

    //example in how to test for a changed state and disable a control
    if (Vol.slide(BtnX, BtnY)) {

      if (Vol.value > 80) {
        Base.disable();
        Base.draw(Base.value);
      }
      else {
        Base.enable();
        Base.draw(Base.value);
      }
    }
    Base.slide(BtnX, BtnY);
    Mid.slide(BtnX, BtnY);
    Treb.slide(BtnX, BtnY);

    // use the value property to report the controls value

    Display.setCursor (65, 65);
    Display.print(Vol.value, 0);
    Display.setCursor(225, 65);
    Display.print(Base.value, 0);
    Display.setCursor  (65, 185);
    Display.print(Mid.value, 0);
    Display.setCursor  (225, 185);
    Display.print(Treb.value, 0);



  }
}


bool ProcessButtonPress(Button TheButton) {

  if (TheButton.press(BtnX, BtnY)) {
    TheButton.draw(B_PRESSED);
    while (Touch.touched()) {
      if (TheButton.press(BtnX, BtnY)) {
        TheButton.draw(B_PRESSED);
      }
      else {
        TheButton.draw(B_RELEASED);
        return false;
      }
      ProcessTouch();
    }

    TheButton.draw(B_RELEASED);
    return true;
  }
  return false;

}

void ProcessTouch() {

  p = Touch.getPoint();

  BtnX = p.x;
  BtnY = p.y;

  // consistency between displays is a mess...
  // this is some debug code to help show
  // where you pressed and the resulting map
  /*
     Serial.print("real: ");
     Serial.print(BtnX);
     Serial.print(",");
     Serial.print(BtnY);
     Display.drawPixel(BtnX, BtnY, C_RED);
  */
  //different values depending on where Touch happened

  // x  = map(x, real left, real right, 0, width-1);
  // y  = map(y, real bottom, real top, 0, height-1);

  // tft with black headers, yellow headers will be different
  BtnX  = map(BtnX, 3850, 320, 0, 319);
  BtnY  = map(BtnY, 3800, 280, 0, 239);
  /*
    Serial.print(", Mapped: ");
    Serial.print(BtnX);
    Serial.print(",");
    Serial.println(BtnY);
    Display.drawPixel(BtnX, BtnY, C_GREEN);
  */
  // cheap debounce
  delay(50);

}
