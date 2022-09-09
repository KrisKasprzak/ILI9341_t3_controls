/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  touchscreen lib           https://github.com/dgolda/UTouch

  // required
  Button(ILI9341_t3 *Display) {d = Display; }
  void init(int16_t ButtonX, int16_t ButtonY, uint8_t ButtonWidth, uint8_t ButtonHeight,
    uint16_t OutlineColor, uint16_t ButtonColor, uint16_t TextColor, uint16_t BackgroundColor,
    const char *ButtonText, int TextOffsetX, int TextOffsetY, const ILI9341_t3_font_t &TextFont ) {
  void draw(bool inverted = false) {
  bool press(int16_t ScreenX, int16_t ScreenY) {
  
  // optional button methods
  void hide()
  void disable()
  void enable()
  void resize(int16_t ButtonX, int16_t ButtonY, uint8_t ButtonW, uint8_t ButtonH)
  void setColors(uint16_t OutlineColor, uint16_t ButtonColor, uint16_t TextColor, uint16_t BackgroundColor, uint16_t DisabledTextColor, uint16_t DisabledButtonColor) {
  void setFont(int TextOffsetX, int TextOffsetY, const ILI9341_t3_font_t &TextFont)
  void setText(const char *ButtonText)
  void setCornerRadius(int radius)
  bool isEnabled() 
  bool isVisible() 
  void setPressDebounce(byte Debounce) 
  
  int value;


*/


/*

  To implement a button is 5 lines of code, look for the Step x below

*/

#include <ILI9341_t3.h>           // fast display driver lib

// step 1 include the library
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>           // custom fonts that ships with ILI9341_t3.h
#include <font_ArialBold.h>
#include "UTouch.h"
#include <Colors.h>

// you must create and pass fonts to the function
#define FONT_TEXT Arial_24
#define FONT_LBUTTON Arial_16
#define FONT_SBUTTON Arial_12_Bold

// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10
unsigned long i;
int BtnX, BtnY, ct = -1;
bool rs = true, st = true;
char buf[1];
// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

UTouch  Touch( 6, 5, 4, 3, 2);

// step 2 create a button object for each button, pass in the display object
Button Button1(&Display);
Button Button2(&Display);
Button Button3(&Display);
Button Button4(&Display);
Button Button5(&Display);

void setup() {

  Serial.begin(57600);

  // fire up the display
  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(C_BLACK);
  analogWrite(A9, 255);
  // fire up the touch display

  // you may need this depending on your display
  Touch.InitTouch(PORTRAIT);
  Touch.setPrecision(PREC_EXTREME);

  // step 3 initialize each button, passing in location, size, colors, button text,
  // button text offsets (to help manage text location on the button and the ILI9341 font
  Button1.init(60, 20, 100, 40, C_BLUE, C_WHITE, C_BLACK, C_BLACK, "Press", -10, -5, FONT_SBUTTON ) ;
  Button2.init(60, 80, 100, 40, C_BLUE, C_WHITE, C_BLACK, C_BLACK,  "Disable", -10, -5, FONT_SBUTTON ) ;
  Button3.init(180, 80, 100, 40, C_BLUE, C_WHITE, C_BLACK, C_BLACK,  "Radius", -10, -5, FONT_SBUTTON ) ;
  Button4.init(60, 140, 100, 40, C_BLUE, C_WHITE, C_BLACK, C_BLACK,  "Move", -10, -5, FONT_SBUTTON ) ;
  Button5.init(180, 140, 100, 40, C_BLUE, C_GREEN, C_BLACK, C_BLACK, "ON", -10, -5, FONT_LBUTTON ) ;

  // optional, you can set other parameters before the draw method (corner size for example)
  // step 4 draw each button
  Button1.draw();
  Button2.draw();
  Button3.draw();
  Button4.draw();
  Button5.draw();
  
}


void loop() {

  if (Touch.dataAvailable()) {

    ProcessTouch();

    // step 5 handle any button presses
    // optional, you can set other parameters upon pressing a button
    // such as setting color or text (could be useful in re-drawing the button
    // with text that says On/Off and in a color such as Green/Red
    // just call the draw event after chaning parameters
    if (ProcessButtonPress(Button1)) {
      Display.setFont(FONT_TEXT);
      Display.setTextColor(C_WHITE);
      Display.setCursor(150, 5);
      Display.print("Pressed");
      delay(500);
      Display.fillRect(150, 5, 200, 40, C_BLACK);
    }
    else if (ProcessButtonPress(Button2)) {
      if (Button3.value == 0) {
        Button3.value = 1;
        Button3.disable();
        Button3.draw();
      }
      else {
        Button3.value = 0;
        Button3.enable();
        Button3.draw();
      }
    }

    if (ProcessButtonPress(Button3)) {
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
        Button3.draw();
      }
      else {
        sprintf(buf, "r=%d", ct);
        Button3.setText(buf);
        Button3.setCornerRadius(ct);
        Button3.draw();
      }
    }

    if (ProcessButtonPress(Button4)) {
      if (rs) {
        rs = false;
        Button5.resize(180, 180, 100, 40);
        Button5.draw();
      }
      else {
        rs = true;
        Button5.resize(180, 140, 100, 40);
        Button5.draw();
      }

    }
    if (ProcessButtonPress(Button5)) {
      if (st) {
        st = false;
        Button5.setText("Off");
        Button5.setColors(C_LTGREY, C_RED, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY);
        Button5.draw();
      }
      else {
        st = true;
        Button5.setText("On");
        Button5.setColors(C_LTGREY, C_GREEN, C_BLACK, C_BLACK, C_DKGREY, C_MDGREY);
        Button5.draw();
      }
    }
  }
}


// my own processing function to handle press and convert to proper screen
// coordinates
// i've found so many differences between different screens, i needed an easy
// way to handle returning screen calcualtions
// my code uses global button x and button y locations
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
  //Display.drawPixel(BtnX, BtnY, C_RED);

  //different values depending on where touch happened

  // x  = map(x, real left, real right, 0, 480);
  // y  = map(y, real bottom, real top, 320, 0);

  // tft with yellow headers
  //BtnX  = map(BtnX, 240, 0, 320, 0);
  //BtnY  = map(BtnY, 379, 0, 240, 0);

  // tft with black headers
  BtnX  = map(BtnX, 0, 240, 320, 0);
  BtnY  = map(BtnY, 0, 380, 240, 0);

  Serial.print(", Mapped coordinates: ");
  Serial.print(BtnX);
  Serial.print(",");
  Serial.println(BtnY);
  Display.drawPixel(BtnX, BtnY, C_GREEN);

}

// my custom function to process a button press
// if user presses a button show button in different color
// if user continues to press a button, wait until release
// to do something, I didn't find a good way to implement Adafruits
// methods, so i created my own
bool ProcessButtonPress(Button TheButton) {

  if (TheButton.press(BtnX, BtnY)) {
    TheButton.draw(B_PRESSED);
    while (Touch.dataAvailable()) {
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
