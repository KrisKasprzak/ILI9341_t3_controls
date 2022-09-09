#include <ILI9341_t3.h>
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>
#include <font_ArialBold.h>
#include <Colors.h>


#define FONT_TITLE Arial_16
#define FONT_DATA Arial_10_Bold

// defines for pin connection
#define TFT_CS        10
#define TFT_DC        9
#define LED_PIN       A9

// defines for locations
#define BXCENTER 80
#define BYCENTER 120
#define BDIAMETER 75
#define BLOWSCALE 0
#define BHIGHSCALE 1200
#define BSCALEINC 200
#define BSWEEPANGLE 300

#define VXCENTER 240
#define VYCENTER 120
#define VDIAMETER 75
#define VLOWSCALE 0
#define VHIGHSCALE 4
#define VSCALEINC .5
#define VSWEEPANGLE 300

// defines for colors
#define BNEEDLECOLOR C_ORANGE
#define BDIALCOLOR C_DKBLUE
#define BTEXTCOLOR C_WHITE
#define BTICCOLOR C_GREY

#define VNEEDLECOLOR C_WHITE
#define VDIALCOLOR C_DKBLUE
#define VTEXTCOLOR C_WHITE
#define VTICCOLOR C_GREY

// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

int bBits;
float bVolts;
float bData;

// create the dial object(s)
Dial Bits(&Display, BXCENTER, BYCENTER, BDIAMETER, BLOWSCALE , BHIGHSCALE, BSCALEINC, BSWEEPANGLE);
Dial Volts(&Display, VXCENTER, VYCENTER, VDIAMETER, VLOWSCALE , VHIGHSCALE, VSCALEINC, VSWEEPANGLE);

void setup() {

  Serial.begin(9600);

  // you know the drill
  pinMode(LED_PIN, OUTPUT);

  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(C_BLACK);
  digitalWrite(LED_PIN, 255);

  // initialize the dials
  Bits.init(BNEEDLECOLOR, BDIALCOLOR, BTEXTCOLOR, BTICCOLOR, "Bits", FONT_TITLE, FONT_DATA);
  Volts.init(VNEEDLECOLOR, VDIALCOLOR, VTEXTCOLOR, VTICCOLOR, "Volts", FONT_TITLE, FONT_DATA);

}

void loop() {

  // get some data
  bBits = analogRead(A4);
  bVolts = bBits * 3.3 / 1024;

  // update the dials
  Bits.draw(bBits);
  Volts.draw(bVolts);

  delay(150);


}
