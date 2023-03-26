/*

  MCU                       https://www.amazon.com/Teensy-3-2-with-pins/dp/B015QUPO5Y/ref=sr_1_2?s=industrial&ie=UTF8&qid=1510373806&sr=1-2&keywords=teensy+3.2
  Display                   https://www.amazon.com/Wrisky-240x320-Serial-Module-ILI9341/dp/B01KX26JJU/ref=sr_1_10?ie=UTF8&qid=1510373771&sr=8-10&keywords=240+x+320+tft
  display library           https://github.com/PaulStoffregen/ILI9341_t3
    available methods

    // required
  SliderH(ILI9341_t3 *Display);   // class constructor
  void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, float ScaleLow, float ScaleHi, float Scale, float Snap, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);    // initializer
  void draw(float val);         // method to draw complete slider
  float slide(float ScreenX, float ScreenY);       // method to move handle as user drags finger over handle,
  // automatically looks for a valid range press returns if control was clicked. use value property to get the selected option


  // optional
  bool changed();
  void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);   // way to reset colors (useful for drawing enabled or disabled)
  void setHandleColor(uint16_t HandleColor);  // method to just draw the handle (useful for showing handle in green for OK value
  void setDisableColor(uint16_t HandleColor, uint16_t SliderColor); // method to just draw the handle (useful for showing handle in green for OK value
  void setHandleSize(int size);
  void disable();
  void enable();
  void show();
  void hide();
  void setHandleShape(byte shape);
  void drawSliderColor(bool color);
  void setPressDebounce(byte Debounce);
  float value;

*/


/*

  Implementatin is 5 steps

*/
#include <ILI9341_t3.h>           // fast display driver lib
#include <XPT2046_Touchscreen.h>
// step 1 include the lib
#include <ILI9341_t3_Controls.h>  // custom control define file
#include <font_Arial.h>
#include <Colors.h>

#define FONT Arial_16
#define TFT_DC 9                  // DC pin on LCD
#define TFT_CS 10                 // chip select pin on LCD
#define LCD_PIN A9                // lcd pin to control brightness
#define T_CS      1
#define T_IRQ     0
// defines for locations
#define ROW0 30
#define ROW1 70
#define ROW2 110
#define ROW3 150
#define ROW4 190


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



byte RedByte = 70, GreenByte = 170, BlueByte = 210, BrightByte = 255;
int BtnX, BtnY;

char buf[22];
uint16_t color, oldcolor;

// create display and DS objects
ILI9341_t3 Display = ILI9341_t3(10, 9);

XPT2046_Touchscreen Touch(T_CS, T_IRQ);
TS_Point TP;

// step 2 create the objects
//SliderH Red(&Display, left, top, width, bar color, back color, ball color);
SliderH Bright(&Display);
SliderH Red(&Display);
SliderH Green(&Display);
SliderH Blue(&Display);


void setup() {

  Serial.begin(9600);

  //you know the drill
  pinMode(LCD_PIN, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  // step 3 initialize each slider
  // low limit, high limit, scale val (0 is off), snap value (0 is off))
  Bright.init (20, ROW1, 100, 55, 255,     0, 0, C_WHITE, C_BLACK, C_GREY);

  Red.init    (20, ROW2, 100, 0, 255, 127.5, 0, C_WHITE, C_BLACK, C_GREEN);
  Green.init  (20, ROW3, 100, 0, 255, 63.75, 0, C_WHITE, C_BLACK, C_BLUE);
  Blue.init   (20, ROW4, 100, 0, 255,     0, 0, C_WHITE, C_BLACK, C_RED);

  // fire up the display
  Display.begin();

  // fire up touch
  Touch.begin();
  Touch.setRotation(3); // you may need to adjust depending on your display 

  Display.invertDisplay(false);
  Display.fillScreen(C_BLACK);

  // turn on the display
  analogWrite(LCD_PIN, 255);

  // set some display settings
  Display.setRotation(1);
  Display.setTextSize(2);
  Display.setTextColor(C_WHITE, C_BLACK);
  Display.setFont(FONT);
  Display.setCursor(10 , 10 );
  Display.print(F("Color Picker"));

  // Step 4 draw each slider
  Red.draw(RedByte);
  Green.draw(GreenByte);
  Blue.draw(BlueByte);
  Bright.draw(BrightByte);

  color = Display.color565(RedByte, GreenByte, BlueByte);

  // draw a color box
  Display.drawRect(140, 80, 100, 100, C_WHITE);
  Display.drawRect(141, 81, 98, 98, C_WHITE);
  Display.fillRect(142, 82, 96, 96, color);

  sprintf(buf, "(%3d),%3d,%3d,%3d", color, RedByte, GreenByte, BlueByte);
  Display.setCursor(50 , 220 );
  Display.print(buf);

}


void loop() {

  // wait for touch
  if (Touch.touched()) {
    ProcessTouch();

    // how to enable disable a slider
    // example on how to change slider colors

    // Step 5 allow user to slide the control and proces results
    // if you really need to monitor if the control was clicked
    // here's how, this may be needed if you have a complicated UI
    // and don't need to automatically change things--code here shows 2 cases
    // 1) automatically updates items based on slider (see if else sections, these
    // run even if slider was not pressed
    // and 2) only updating items if the slider was changed
    // do not use the return value as slider value
    // only use SliderV.value for the value

    bool changed = Bright.slide(BtnX, BtnY);
    // note if touch is not in range of slider, operation will be ignored
    // meaning you don't need to test press location--the lib does that
    Red.slide(BtnX, BtnY);
    Green.slide(BtnX, BtnY);
    Blue.slide(BtnX, BtnY);

    // update the color swatch
    // as mentioned above, you you don't want to process code unless slider was moved
    // wrap a change test around checkbox values
    // here only the band 1 screen text updates
    if (!changed) {
      color = Display.color565(Red.value, Green.value, Blue.value);
      Display.drawRect(140, 80, 100, 100, C_WHITE);
      Display.drawRect(141, 81, 98, 98, C_WHITE);
      Display.fillRect(142, 82, 96, 96, color);
      Display.fillRect(50, 220, 200, 50, C_BLACK);
      sprintf(buf, "(%3d),%3d,%3d,%3d", color, (int)Red.value, (int) Green.value, (int) Blue.value);
      Display.setCursor(50 , 220 );
      Display.print(buf);
    }
    // as mentioned above, you code processes regardless if slider was moved
    // here we reset brightness on ANY slider move--don't like it? add changed check to
    // only process if bright slider was moved
    analogWrite(LCD_PIN, Bright.value);
  }

}

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
