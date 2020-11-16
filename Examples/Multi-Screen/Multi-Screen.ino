/*

  This example shows how to create a multi-screen interface--good for having setup screens or even
  screen up screens that have setup screens

  the general notion is that a sub-screen (like a setup screen) has 2 parts 1) the processing
  screen and 2) the content drawing screen. Call the processing screen and inside the processing screen
  call the content drawing screen. After contents is drawn, control is returned back to the
  processing screen to process any actions on the processing screen. The reason for this dual function approach
  is that of you have a second option screen, the previous sceen can be easily drawn again.

*/

#include <SPI.h>
#include "ILI9341_t3.h"
#include "font_Arial.h"      // custom fonts that ships with ILI9341_t3.h
#include "UTouch.h"                     // touchscreen lib
#include "ILI9341_t3_Controls.h"

//my way of defining fonts
#define F_A24 Arial_24
#define F_A20 Arial_20
#define F_A14 Arial_14
#define F_A10 Arial_10

// typical tft pin definition
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

// i connect LED to A9 so i can manage brightness
#define LCD_PIN A9

// just a bunch of location defs
#define ROW1 50
#define ROW2 80
#define ROW3 110
#define ROW4 140
#define ROW5 200
#define COL1 10
#define COL2 180
#define BAND1 20
#define BAND2 60
#define BAND3 100
#define BAND4 140
#define BAND5 180
#define BAND6 220
#define BAND7 260
#define BAND8 300
#define MINDB -12
#define MAXDB 12
#define TOP 67
#define HEIGHT 144

// some color defs
#define SLIDECOLOR C_GREY
#define HANDLECOLOR C_ORANGE
#define BACKCOLOR BackColor
#define TEXTCOLOR C_WHITE

// variables to hold slider values
int BtnX, BtnY;
int Band1 = 0;
int Band2 = 0;
int Band3 = 0;
int Band4 = 0;
int Band5 = 0;
int Band6 = 0;
int Band7 = 0;
int Band8 = 0;

int RedVal = 0, GreenVal = 0, BlueVal = 0;
uint16_t BackColor = BackColor;
int TickState = true, SnapState = true;

byte Tick0ID, Tick1ID, Tick2ID, Tick3ID;
byte Snap0ID, Snap1ID, Snap2ID, Snap3ID;

float TickSetting = 2.0;
float SnapSetting = 1.0;

ILI9341_t3 Display = ILI9341_t3(TFT_CS, TFT_DC);

UTouch  Touch( 6, 5, 4, 3, 2);


// create buttons, check boxes, option buttons
Button EqualizerBTN(&Display);
Button ColorPickerBTN(&Display);

Button SettingsBTN(&Display);
Button DoneBTN(&Display);

SliderV sBand1(&Display);
SliderV sBand2(&Display);
SliderV sBand3(&Display);
SliderV sBand4(&Display);
SliderV sBand5(&Display);
SliderV sBand6(&Display);
SliderV sBand7(&Display);
SliderV sBand8(&Display);

SliderH Red(&Display);
SliderH Green(&Display);
SliderH Blue(&Display);

OptionButton TickO(&Display);
OptionButton SnapO(&Display);

CheckBox TickCB(&Display);
CheckBox SnapCB(&Display);


