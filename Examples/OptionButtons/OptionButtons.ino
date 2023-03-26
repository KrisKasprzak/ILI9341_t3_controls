/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3

  // required
  OptionButton(ILI9341_t3 *Display) {d = Display; }
  void init(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, int16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont) {
  int add(uint16_t ButtonX, uint16_t ButtonY,const char *Text ) {
  void draw(int OptionID) {
  int press(uint16_t ScreenX, uint16_t ScreenY) // returns if control was clicked. use value property to get the selected option

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
#include <XPT2046_Touchscreen.h>
// step 1 include the library
#include <ILI9341_t3_Controls.h>  // custom control define file
#include <font_Arial.h>

#define FONT Arial_16
#define TFT_DC 9       // DC pin on LCD
#define TFT_CS 10      // chip select pin on LCD
#define LCD_PIN A9     // lcd pin to control brightness
#define T_CS      0
#define T_IRQ     1

#define ROW1 60
#define ROW2 90
#define ROW3 120
#define ROW4 150
#define ROW5 200

#define COL1 10
#define COL2 180

int OptionA0ID, OptionA1ID, OptionA2ID, OptionA3ID, OptionA4ID;
int OptionB0ID, OptionB1ID, OptionB2ID, OptionB3ID, OptionB4ID;

int BtnX, BtnY;

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
#define C_LTGREY        0xE71C  
#define C_LTBLUE    0x73DF    
#define C_LTRED         0xFBAE
#define C_LTGREEN       0x7FEE
#define C_LTCYAN    0x77BF
#define C_LTMAGENTA     0xFBB7
#define C_LTYELLOW      0xF7EE
#define C_LTTEAL    0x77FE  
#define C_LTORANGE      0xFDEE
#define C_LTPINK        0xFBBA
#define C_LTPURPLE    0xD3BF
#define C_DKGREY        0x52AA
#define C_DKBLUE        0x080B
#define C_DKRED         0x7800
#define C_DKGREEN       0x03C2   
#define C_DKCYAN        0x032F  
#define C_DKMAGENTA     0x900B
#define C_DKYELLOW      0x94A0
#define C_DKTEAL    0x0452
#define C_DKORANGE    0x92A0         
#define C_DKPINK        0x9009
#define C_DKPURPLE      0x8012  
#define C_MDGREY        0x7BCF  
#define C_MDBLUE    0x1016
#define C_MDRED       0xB000
#define C_MDGREEN     0x0584
#define C_MDCYAN      0x04B6
#define C_MDMAGENTA   0xB010
#define C_MDYELLOW      0xAD80    
#define C_MDTEAL    0x0594     
#define C_MDORANGE      0xB340           
#define C_MDPINK        0xB00E

// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

XPT2046_Touchscreen Touch(T_CS, T_IRQ);
TS_Point TP;

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

  OBB.init(C_WHITE, C_GREEN, C_DKGREY, C_WHITE, C_DKGREY, 20, -2, Arial_14 );
  OptionB0ID = OBB.add(COL2, ROW1, "Steak");
  OptionB1ID = OBB.add(COL2, ROW2, "Pork");
  OptionB2ID = OBB.add(COL2, ROW3, "Chicken");
  OptionB3ID = OBB.add(COL2, ROW4, "Fish");

  CB1.init(COL1, ROW5, C_WHITE, C_GREEN, C_DKGREY, C_WHITE, C_BLACK, 30, 2, "Enable", Arial_14 );
  CB2.init(COL2, ROW5, C_WHITE, C_GREEN, C_DKGREY, C_WHITE, C_BLACK, 30, 2, "Show", Arial_14 );


  // fire up the display
  Display.begin();

  // fire up touch
  Touch.begin();
  Touch.setRotation(3); // this may need to be adjusted depending on your display
  
  Display.invertDisplay(false);
  Display.fillScreen(C_BLACK);
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

  OBA.draw(0);
  OBB.draw(1);

  CB1.draw(true);
  CB2.draw(true);

}

void loop() {

  if (Touch.touched()) {
    ProcessTouch();

    // if you really need to monitor if the control was clicked
    // here's how, this may be needed if you have a complicated UI
    // and don't need to automatically change things--code here shows 2 cases
    // 1) automatically updates items based on check box value (see if else sections, these
    // run even of check box was not pressed
    // and 2) only updating items if the check box was changed
    // do not use the return value as check box state
    // only use Checkbox.value for the state
    bool changed = CB1.press(BtnX, BtnY);
    CB2.press(BtnX, BtnY);


    // step 6 proces the option group (this mehod will pass back the selected option
    // so no need to rip through each option to test if selected. this call will pass back
    // the selected option
    OBA.press(BtnX, BtnY);
    OBB.press(BtnX, BtnY);

    // process option A group
    if (OBA.value == OptionA0ID) {
      // your code goes here
    }
    else if (OBA.value == OptionA1ID) {
      // your code goes here
    }
    else if (OBA.value == OptionA2ID) {
      // your code goes here
    }
    else if (OBA.value == OptionA3ID) {
      // your code goes here
    }

    //process option B group
    if (OBB.value == OptionB0ID) {
      // your code goes here
    }
    else if (OBB.value == OptionB1ID) {
      // your code goes here
    }
    else if (OBB.value == OptionB2ID) {
      // your code goes here
    }
    else if (OBB.value == OptionB3ID) {
      // your code goes here
    }

    // as mentioned above, you you don't want to process code unless checkbox was clicked
    // wrap a change test around checkbox values
    if (changed) {
      if (CB1.value) {
        Serial.println("enabling");
        OBA.enable();
        OBA.draw(OBA.value);
      }
      else {
        Serial.println("disabling");
        OBA.disable();
        OBA.draw(OBA.value);
      }
    }

    // as mentioned above, you code processes regardless if checkbox was clicked
    if (CB2.value) {
      Serial.println("showing");
      OBB.show();
      OBB.draw(OBB.value);
    }
    else {
      Serial.println("hiding");
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

  TP = Touch.getPoint();
  BtnX = TP.x;
  BtnY = TP.y;
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
