#include <ILI9341_t3.h>           // fast display driver lib
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>           // custom fonts that ships with ILI9341_t3.h
#include <SPI.h>

// I like to use defines for fonts
#define FONT_TITLE Arial_24
#define FONT_DATA Arial_16

// standard pin stuff
#define TFT_CS        10
#define TFT_DC        9
// I generally connect LCD to a pin for controlling brightness
// connect LED to 3V3 for display always on
#define LED_PIN       A9

// defines for bar char locations and such
#define B_XORIGIN 30
#define B_YORIGIN 220
#define B_WIDE 60
#define B_HEIGHT 180
#define B_LOWSCALE 0
#define B_HIGHSCALE 1000
#define B_SCALEINC 250
#define B_BORDERCOLOR C_WHITE
#define B_BARCOLOR C_RED
#define B_BACKBARCOLOR C_DKRED
#define B_BACKCOLOR C_BLACK

#define V_XORIGIN 200
#define V_YORIGIN 220
#define V_WIDE 30
#define V_HEIGHT 180
#define V_LOWSCALE 0
#define V_HIGHSCALE 1
#define V_SCALEINC .5
#define V_BORDERCOLOR C_WHITE
#define V_BARCOLOR C_BLUE
#define V_BACKBARCOLOR C_DKBLUE
#define V_BACKCOLOR C_BLACK

// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

// some variables to hold values
int bBits;
float bVolts;
float bData;

// create an object for each point to plot
BarChartV Bits(&Display,  B_XORIGIN, B_YORIGIN, B_WIDE, B_HEIGHT, B_LOWSCALE, B_HIGHSCALE, B_SCALEINC);
BarChartV Volts(&Display, V_XORIGIN, V_YORIGIN, V_WIDE, V_HEIGHT, V_LOWSCALE, V_HIGHSCALE, V_SCALEINC);

void setup() {

  Serial.begin(9600);
  // you know the drill
  pinMode(LED_PIN, OUTPUT);

  // fire up the display
  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(C_BLACK);

  // turn on the display
  digitalWrite(LED_PIN, HIGH);

  // initiallize each bar chart
  Bits.init("Bits", B_BORDERCOLOR, B_BARCOLOR, B_BACKBARCOLOR, B_BACKCOLOR, FONT_TITLE , FONT_DATA );
  Volts.init("Volts", V_BORDERCOLOR, V_BARCOLOR, V_BACKBARCOLOR, V_BACKCOLOR, FONT_TITLE , FONT_DATA );
  // completely optional but you can hide all the text
  // Bits.showTitle(false);
  // Bits.showScale(false);

}

void loop() {

  bBits = analogRead(A4);
  bVolts = bBits * 3.3 / 1024;

  // plot the data
  Bits.plot(bBits);
  Volts.plot(bVolts);

  // completely optional but you can reset the scale if vals exceed initial scale
  // Bits.setScale(0, 2000, 500);

  delay(50);

}