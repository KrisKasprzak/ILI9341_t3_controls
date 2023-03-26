/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3

  // required
  CheckBox(ILI9341_t3 *Display) {d = Display; }
  void init(int16_t ButtonX, uint16_t ButtonY, uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const char *Text, const ILI9341_t3_font_t &TextFont ) {
  void draw(int val) {
  int press(int16_t SceenX, int16_t ScreenY) // returns if control was clicked. use value property to get the state

  // optional button methods
  void show() {
  void hide() {
  void disable() {
  void enable() {
  void resize(int16_t ButtonX, int16_t ButtonY, uint8_t Size) {
  void setColors(uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t BackgroundColor, uint16_t DisableOutlineColor,  uint16_t DisableTextColor, uint16_t DisableUPColor, uint16_t DisableDownColor) {
  void setText(int TextOffsetX,int TextOffsetY, const char *Text, const ILI9341_t3_font_t &TextFont) {
  void setCornerRadius(int val) {
  bool isEnabled() {
  bool isVisibled() {
  void setPressDebounce(byte Debounce) {
  bool value;

*/

/*

  Implementing this library is done in 5 lines of code, look for Step xxx below

*/

#include <ILI9341_t3.h>           // fast display driver lib
#include <XPT2046_Touchscreen.h>
// step 1 include the library
#include <ILI9341_t3_Controls.h>  // custom control define file
#include <font_Arial.h>

#define FONT Arial_16

#define LONG_PRESS_DURATION 1000
#define NO_PRESS 0
#define SHORT_PRESS 1
#define LONG_PRESS 2

#define ROW1 55
#define ROW2 90
#define ROW3 125
#define ROW4 160
#define ROW5 215

#define TFT_DC 9       // DC pin on LCD
#define TFT_CS 10      // chip select pin on LCD
#define LCD_PIN A9     // lcd pin to control brightness
#define T_CS      0
#define T_IRQ     1

#define C_WHITE       0xFFFF
#define C_BLACK       0x0000
#define C_GREY        0xC618
#define C_BLUE        0x001F
#define C_RED         0xF800
#define C_GREEN       0x07E0
#define C_CYAN        0x07FF
#define C_MAGENTA     0xF81F
#define C_YELLOW      0xFFE0  
#define C_TEAL      0x0438       
#define C_ORANGE        0xDC00          
#define C_PINK          0xF81F
#define C_PURPLE    0x801F

#define OUTLINECOLOR  C_GREY
#define CHECKEDCOLOR      C_GREEN
#define UNCHECKEDCOLOR    C_BLACK
#define BACKCOLOR    C_BLACK
#define TEXTCOLOR C_WHITE

int BtnX, BtnY;

// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

XPT2046_Touchscreen Touch(T_CS, T_IRQ);
TS_Point TP;

// Step 2 create an object for each check box
CheckBox CB1(&Display);
CheckBox CB2(&Display);
CheckBox CB3(&Display);
CheckBox CB4(&Display);
CheckBox CB5(&Display);

void setup() {

  Serial.begin(9600);
  pinMode(LCD_PIN, OUTPUT);

  // Step 3 create each each check box, passing in the location, colors, text offsets (to let you control text location
  // text and ILI9341 text font, note size and disable colors have defaults that can be overridded before draw
  // method or change the #define in the .h file
  CB1.init(50, ROW1, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Bananas", Arial_14 );
  CB2.init(50, ROW2, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Apples", Arial_14 );
  CB3.init(50, ROW3, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Cherries", Arial_14 );
  CB4.init(50, ROW4, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Pickles", Arial_14 );
  CB5.init(50, ROW5, OUTLINECOLOR, CHECKEDCOLOR, UNCHECKEDCOLOR, TEXTCOLOR, BACKCOLOR, 20, 5, "Only allow fruit", Arial_14 );

  // fire up the display
  Display.begin();

  // fire up touch
  Touch.begin();
  Touch.setRotation(3); // this may need to be adjusted depending on your display
  
  Display.invertDisplay(false);

  Display.setRotation(1);

  // turn the brightness up
  analogWrite(LCD_PIN, 255);

  Display.fillScreen(C_BLACK);

  Display.fillRect(0, 0, 480, 50, C_BLUE);
  Display.setTextColor(C_WHITE);
  Display.setFont(Arial_24);
  Display.setCursor(10 , 10 );
  Display.print(F("Check Box Example"));

  // Step 4 draw the check box controls
  // optional, you can set other parameters before the draw method (size for example)
  CB1.draw(true);
  CB2.draw(true);
  CB3.draw(true);
  CB4.draw(true);
  CB5.draw(false);

}

void loop() {

  if (Touch.touched()) {
    ProcessTouch();

    // Step 5 upon press of the checkbox process accordingly
    // optional, you can set other parameters upon pressing a check box
    // such as setting color or text (could be useful in re-drawing the control
    // with different text, enabling /disabling a control
    // just call the draw event after chaning parameters

    CB1.press(BtnX, BtnY);
    CB2.press(BtnX, BtnY);
    CB3.press(BtnX, BtnY);
    CB4.press(BtnX, BtnY);
    // if you really need to monitor if the control was clicked
    // here's how, this may be needed if you have a complicated UI
    // and don't need to automatically change things--code here automatically
    // updates items based on check box value (see if else sections, these
    // run even of check box was not pressed
    // do not use the return value as check box state
    // only use Checkbox.value for the state
    bool changed = CB5.press(BtnX, BtnY);

// as mentioned above, you code processes regardless if checkbox was clicked
    if (CB1.value) {
      // un checked your code goes here
    }
    else {
      // checked, your code goes here
    }


    if (CB2.value) {
      // un checked your code goes here
    }
    else {
      // checked, your code goes here
    }
    if (CB3.value) {
      // un checked your code goes here
    }
    else {
      // checked, your code goes here
    }
    if (CB4.value) {
      // un checked your code goes here
    }
    else {
      // checked, your code goes here
    }

    // as mentioned above, you you don't want to process code unless checkbox was clicked
    // wrap a change test around checkbox values
    if (changed) {
      if (CB5.value) {
        CB4.disable();
        CB4.draw(false); // your call, but if option disabled, maybe force state to false
      }
      else {
        CB4.enable();
        CB4.draw(CB4.value);
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


  TP = Touch.getPoint();
  BtnX = TP.x;
  BtnY = TP.y;

  // consistency between displays is a mess...
  // this is some debug code to help show
  // where you pressed and the resulting map

  // x  = map(x, real left, real right, 0, 480);
  // y  = map(y, real bottom, real top, 320, 0);

  //yellow headers
  BtnX = map(BtnX, 3970, 307, 320, 0);
  BtnY = map(BtnY, 3905, 237, 240, 0);

  //black headers
  //BtnX  = map(BtnX, 0, 3905, 320, 0);
  //BtnY  = map(BtnY, 0, 3970, 240, 0);

  Serial.print(", Mapped coordinates: ");
  Serial.print(BtnX);
  Serial.print(",");
  Serial.println(BtnY);
  Display.fillCircle(BtnX, BtnY, 2, C_GREEN);

}
