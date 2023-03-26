#include <ILI9341_t3.h>
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>
#include <font_ArialBold.h>


// http://www.barth-dev.de/online/rgb565-color-picker/


#define C_WHITE   		0xFFFF
#define	C_BLACK   		0x0000
#define C_GREY       	0xC618
#define	C_BLUE    		0x001F
#define	C_RED     		0xF800
#define	C_GREEN   		0x07E0
#define C_CYAN    		0x07FF
#define C_MAGENTA 		0xF81F
#define C_YELLOW  		0xFFE0  
#define C_TEAL			0x0438       
#define C_ORANGE      	0xDC00          
#define C_PINK        	0xF81F
#define C_PURPLE		0x801F


#define C_LTGREY      	0xE71C  
#define C_LTBLUE		0x73DF    
#define C_LTRED       	0xFBAE
#define C_LTGREEN     	0x7FEE
#define C_LTCYAN		0x77BF
#define C_LTMAGENTA   	0xFBB7
#define C_LTYELLOW    	0xF7EE
#define C_LTTEAL		0x77FE  
#define C_LTORANGE    	0xFDEE
#define C_LTPINK      	0xFBBA
#define C_LTPURPLE		0xD3BF

//#define C_DKGREY      	0x2124
#define C_DKGREY      	0x52AA
#define C_DKBLUE      	0x080B
#define C_DKRED       	0x7800
#define C_DKGREEN     	0x03C2   
#define C_DKCYAN      	0x032F  
#define C_DKMAGENTA   	0x900B
#define C_DKYELLOW    	0x94A0
#define C_DKTEAL		0x0452
#define C_DKORANGE   	0x92A0         
#define C_DKPINK      	0x9009
#define C_DKPURPLE    	0x8012  
 
#define C_MDGREY      	0x7BCF  
#define C_MDBLUE		0x1016
#define	C_MDRED     	0xB000
#define	C_MDGREEN   	0x0584
#define C_MDCYAN    	0x04B6
#define C_MDMAGENTA 	0xB010
#define C_MDYELLOW      0xAD80	  
#define C_MDTEAL		0x0594     
#define C_MDORANGE      0xB340           
#define C_MDPINK        0xB00E
#define C_MDPURPLE		0x8816 



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
