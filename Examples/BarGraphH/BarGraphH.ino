#include <ILI9341_t3.h>
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>

#define FONT_TITLE Arial_24
#define FONT_DATA Arial_10

#define TFT_CS        10
#define TFT_DC        9
#define LED_PIN       A9
#define B_XORIGIN 70
#define B_YORIGIN 70
#define B_WIDE 170
#define B_HEIGHT 20
#define B_LOWSCALE 0
#define B_HIGHSCALE 1000
#define B_SCALEINC 250
#define B_BORDERCOLOR C_WHITE
#define B_BARCOLOR C_RED
#define B_BACKBARCOLOR C_DKRED
#define B_BACKCOLOR C_BLACK

#define V_XORIGIN 50
#define V_YORIGIN 150
#define V_WIDE 200
#define V_HEIGHT 50
#define V_LOWSCALE 0
#define V_HIGHSCALE 1
#define V_SCALEINC .5

ILI9341_t3 Display(TFT_CS, TFT_DC);

int bBits;
float bVolts;
float bData;

// create an object for each bar chart
BarChartH Bits(&Display,  B_XORIGIN, B_YORIGIN, B_WIDE, B_HEIGHT, B_LOWSCALE, B_HIGHSCALE, B_SCALEINC);
BarChartH Volts(&Display, V_XORIGIN, V_YORIGIN, V_WIDE, V_HEIGHT, V_LOWSCALE, V_HIGHSCALE, V_SCALEINC);

void setup() {
  Serial.begin(9600);

  pinMode(TFT_CS,  OUTPUT);
  pinMode(TFT_DC,  OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(ILI9341_BLACK);
  digitalWrite(LED_PIN, HIGH);

  Bits.init("Bits", B_BORDERCOLOR, B_BARCOLOR, B_BACKBARCOLOR, B_BACKCOLOR, FONT_TITLE, FONT_DATA);
  // these methods are optional but let you see what you can do
  // Bits.ShowScale(false);
  // Bits.ShowTitle(false);
  // Bits.Plot(0);


  Volts.init("Volts", C_WHITE, C_BLUE, C_DKBLUE, C_BLACK, FONT_TITLE, FONT_DATA);
  // these methods are optional but let you see what you can do
  Volts.showScale(false);
  // Volts.showTitle(false);
  // Volts.plot(0);

}

void loop() {

  bBits = analogRead(A4);
  bVolts = bBits * 3.3 / 1024;

  Bits.plot(bBits);
  // optional
  // if data exceeds top limit reset the scale
  // Bits.SetScale(0,2000,500);

  Volts.plot(bVolts);
  delay(50);

}