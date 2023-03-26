/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
  touchscreen lib           https://github.com/dgolda/UTouch

  BarChartH(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc);
  void init(const char *Title, uint16_t TextColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );
  void setBarColor(uint16_t val = 0xF800);
  void plot(float val);
  void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);
  void showTitle(bool val);
  void showScale(bool val);

*/


/*

  To implement a button is 4 lines of code, look for the Step x below

*/
#include <ILI9341_t3.h>
// step 1 include the library
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
#define B_HIGHSCALE 4.0
#define B_SCALEINC 1.0


#define V_XORIGIN 50
#define V_YORIGIN 150
#define V_WIDE 200
#define V_HEIGHT 50
#define V_LOWSCALE 0
#define V_HIGHSCALE 4.0
#define V_SCALEINC .5

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

#define BORDERCOLOR C_WHITE
#define BACKCOLOR C_BLACK


ILI9341_t3 Display(TFT_CS, TFT_DC);

int a7Bits, a8Bits;
float a7Volts, a8Volts;

// step 2 create an object for each bar chart
BarChartH A7Volts(&Display );
BarChartH A8Volts(&Display );

void setup() {
  Serial.begin(9600);

  pinMode(TFT_CS,  OUTPUT);
  pinMode(TFT_DC,  OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(ILI9341_BLACK);
  digitalWrite(LED_PIN, HIGH);

  // step 3 initialize
  A7Volts.init(B_XORIGIN, B_YORIGIN, B_WIDE, B_HEIGHT, B_LOWSCALE, B_HIGHSCALE, B_SCALEINC, "A7 Voltage", C_WHITE, BORDERCOLOR, C_RED, C_DKRED, BACKCOLOR, FONT_TITLE, FONT_DATA);
  // these methods are optional but let you see what you can do
  // A7Volts.ShowScale(false);
  // A7Volts.ShowTitle(false);
  // A7Volts.Plot(0);


  A8Volts.init(V_XORIGIN, V_YORIGIN, V_WIDE, V_HEIGHT, V_LOWSCALE, V_HIGHSCALE, V_SCALEINC, "A8 Voltage", C_WHITE, BACKCOLOR, C_BLUE, C_DKBLUE, BACKCOLOR, FONT_TITLE, FONT_DATA);
  // these methods are optional but let you see what you can do
  A8Volts.showScale(false);
  // A8Volts.showTitle(false);
  // A8Volts.plot(0);

}

void loop() {

  a7Bits = analogRead(A7);
  a8Bits = analogRead(A8);

  a7Volts = a7Bits * 3.3 / 1024;
  a8Volts = a8Bits * 3.3 / 1024;
  
  // step 4 draw
  A7Volts.draw(a7Volts);
  A8Volts.draw(a8Volts);
  delay(50);

}
