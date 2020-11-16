/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  touchscreen lib           https://github.com/dgolda/UTouch

  // required
  OptionButton(ILI9341_t3 *Display) {d = Display; }
  void init(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, int16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont) {
  int add(uint16_t ButtonX, uint16_t ButtonY,const char *Text ) {
  void draw(int OptionID) {
  int press(uint16_t ScreenX, uint16_t ScreenY) {
  
  // optional button methods
  void select(int val) {
  void show() {
  void hide() {
  void disable() { 
  void enable() { 
  void resize(byte radius) {
  void setColors(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, uint16_t TextColor, uint16_t BackgroundColor, uint16_t DisableOutlineColor, uint16_t DisableSelColor, uint16_t DisableUnSelColor, uint16_t DisableTextColor) {
  void setFont(int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont) {
  void setText(int ID, const char *Text) {
  int selected() { 
  bool isEnable() { 
  bool isVisible() { 
  void setPressDebounce(byte Debounce) { 
  int value;

*/

/*

  To implement an option button group is 6 lines of code, look for the Step x below

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include "UTouch.h"               // touchscreen lib
// step 1 include the library
#include <ILI9341_t3_Controls.h>  // custom control define file
#include <font_Arial.h>

#define FONT Arial_16
#define TFT_DC 9       // DC pin on LCD
#define TFT_CS 10      // chip select pin on LCD
#define LCD_PIN A9     // lcd pin to control brightness
#define BACKCOLOR    C_BLACK
#define ROW1 60
#define ROW2 90
#define ROW3 120
#define ROW4 150
#define ROW5 200

#define COL1 10
#define COL2 180

int OptionA = 0, OptionB = 0;
int OptionA0ID, OptionA1ID, OptionA2ID, OptionA3ID, OptionA4ID;
int OptionB0ID, OptionB1ID, OptionB2ID, OptionB3ID, OptionB4ID;
int BtnX, BtnY;
int CBstate1 = CHECKED;
int CBstate2 = CHECKED;
// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

// create the touch screen object
// UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch  Touch( 6, 5, 4, 3, 2);

// step 2 create an option button GROUP (not individual options), pass in the display object
OptionButton OBA(&Display);
OptionButton OBB(&Display);

CheckBox CB1(&Display);
CheckBox CB2(&Display);

void setup() {

  Serial.begin(9600);
  pinMode(LCD_PIN, OUTPUT);

  // step 3 initialize each option button group, passing in colors,
  // button text offsets (to help manage text location on the button and the ILI9341 font
  OBA.init(C_WHITE, C_GREEN, C_DKGREY, C_WHITE, C_BLACK, 20, -2, Arial_14 );

  // step 4 Add an option to be listed in the button group, passing in location,
  // and text
  OptionA0ID = OBA.add(COL1, ROW1, "Apples");
  OptionA1ID = OBA.add(COL1, ROW2, "Oranges");
  OptionA2ID = OBA.add(COL1, ROW3, "Mangos");
  OptionA3ID = OBA.add(COL1, ROW4, "Strawberries");

  OBB.init(C_WHITE, C_GREEN, C_DKGREY, C_WHITE, C_BLACK, 20, -2, Arial_14 );
  OptionB0ID = OBB.add(COL2, ROW1, "Steak");
  OptionB1ID = OBB.add(COL2, ROW2, "Pork");
  OptionB2ID = OBB.add(COL2, ROW3, "Chicken");
  OptionB3ID = OBB.add(COL2, ROW4, "Fish");

  CB1.init(COL1, ROW5, C_WHITE, C_GREEN, C_DKGREY, C_WHITE, C_BLACK, 30, 2, "Enable", Arial_14 );
  CB2.init(COL2, ROW5, C_WHITE, C_GREEN, C_DKGREY, C_WHITE, C_BLACK, 30, 2, "Show", Arial_14 );


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

  Display.fillRect(0, 0, 480, 50, C_DKBLUE);
  Display.setTextColor(C_WHITE);
  Display.setFont(Arial_24);
  Display.setCursor(10 , 10 );
  Display.print(F("Options Example"));


  Display.fillRect(5, 55, 320 / 2 - 10, 240 - 55 - 10, C_DKGREY);
  Display.fillRect(320 / 2 + 10, 55, 320 / 2 - 10, 240 - 55 - 10, C_DKGREY);

  // optional, you can set other parameters before the draw method (color and default selected
  // option for example)
  // step 5 draw each option button group (this method will automatically draw
  // all items in the group

  OBA.draw(OptionA2ID);
  OBB.draw(OptionA1ID);

  CB1.draw(CBstate1);
  CB2.draw(CBstate2);

}

void loop() {

  if (Touch.dataAvailable()) {
    ProcessTouch();

    CBstate1 = CB1.press(BtnX, BtnY);
    CBstate2 = CB2.press(BtnX, BtnY);


    // step 6 proces the option group (this mehod will pass back the selected option
    // so no need to rip through each option to test if selected. this call will pass back
    // the selected option
    OptionA = OBA.press(BtnX, BtnY);
    OptionB = OBB.press(BtnX, BtnY);

    // process option A group
    if (OptionA == OptionA0ID) {
      // your code goes here
    }
    else if (OptionA == OptionA1ID) {
      // your code goes here
    }
    else if (OptionA == OptionA2ID) {
      // your code goes here
    }
    else if (OptionA == OptionA3ID) {
      // your code goes here
    }

    //process option B group
    if (OptionB == OptionB0ID) {
      // your code goes here
    }
    else if (OptionB == OptionB1ID) {
      // your code goes here
    }
    else if (OptionB == OptionB2ID) {
      // your code goes here
    }
    else if (OptionB == OptionB3ID) {
      // your code goes here
    }

    if (CBstate1 == CHECKED ) {
      OBA.enable();
      OBA.draw(OBA.value);
    }
    else if (CBstate1 == UNCHECKED ) {
      OBA.disable();
      OBA.draw(OBA.value);
    }

    if (CBstate2 == CHECKED ) {
      OBB.show();
      OBB.draw(OBB.value);
    }
    else if (CBstate2 == UNCHECKED ) {
      OBB.hide();
      OBB.draw(OBB.value);
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
  // Display.drawPixel(BtnX, BtnY, C_GREEN);

}