void setup() {

  Serial.begin(9600);
  pinMode(LCD_PIN, OUTPUT);
  // setup display
  delay(100);
  Display.begin();
  digitalWrite(LCD_PIN, 255);
  // set orientation--using math to flip display upside down if display is mounted that way
  Display.setRotation(1);
  Display.fillScreen(BackColor);

  Touch.InitTouch(PORTRAIT);
  Touch.setPrecision(PREC_EXTREME);

  // initialize buttons
  EqualizerBTN.init(   160, 100, 220, 40, C_BLUE, C_WHITE, BackColor, BackColor,  "Equalizer", -25, -8, F_A20 ) ;
  ColorPickerBTN.init( 160, 150, 220, 40, C_BLUE, C_WHITE, BackColor, BackColor,  "Colors", -25, -8, F_A20 ) ;
  SettingsBTN.init(200, 20, 70, 35, C_BLUE, C_GREY, BackColor, BackColor, "Option", -10, -8, F_A14 ) ;
  DoneBTN.init(    280, 20, 70, 35, C_BLUE, C_GREEN, BackColor, BackColor, "Done", -10, -8, F_A14 ) ;

  // initialize sliders
  sBand1.init(BAND1, TOP, HEIGHT, MINDB, MAXDB, TickSetting, SnapSetting, SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand2.init(BAND2, TOP, HEIGHT, MINDB, MAXDB, TickSetting, SnapSetting, SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand3.init(BAND3, TOP, HEIGHT, MINDB, MAXDB, TickSetting, SnapSetting, SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand4.init(BAND4, TOP, HEIGHT, MINDB, MAXDB, TickSetting, SnapSetting, SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand5.init(BAND5, TOP, HEIGHT, MINDB, MAXDB, TickSetting, SnapSetting, SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand6.init(BAND6, TOP, HEIGHT, MINDB, MAXDB, TickSetting, SnapSetting, SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand7.init(BAND7, TOP, HEIGHT, MINDB, MAXDB, TickSetting, SnapSetting, SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand8.init(BAND8, TOP, HEIGHT, MINDB, MAXDB, TickSetting, SnapSetting, SLIDECOLOR, BackColor, HANDLECOLOR);
  Red.init    (30, ROW2, 150, 0, 255, 0, 0, C_WHITE, BackColor, C_RED);
  Green.init  (30, ROW3, 150, 0, 255, 0, 0, C_WHITE, BackColor, C_GREEN);
  Blue.init   (30, ROW4, 150, 0, 255, 0, 0, C_WHITE, BackColor, C_BLUE);

  // initialize checkboxes
  TickCB.init(10, 70, C_WHITE, C_GREEN , C_DKGREY, C_WHITE, BackColor, 30, + 2, "Show ticks", Arial_14 );
  SnapCB.init(170, 70, C_WHITE, C_GREEN , C_DKGREY, C_WHITE, BackColor, 30, + 2, "Snapping", Arial_14 );

  // initialize options buttons and populate them with some options
  TickO.init(C_WHITE, C_GREEN , C_DKGREY , C_WHITE, BackColor, 20, -2, Arial_14 );
  Tick0ID = TickO.add(40, 100, "2 db");
  Tick1ID = TickO.add(40, 130, "3 db");
  Tick2ID = TickO.add(40, 160, "4 db");
  Tick3ID = TickO.add(40, 190, "6 db");

  SnapO.init(C_WHITE, C_GREEN , C_DKGREY , C_WHITE, BackColor, 20, -2, Arial_14 );
  Snap0ID = SnapO.add(200, 100, "2 db");
  Snap1ID = SnapO.add(200, 130, "3 db");
  Snap2ID = SnapO.add(200, 160, "4 db");
  Snap3ID = SnapO.add(200, 190, "6 db");

  // here we go... we have a function to draw the main menu screen--that way we can easily
  // redraw after completing setup screens
  // loop is the processor for the main menu screen
  Draw_MainMenu();


}

void loop() {


  if (Touch.dataAvailable()) {
    ProcessTouch();

    // if user want's to see sub-screen (an equalizer in this case)
    // call the processor and inside the processor is the draw screen
    // keep reading...
    if (ProcessButtonPress(EqualizerBTN)) {
      Process_Equalizer();
      // after user exits processor screen
      // readraw the main menu and return to loop for processing
      Draw_MainMenu();
    }

    // if user want's to see another sub-screen (an color changer in this case)
    // call the processor and inside the processor is the draw screen
    // keep reading...
    if (ProcessButtonPress(ColorPickerBTN)) {
      Process_SetColors();
      // after user exits processor screen
      // readraw the main menu and return to loop for processing
      Draw_MainMenu();
    }

  }
}


/*

  service function UI screen

*/
void Draw_MainMenu() {

  //nothing fancy, just a header and some buttons
  Display.fillScreen(BackColor);

  Display.fillRect(0, 0, 480, 50, C_DKBLUE);
  Display.setTextColor(C_WHITE);
  Display.setFont(F_A24);
  Display.setCursor(10 , 10 );
  Display.print(F("Main Menu"));

  Display.setFont(F_A24);
  Display.setTextColor(C_WHITE, BackColor);

  EqualizerBTN.draw();
  ColorPickerBTN.draw();

}

void Process_Equalizer() {

  // process equalizer screen, here we enter a loop until user exists
  // inside the loop we process any button presses, etc.

  bool KeepIn = true;

  // draw the ui
  Draw_EqualizerScreen();
  // draw some updatable text on the ui screen
  // here we will update just a portion so have a function that
  // updates that data
  Draw_EqualizerText();

  // begin the proceessing loop
  while (KeepIn) {

    if (Touch.dataAvailable()) {
      // my way of handling screen presses and adjusting to get correct screen coorinates--since different
      // tft's react differently
      ProcessTouch();

      // handle any slider changes
      Band1 = sBand1.slide(BtnX, BtnY);
      Band2 = sBand2.slide(BtnX, BtnY);
      Band3 = sBand3.slide(BtnX, BtnY);
      Band4 = sBand4.slide(BtnX, BtnY);
      Band5 = sBand5.slide(BtnX, BtnY);
      Band6 = sBand6.slide(BtnX, BtnY);
      Band7 = sBand7.slide(BtnX, BtnY);
      Band8 = sBand8.slide(BtnX, BtnY);

      // really we should check for any slider change and updated if needed
      // heck, Teensy is soo fast, just update all on any change
      Draw_EqualizerText();

      // user exists, then get out of loop and control returned to after
      // Process_Equalizer();
      if (ProcessButtonPress(DoneBTN)) {
        KeepIn = false;
      }

      // here's why I go throuhg the trouble of having all these darn screen drawing
      // functions....
      // use hits setup, draw setup, but what does user see after leaving this sub-screen?
      // you guessed it, you can now easily redraw the equalizer screen and continue
      // processing the equalizer screen
      if (ProcessButtonPress(SettingsBTN)) {
        // go do something else...
        Process_SettingsScreen();
        // but when you are done redraw the equalizer screen (as you are back in that screen)
        Draw_EqualizerScreen();
        Draw_EqualizerText();
      }
    }
  }
}

// function to just draw the UI

void Draw_EqualizerScreen() {

  // blank out old screen
  Display.fillScreen(BackColor);

  // draw the new screen
  Display.fillScreen(BackColor);
  Display.fillRect(0, 0, 480, 50, C_DKBLUE);
  Display.setTextColor(C_WHITE);
  Display.setFont(F_A24);
  Display.setCursor(10 , 10 );
  Display.print(F("Equalizer"));

  // add any ui controls
  // but recall, processing of these controls is done in the Process_xxx function
  sBand1.draw(Band1);
  sBand2.draw(Band2);
  sBand3.draw(Band3);
  sBand4.draw(Band4);
  sBand5.draw(Band5);
  sBand6.draw(Band6);
  sBand7.draw(Band7);
  sBand8.draw(Band8);

  DoneBTN.draw();
  SettingsBTN.draw();

}

// screen to draw jsut a portion of the UI screen. This data changes upon a UI control change
// so just update some text as opposed to the whole screen (which includes sliders and will flicker...yuck)
void Draw_EqualizerText() {

  char buf[4];

  Display.setTextColor(C_WHITE, BackColor);
  Display.setFontAdafruit();
  Display.setTextSize(1);
  sprintf(buf, "% 3d", Band1);
  Display.setCursor(BAND1 - 10 , TOP - 10 ); Display.print(buf);
  sprintf(buf, "% 3d", Band2);
  Display.setCursor(BAND2 - 10 , TOP - 10 ); Display.print(buf);
  sprintf(buf, "% 3d", Band3);
  Display.setCursor(BAND3 - 10 , TOP - 10 ); Display.print(buf);
  sprintf(buf, "% 3d", Band4);
  Display.setCursor(BAND4 - 10 , TOP - 10 ); Display.print(buf);
  sprintf(buf, "% 3d", Band5);
  Display.setCursor(BAND5 - 10 , TOP - 10 ); Display.print(buf);
  sprintf(buf, "% 3d", Band6);
  Display.setCursor(BAND6 - 10 , TOP - 10 ); Display.print(buf);
  sprintf(buf, "% 3d", Band7);
  Display.setCursor(BAND7 - 10 , TOP - 10 ); Display.print(buf);
  sprintf(buf, "% 3d", Band8);
  Display.setCursor(BAND8 - 10, TOP - 10 ); Display.print(buf);

  Display.setCursor(BAND1 , TOP + HEIGHT + 10 ); Display.print(F("63"));
  Display.setCursor(BAND2 , TOP + HEIGHT + 10 ); Display.print(F("125"));
  Display.setCursor(BAND3 , TOP + HEIGHT + 10 ); Display.print(F("250"));
  Display.setCursor(BAND4 , TOP + HEIGHT + 10); Display.print(F("500"));
  Display.setCursor(BAND5 , TOP + HEIGHT + 10); Display.print(F("1K"));
  Display.setCursor(BAND6 , TOP + HEIGHT + 10); Display.print(F("2K"));
  Display.setCursor(BAND7 , TOP + HEIGHT + 10); Display.print(F("4K"));
  Display.setCursor(BAND8 , TOP + HEIGHT + 10); Display.print(F("8K"));

}


// another special screen for settings
// this is the processing screen and as you may have guessed, there is a separate
// function to draw the screen contents
// again I do this so I can easily redraw the contents if a sub-screen is called

void Process_SettingsScreen() {

  int stateTB, stateSN, optionTB, optionSN;

  // the flag to enter a processing loop
  bool KeepIn = true;

  // yep, here we go, draw the darn contents, but proces screen presses in this screen.
  Draw_SettingsScreen();

  // begin the processing loop as in all my processing screens
  while (KeepIn) {
    if (Touch.dataAvailable()) {

      ProcessTouch();

      // proces some optios for the equalizer
      stateTB = TickCB.press(BtnX, BtnY);
      stateSN = SnapCB.press(BtnX, BtnY);
      optionTB = TickO.press(BtnX, BtnY);
      optionSN = SnapO.press(BtnX, BtnY);

      // I admit this is an overly complicated example to show how to create nested screens
      // but i just love that cute equalizer thingy
      if (stateTB == 1 ) {
        TickO.enable();
        TickO.draw(TickO.value);
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }
      else if (stateTB == 0) {
        TickO.disable();
        TickO.draw(TickO.value);
        TickSetting = 0.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }


      if (stateSN == 1) {
        SnapO.enable();
        SnapO.draw(SnapO.value);
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }
      else if (stateSN == 0) {
        SnapO.disable();
        SnapO.draw(SnapO.value);
        SnapSetting = 0.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }

      if (optionTB == Tick0ID) {
        TickSetting = 2.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }
      else if (optionTB == Tick1ID) {
        TickSetting = 3.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }
      else if (optionTB == Tick2ID) {
        TickSetting = 4.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }
      else if (optionTB == Tick3ID) {
        TickSetting = 6.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }

      if (optionSN == Snap0ID) {
        SnapSetting = 2.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }
      else if (optionSN == Snap1ID) {
        SnapSetting = 3.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }
      else if (optionSN == Snap2ID) {
        SnapSetting = 4.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }
      else if (optionSN == Snap3ID) {
        SnapSetting = 6.0;
        sBand1.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand2.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand3.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand4.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand5.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand6.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand7.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
        sBand8.setScale(MINDB, MAXDB, TickSetting, SnapSetting);
      }

      if (ProcessButtonPress(DoneBTN)) {
        // now were exiting this process screen meaning we're
        // returning control back to the equalizer process loop
        // so after returing we will need to redraw the process equalizer screen
        KeepIn = false;
      }

    }
  }

}

// this screen will display the ui for the settings, but again ui is processed in
// the calling function
void Draw_SettingsScreen() {

  Display.fillScreen(BackColor);
  Display.fillRect(0, 0, 480, 50, C_DKBLUE);
  Display.setTextColor(C_WHITE);
  Display.setFont(F_A24);
  Display.setCursor(10 , 10 );
  Display.print(F("Settings"));

  TickCB.draw(TickCB.value);
  TickO.draw(TickO.value);

  SnapCB.draw(SnapCB.value);
  SnapO.draw(SnapO.value);

  DoneBTN.draw();

}

// ok, ya got me...
// this function doest not call a ui drawing screen why?
// because there are no sub-screens that may get created
// if you have no sub screens, you don't have a reason
// to simplify drawing UI, here we'll just draw the UI
void Process_SetColors() {

  // typical flag for the ui processing loop
  bool KeepIn = true;

  // draw all the UI
  Display.fillScreen(BackColor);
  Display.fillRect(0, 0, 480, 50, C_DKBLUE);
  Display.setTextColor(C_WHITE);
  Display.setFont(F_A24);
  Display.setCursor(10 , 10 );
  Display.print(F("Background "));

  Display.fillRect(200, 80, 100, 100, C_WHITE);

  Display.fillRect(203, 83, 94, 94, BackColor);

  Red.draw(RedVal);
  Green.draw(GreenVal);
  Blue.draw(BlueVal);
  DoneBTN.draw();

  // start processing UI, again keep in this loop, with displayed UI until user
  // presses done button
  while (KeepIn) {

    if (Touch.dataAvailable()) {
      ProcessTouch();

      RedVal = Red.slide(BtnX, BtnY);
      GreenVal = Green.slide(BtnX, BtnY);
      BlueVal = Blue.slide(BtnX, BtnY);

      BackColor = Display.color565(RedVal, GreenVal, BlueVal);

      Display.fillRect(203, 83, 94, 94, BackColor);

      if (ProcessButtonPress(DoneBTN)) {
        KeepIn = false;
      }
    }


  }

  sBand1.setColors(SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand2.setColors(SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand3.setColors(SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand4.setColors(SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand5.setColors(SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand6.setColors(SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand7.setColors(SLIDECOLOR, BackColor, HANDLECOLOR);
  sBand8.setColors(SLIDECOLOR, BackColor, HANDLECOLOR);

  Red.setColors(C_WHITE, BackColor, C_RED);
  Green.setColors(C_WHITE, BackColor, C_GREEN);
  Blue.setColors(C_WHITE, BackColor, C_BLUE);


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

  //Serial.print(", Mapped coordinates: ");
  //Serial.print(BtnX);
  //Serial.print(",");
  //Serial.println(BtnY);
  //Display.drawPixel(BtnX, BtnY, C_GREEN);

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
