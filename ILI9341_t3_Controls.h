/*
The MIT License (MIT)

library writen by Kris Kasprzak

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permissionotice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. InO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

On a personal note, if you develop an application or product using this library
and make millions of dollars, I'm happy for you!

rev		date			author				change
1.0		9/2019			kasprzak			initial code
2.0		9/2020			kasprzak			added shapes and sizes for handles
3.0		10/2020			kasprzak			fixed some repaint issues in CGraph, added marker support, added Button class with tons of methods
4.0		11/2020			kasprzak			fixed bugs added Button, Checkbox, OptionButton classes
5.0		11/2020			kasprzak			modified sliders, option and check to return true/false if pressed, and actual value stored in value property
5.1		11/2020			kasprzak			added automatic "blank out" old handle support insided draw() method in sliderH and SliderV (really needed when a slide is redrawn based on input other than a finger slide (encoder)
5.4		12/2021			kasprzak			added ring sliders 
5.5 		11/2022			kasprzak			added better text centering control
6.2 		3/2025			kasprzak			initialize default graph count to 0, and added #define MAX_GRAPHS 10 
7.0 		11/2022			kasprzak			added bar segment option for horizontal and vertical bar gauges
8.0 		6/2025			kasprzak			added arc shape bar segment class and made segment bar charts dividers actual and no a percentage
8.1 		6/2025			kasprzak			simplified bar segment api's
*/



#ifndef ILI9341_t3_CONTROLS_H
#define ILI9341_t3_CONTROLS_H

#define  ILI9341_t3_CONTROLS_VER 8.1

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#ifdef __cplusplus
#include "Arduino.h"
#endif

#include <ILI9341_t3.h>   


#define G_REPAINT 0
#define G_DRAWOVER 1
#define BELOW 0
#define ABOVE 1

#define SLIDER_HANDLE_SIZE 16
#define HANDLE_NONE 0
#define HANDLE_CIRCLE 1
#define HANDLE_SQUARE 2
#define HANDLE_TRIANGLE_1 3
#define HANDLE_TRIANGLE_2 4
#define HANDLE_TRIANGLE_3 5
#define HANDLE_RECTANGLE 6

#define LOCATION_TOP 0
#define LOCATION_BOTTOM 1

#define DISABLED 0
#define ENABLED 1

#define MAX_GRAPHS 10

/*
#define	C_BLACK   		0x0000
#define	C_BLUE    		0x001F
#define	C_RED     		0xF800
#define	C_GREEN   		0x07E0
#define C_CYAN    		0x07FF
#define C_MAGENTA 		0xF81F
#define C_YELLOW  		0xFFE0  
#define C_WHITE   		0xFFFF

#define C_LTBLUE		0xB6DF    
#define C_LTTEAL		0xBF5F  
#define C_LTGREEN     	0xBFF7
#define C_LTCYAN		0xC7FF
#define C_LTRED       	0xFD34
#define C_LTMAGENTA   	0xFD5F
#define C_LTYELLOW    	0xFFF8
#define C_LTORANGE    	0xFE73
#define C_LTPINK      	0xFDDF
#define C_LTPURPLE		0xCCFF
#define C_LTGREY      	0xE71C  

#define C_TEAL			0x0438       
#define C_ORANGE      	0xFD20          
#define C_PINK        	0xF81F
#define C_PURPLE		0x801F
#define C_GREY       	0xC618
 
#define C_DKBLUE      	0x000D
#define C_DKTEAL		0x020C
#define C_DKGREEN     	0x03E0      
#define C_DKCYAN      	0x03EF      
#define C_DKRED       	0x6000
#define C_DKMAGENTA   	0x8008
#define C_DKYELLOW    	0x8400
#define C_DKORANGE   	0x8200         
#define C_DKPINK      	0x9009
#define C_DKPURPLE    	0x4010
#define C_DKGREY      	0x3186

#define C_MDGREY      	0x7BCF
 */




#define B_PRESSED true
#define B_RELEASED false
#define TFT_DEBOUNCE 100  // debounce delay to minimize screen repress

#define CORNER_AUTO   -1
#define CORNER_SQUARE  0

#define  MAX_OPTION 20
#define OPTION_BUTTON_RADIUS 10

#define CHECKBOX_SIZE 20

#define C_DISABLE_LIGHT 0xC618
#define C_DISABLE_MED	0x7BCF
#define C_DISABLE_DARK	0x3186
#define MAXCHARLEN 30


class BarChartA {

public:

	BarChartA(ILI9341_t3 *Display);

	void init(uint16_t ArcRadius, uint16_t ArcCenterY, uint16_t BarWidth, uint16_t OffsetFromTop, uint16_t SweepAngle, uint8_t Segments, float ScaleLow, float ScaleHigh);

	void draw(float Value);

	void setScale(float ScaleLow, float ScaleHigh);	

	void setBars(uint16_t SweepAngle, uint16_t Segments, uint16_t BarWidth, float GapSize);
	
	void setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV);
	
	void setSectionSize(float Divider1, float Divider2);

private:
	ILI9341_t3 			*d;
	float p1x= 0.0f, p1y= 0.0f, p2x= 0.0f, p2y= 0.0f, p3x= 0.0f, p3y= 0.0f, p4x= 0.0f, p4y= 0.0f, drawangle= 0.0f;
	uint16_t rad = 800, xoffset = 160, yoffset = 500, topoffset = 30;
	uint16_t sweepangle = 22;
	float startangle = 0.0f;
	uint16_t segments = 44;
	float width = 20, gap = 0.003;
	float arcangle = 0;
	uint16_t i = 0, barcolor;
	float low = 0.0f, high = 1024.0f;
	uint16_t	bars = 0;
	uint16_t barwidth = 0;
	uint8_t	divider = 0;
	float tempval = 0.0f;
	uint16_t color_l = 0x07E0;
	uint16_t color_m = 0xFFE0;
	uint16_t color_h = 0xF800;
	uint16_t color_v = 0x52AA;
	float divider_1= 0.5f;
	float divider_2= 0.75f;
	float divider1= 0.5f;
	float divider2=  0.75f;
	float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};


class BarChartH {

public:

	BarChartH(ILI9341_t3 *Display);

	// init for bar chart
	void init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc, const char *Title, uint16_t TextColor, uint16_t BorderColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );
	
	// init for bar chart with segments
	void init(float GraphXLoc, float GraphYLoc, float GraphHeight, float ScaleLow, float ScaleHigh, uint16_t NumberOfBars, float BarWidth, uint8_t DividerSize, const char *Title, uint16_t TextColor, uint16_t ColorLow, uint16_t ColorMed, uint16_t ColorHigh, uint16_t ColorNull,const ILI9341_t3_font_t &TitleFont );

	void showScale(bool val);

	void setBarColor(uint16_t val = 0xF800);

	void draw(float val);

	void refresh();

	void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);	
	
	void setTitleText(const char *Title, const ILI9341_t3_font_t &TitleFont );	

	void showTitle(bool val);
	
	// allows bar segments as opposed to one large block, construct the object then override with these methods
	
	void setBars(uint16_t Left, uint16_t Top,uint8_t BarWidth, uint16_t NumberofBars,uint8_t DividerSize);
	void setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV);
	void setSectionSize(float Divider1, float Divider2);
	float getActualWidth();


private:
		ILI9341_t3 			*d;
		ILI9341_t3_font_t	tf;
		ILI9341_t3_font_t	sf;
		bool	st = true, ss = true;
		char	ti[20];
		char	sc[20];
		char	cc[2] = "D";
		char	text[30];
		float	XLow;
		float	XHigh;
		float	XInc;
		int Dec, tLen, tHi;
		float	Low;
		float	High;
		float	Inc;
		float	barinc = 0;
		uint16_t	bars = 0, stepval = 0;
		uint16_t barwidth = 0;
		uint16_t barcolor = 0;
		uint8_t	divider = 0;
		float	gx;
		float	gy;
		float	gw;
		float	gh;
		uint16_t color_l = 0;
		uint16_t color_m = 0;
		uint16_t color_h = 0;
		uint16_t color_v = 0;
		bool bartype = false;
		float divider_1= 0.0f;
		float divider_2= 0.0f;
		float divider1= 0.0f;
		float divider2= 0.0f;
		uint16_t cnt = 0;
		uint16_t tc;
		uint16_t oc;
		uint16_t rc;
		uint16_t bc;
		uint16_t ac;
		bool redraw;
		float range, TempY, level, i, data;
		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};


class BarChartV {

public:

	BarChartV(ILI9341_t3 *Display);

	void init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc, const char *Title, uint16_t TextColor, uint16_t BorderColor, uint16_t BarColor, uint16_t BarBlankColor, uint16_t BackgroundColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );


	// init for bar chart with segments
	void init(float GraphXLoc, float GraphYLoc, float GraphWidth, float ScaleLow, float ScaleHigh, uint16_t NumberOfBars, float BarHeight, uint8_t DividerSize, const char *Title, uint16_t TextColor, uint16_t ColorLow, uint16_t ColorMed, uint16_t ColorHigh, uint16_t ColorNull,const ILI9341_t3_font_t &TitleFont );
	
	
	void setBarColor(uint16_t val = 0xF800);

	void draw(float val);
	
	void refresh();

	void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);

	void showTitle(bool val);

	void showScale(bool val);
	
	// allows bar segments as opposed to one large block, construct the object then override with these methods
	void setBars(uint16_t Left, uint16_t Top,uint8_t BarHeight, uint16_t NumberofBars,uint8_t DividerSize);
	void setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV);
	void setSectionSize(float Divider1, float Divider2);
	float getActualHeight();

private:

		ILI9341_t3 			*d;
		ILI9341_t3_font_t	tf;
		ILI9341_t3_font_t	sf;
		bool	st = true, ss = true;
		char	ti[20];
		char	sc[20];
		char	cc[2] = "D";
		char	text[30];
		float	XLow;
		float	XHigh;
		float	XInc;
		int Dec, tLen, tHi;
		float	Low;
		float	High;
		float	Inc;
		float	barinc;
		uint16_t	bars = 0;
		uint8_t barheight = 0;
		uint16_t barcolor = 0;
		uint8_t	divider = 0;
		uint16_t color_l = 0;
		uint16_t color_m = 0;
		uint16_t color_h = 0;
		uint16_t color_v = 0;
		bool bartype = false;
		float divider_1= 0.0f;
		float divider_2= 0.0f;
		float divider1= 0.0f;
		float divider2= 0.0f;
		uint16_t cnt = 0;
		float	gx;
		float	gy;
		float	gw;
		float	gh;
		
		uint16_t tc;
		uint16_t oc;
		uint16_t rc;
		uint16_t bc;
		uint16_t ac;
		bool redraw;
		float stepval, range, TempY, level, i, data;
		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};

class CGraph {

public:

	CGraph(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float XAxisLow, float XAxisHigh, float XAxisInc, float YAxisLow, float YAxisHigh, float YAxisInc);

	void init(const char *Title, const char *XAxis, const char *YAxis, uint16_t TextColor, uint16_t GridColor, uint16_t AxisColor, uint16_t BackColor,uint16_t PlotkColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &AxisFont );

	void plot(int ID, float y);

	void setX(float x);

	int add(const char * DataLabel, uint16_t DataColor );

	void setYAxis(float Ylow, float YHigh, float YInc);

	void setXAxis(float XAxisLow, float XAxisHigh, float XAxisInc);

	void showTitle(bool val);

	void showLegend(bool val);

	void showAxisLabels(bool val);

	void drawLegend(byte Location);
	
	void resetStart(int ID);
	
	void showXScale(bool val);
	
	void setXTextOffset(int val);
	
	void setYTextOffset(int val);
	
	void setYLegendOffset(int val);
		
	void setXTextScale(float val);
	
	void showYScale(bool val);

	void setMarkerSize(int ID, byte val);

	void setLineThickness(int ID, byte val);

	void setTitle(const char *Title);

	void setXAxisName(const char *Name);

	void setYAxisName(const char *Name);

	void drawGraph();

private:

		ILI9341_t3 			*d;
		ILI9341_t3_font_t	tf;
		ILI9341_t3_font_t	af;
		int ID = 0;
		float x, y;
		float	i, j;
		bool	st, sl, sal, sxs, sys;
		float	Delta;
		int k;
		float	XLow, XHigh, XInc;
		float	YLow, YHigh, YInc;
		float XTextScale;
		bool RedrawGraph = true;
		bool HaveFirstPoint[10];
		float	XPoint, YPoint, oXPoint[10], oYPoint[10], TextHeight;
		float	XDec = 0.0, YDec = 0.0;
		char	text[30];
		byte	oOrientation = 0;
		float	gx, gy, gw, gh;
		int StartPointX, StartPointY, XScaleOffset, YScaleOffset, YlegendOffset;
		char buf0[20], buf1[20], buf2[20], buf3[20], buf4[20], buf5[20], buf6[20], buf7[20], buf8[20], buf9[20];
		char    *dl[20] = {buf0, buf1, buf2, buf3, buf4, buf5, buf6, buf6, buf8, buf9};
		char	title[40];
		byte tl = 0; // title location
		char	xatitle[40];
		char	yatitle[40];
		uint16_t tc;
		uint16_t dc[10];
		uint16_t ac;
		uint16_t gc;
		uint16_t bc;
		uint16_t pc;
		byte pdia[20];
		byte linet[20];
		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);

};

class Dial {
public:
	Dial(ILI9341_t3 *disp, int CenterX, int CenterY, int DialRadius, float LowVal , float HiVal , float ValInc, float SweepAngle);
	
	void init(uint16_t NeedleColor, uint16_t DialColor, uint16_t TextColor, uint16_t TickColor, const char *Title, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &DataFont );

	void draw(float val);
		
private:
	
	bool Redraw = true;
	ILI9341_t3 			*d;			
	ILI9341_t3_font_t	tf;
	ILI9341_t3_font_t	df;
	char t[40];
	int cx;
	int cy;
	int dr;
	float lv;
	float hv;
	float inc;
	float sa;
	uint16_t nc;
	uint16_t dc;
	uint16_t tc;
	uint16_t ic;
	float degtorad;
	float offset, stepval, angle, data;
	float i;

	//variables to track new needle values
	float ix;
	float iy;
	float ox;
	float oy;
	float tx;
	float ty;
	float dx;
	float dy;
	float lx;
	float rx;
	float ly;
	float ry;
	int tLen, tHi;
	char buf[38];
	int dec;
	// variables to track previous needle values
	float px;
	float py;
	float pix;
	float piy;
	float plx;
	float ply;
	float prx;
	float pry;

};

class SliderH {

 public:

	SliderH(ILI9341_t3 *Display);		// class constructor
		
	void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, float ScaleLow, float ScaleHi, float Scale, float Snap, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);		// initializer
  
	void draw(float val);					// method to draw complete slider
		
	bool slide(float ScreenX, float ScreenY);			// method to move handle as user drags finger over handle, this method automatically looks for a valid range press
  
	void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);		// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t HandleColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setDisableColor(uint16_t HandleColor, uint16_t SliderColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setHandleSize(int size);

	void setHandleSize(int size, int width);

	void disable();

	void enable();

	void show();

	void hide();

    void setBarThickness(byte Thickness);

	void setHandleShape(byte shape);

	void drawSliderColor(bool color);

	void setPressDebounce(byte Debounce);

	void resetScale(float ScaleLow, float ScaleHi, float Scale, float Snap);

	float value;

private:

	ILI9341_t3 *d;			// the display object
	uint16_t sColor;		// the slider color
	uint16_t bColor;		// the slider background color
	uint16_t hColor;		// the sliders drag handle
	uint16_t dsColor;
	uint16_t dhColor;
	uint16_t tsColor;
	uint16_t ssColor;
	uint16_t thColor;
	float x;
	float y;
	uint16_t l;			// the left coordinate of the scale
	uint16_t t;			// the top coordinate of the scale
	uint16_t w;			// the width of the scale
	byte bt;			// the thockness of the bar
	float ox;			// the old screen x value where user pressed
	bool enabled;
	float sl;				// the scale lower value
	float sh;				// the scale upper value
	float pos;				// the position on the scale
	float sn;				// the snap increment
	float sc;				// the scale increment
	float ce;				// the tick mark where zero is (for drawing heavy line on +/- scales
	float i;				// loop counter
	int handlesize;
	int handlewidth;	
	byte handleshape;
	bool visible;
	bool colorscale;		// flag to draw slider in handle color
	float MapFloat(float x, float fromLow, float fromHigh, float toLow, float toHigh); // why Arduino has no mapping for floats is beyond me, here it is...
	byte debounce;
	
  };

class SliderV {

 public:

	SliderV(ILI9341_t3 *Display); // class constructor
  
	void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderH, float ScaleLow, float ScaleHi, float scale, float snap, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);	// initializer
   
	void draw(float val);						// method to draw complete slider
   
	bool slide(uint16_t ScreenX, uint16_t ScreenY);	   // method to move handle as user drags finger over handle, this method automatically looks for a valid range press
    
	void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);	// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t HandleColor);		// method to just draw the handle (useful for showing handle in green for OK value

	void setHandleSize(int val);

	void setHandleSize(int size, int width);
	
	void setHandleShape(byte val);

	void drawSliderColor(bool val);

	void setDisableColor(uint16_t HandleColor, uint16_t SliderColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setScale(float ScaleLow, float ScaleHi, float scale = 0.0, float snap= 0.0);

	void setBarThickness(byte Thickness);

	void disable();

	void enable();

	void show();

	void hide();

	void setPressDebounce(byte Debounce);

	float value;

private:

	ILI9341_t3 *d;			// the display object
	uint16_t sColor;		// the slider color
	uint16_t bColor;		// the slider background color
	uint16_t hColor;		// the sliders drag handle
	uint16_t dsColor;
	uint16_t dhColor;
	uint16_t tsColor;
	uint16_t thColor;
	uint16_t x;			// the left coordinate of the scale
	uint16_t y;			// the top coordinate of the scale
	uint16_t l;			// the left coordinate of the scale
	uint16_t t;			// the top coordinate of the scale
	uint16_t w;			// the with of the scale
	uint16_t h;			// the with of the scale
	byte bt;			// the thockness of the bar
	float oy;			// the old screen y value where user pressed
	float sl;				// the scale lower value
	float sh;				// the scale upper value
	float pos;				// the screen coordinate position
	float sn;				// the snap increment
	float sc;				// the scale increment
	float ce;				// the tick mark where zero is (for drawing heavy line on +/- scales
	float i;				// loop counter
	byte tl;
	bool colorscale;		// flag to draw slider in handle color
	float MapFloat(float x, float fromLow, float fromHigh, float toLow, float toHigh);// why Arduino has no mapping for floats is beyond me, here it is...
	int tLen, tHi;
	int handlesize;
	int handlewidth;
	byte handleshape;
	bool enabled;
	bool visible;
	byte debounce;
  };

class SliderOnOff {

 public:
	
	SliderOnOff(ILI9341_t3 *display, uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, uint16_t SliderH, uint16_t SliderColor, uint16_t BackColor, uint16_t OnColor, uint16_t OffColor);// class constructor
  
	void draw(bool state);			// method to draw complete slider
   
	bool slide(float ScreenX,float ScreenY);	// method to move handle as user drags finger over handle, this method automatically looks for a valid range press

	bool changed();						// method to return if state change, useful for determining if a something should be done but not done unless state change

	bool getValue();
     
private:

	ILI9341_t3 *_d;			// the display object
	uint16_t _sColor;		// the slider color
	uint16_t _bColor;		// the slider background color
	uint16_t _onColor;		// the sliders on color
	uint16_t _offColor;		// the sliders on color
	uint16_t _l;				// the left coordinate of the scale
	uint16_t _t;				// the top coordinate of the scale
	uint16_t _w;				// the with of the scale
	uint16_t _h;				// the with of the scale
	bool _pos;				// the screen coordinate position
	bool _changed;			//flag to track if button was just changed
  };



/*

Button class inspired by Adafruit, but added several methods

*/

class Button {
public:
    Button(ILI9341_t3 *Display) {d = Display; }

// text on buttons
	void init(int16_t ButtonX, int16_t ButtonY, uint8_t ButtonWidth, uint8_t ButtonHeight,
		uint16_t OutlineColor, uint16_t ButtonColor, uint16_t TextColor, uint16_t BackgroundColor,
		const char *ButtonText, int16_t TextOffsetX, int16_t TextOffsetY, const ILI9341_t3_font_t &TextFont ) {

		x = ButtonX;
		y = ButtonY;
		w = ButtonWidth;
		h = ButtonHeight;
		outlinecolor = OutlineColor;
		fillcolor = ButtonColor;
		textcolor = TextColor;
		backcolor = BackgroundColor;
		disablecolorfill = C_DISABLE_LIGHT;
		disablecolortext = C_DISABLE_DARK;
		x_offset = TextOffsetX;
		y_offset = TextOffsetY;
		strncpy(label, ButtonText, 20);
		f = TextFont;
		ct = CORNER_AUTO;
		bt = 4;
		drawit = true;
		enabled = true;
		visible = true;
		debounce = TFT_DEBOUNCE;
		value = 0; // user controlled for whatever....
		newcorner = false;
		Has565Icon = false;
		HasMonoIcon = false;
		

	}
	
	// color 565 icon, no text

	void init(int16_t ButtonX, int16_t ButtonY, uint8_t ButtonWidth, uint8_t ButtonHeight,
		uint16_t OutlineColor, uint16_t ButtonColor, uint16_t TextColor, uint16_t BackgroundColor,
		const uint16_t  *UpIcon, int16_t IconWidth, int16_t IconHeight, int OffsetLeft, int OffsetTop ) {

		x = ButtonX;
		y = ButtonY;
		w = ButtonWidth;
		h = ButtonHeight;
		outlinecolor = OutlineColor;
		fillcolor = ButtonColor;
		backcolor = BackgroundColor;
		disablecolorfill = C_DISABLE_LIGHT;
		disablecolortext = C_DISABLE_DARK;
		x_offset = OffsetLeft;
		y_offset = OffsetTop;
		ct = CORNER_AUTO;
		Has565Icon = true;
		HasMonoIcon = false;
		upicon = UpIcon;
		iconw = IconWidth;
		iconh = IconHeight;
		bt = 4;
		drawit = true;
		enabled = true;
		visible = true;
		debounce = TFT_DEBOUNCE;
		value = 0; // user controlled for whatever....
		newcorner = false;

	}
	

		// mono icon, no text
	
		void init(int16_t ButtonX, int16_t ButtonY, uint8_t ButtonWidth, uint8_t ButtonHeight, 
		uint16_t OutlineColor, uint16_t ButtonColor, uint16_t TextColor, uint16_t BackgroundColor, 
		const unsigned char  *Icon, int16_t IconWidth, int16_t IconHeight, int OffsetLeft, int OffsetTop ) {

		x = ButtonX;
		y = ButtonY;
		w = ButtonWidth;
		h = ButtonHeight;
		outlinecolor = OutlineColor;
		fillcolor = ButtonColor;
		textcolor = TextColor;
		backcolor = BackgroundColor;
		disablecolorfill = C_DISABLE_LIGHT;
		disablecolortext = C_DISABLE_DARK;
		x_offset = OffsetLeft;
		y_offset = OffsetTop;
		ct = CORNER_AUTO;
		Has565Icon = false;
		HasMonoIcon = true;
		monoicon = Icon;
		iconw = IconWidth;
		iconh = IconHeight;
		bt = 4;
		drawit = true;
		enabled = true;
		visible = true;
		debounce = TFT_DEBOUNCE;
		value = 0; // user controlled for whatever....
		newcorner = false;

	}

	void draw(bool b_pressed = false) {

		uint16_t outline, text, fill;

		if (!visible) {
			return;
		}

		if (!b_pressed) {
			drawit = true;
			fill = fillcolor;
			outline = outlinecolor;
			text = textcolor;
		} 
		else {
			if (drawit == false) {
				return;
			}

			drawit = false;
			fill =  outlinecolor;
			outline = outlinecolor;
			text = textcolor;
		}

		if (newcorner){
			newcorner = false;
		}

		if (!enabled) {

			if (ct == CORNER_AUTO){
				d->fillRoundRect(x - (w/2)+(bt/2), y - (h/2)+(bt/2), w-bt, h-bt, min(w,h)/4, disablecolorfill);
				d->drawRoundRect(x - (w/2)+(bt/2), y - (h/2)+(bt/2), w-bt, h-bt, min(w,h)/4-(bt/2), disablecolortext);
			}
			else if(ct == CORNER_SQUARE) {
				d->fillRect(x - (w/2), y - (h/2), w, h, disablecolorfill);
				d->drawRect(x - (w/2)+(bt/2), y - (h/2)+(bt/2), w-bt, h-bt, disablecolortext);
			}
			else {
				d->fillRoundRect(x - (w/2), y - (h/2), w, h, ct, disablecolorfill);
				d->drawRect(x - (w/2)+(bt/2), y - (h/2)+(bt/2), w-bt, h-bt, disablecolorfill);
			}
			
			d->setTextColor(disablecolortext);	

		}
		else{
						
			if (ct == CORNER_AUTO){			
				d->fillRoundRect(x - (w/2)+(bt/2), y - (h/2)+(bt/2), w-bt, h-bt, min(w,h)/4, fill);
				d->drawRoundRect(x - (w/2)+(bt/2), y - (h/2)+(bt/2), w-bt, h-bt, min(w,h)/4-(bt/2), outline);
			}
			else if(ct == CORNER_SQUARE) {
				d->fillRect(x - (w/2), y - (h/2), w, h, fill);
				d->drawRect(x - (w/2)+(bt/2), y - (h/2)+(bt/2), w-bt, h-bt, outline);
			}
			else {
/*
				if (b_pressed){					
					// updated to make button smaller on press
					// blank put border
					d->fillRoundRect(x - (w/2), y - (h/2), w, h, ct, backcolor);
					// draw new border
					d->fillRoundRect(x - (w/2)+(bt/2), y - (h/2)+(bt/2), w-bt, h-bt, ct, outline);
					// fill it in
					d->fillRoundRect(x - (w/2)+(bt/1), y - (h/2)+(bt/1), w-(2*bt), h-(2*bt), ct-(bt/2), fill);
				}
				else {
					*/
					// draw border
					d->fillRoundRect(x - (w/2), y - (h/2), w, h, ct, outline);
					// fill it in
					d->fillRoundRect(x - (w/2)+(bt/1), y - (h/2)+(bt/1), w-(2*bt), h-(2*bt), ct-(bt/2), fill);
			//	}
			}

			d->setTextColor(text);

		}

		if ((!Has565Icon) && (!HasMonoIcon)){

			d->setFont(f);

			if ((x_offset == 0) && (y_offset == 0)){
				d->setCursor(x - (d->measureTextWidth(label)/2), y - (d->measureTextHeight(label)/2));
			}
			else {
				d->setCursor(x + x_offset , y + y_offset);
			}
			d->print(label);
		}
		else {

			if (!b_pressed) {
				if (enabled) {
					if (Has565Icon){
						draw565Bitmap(x - (w/2) + x_offset, y-(h/2)+ y_offset, upicon, iconw, iconh );
					}
					else if (HasMonoIcon){
						drawMonoBitmap(x - (w/2) + x_offset, y-(h/2)+ y_offset, monoicon, iconh, iconw, text );
					}
				}
				else {
					if (Has565Icon){
						draw565Bitmap(x - (w/2) + x_offset, y-(h/2)+ y_offset, upicon, iconw, iconh );
					}
					else if (HasMonoIcon){
						drawMonoBitmap(x - (w/2) + x_offset, y-(h/2)+ y_offset, monoicon, iconh, iconw, text );
					}
				}
			}
			else {

					if (Has565Icon){
						draw565Bitmap(x - (w/2) + x_offset, y-(h/2)+ y_offset, upicon, iconw, iconh );
					}
					else if (HasMonoIcon){
						drawMonoBitmap(x - (w/2) + x_offset, y-(h/2)+ y_offset, monoicon, iconh, iconw, text );
					}
			}
		}

				
	}

	bool press(int16_t ScreenX, int16_t ScreenY) {

		if ((!enabled) || (!visible)) {
			//delay(debounce);
			return false;
		}	

		if ((ScreenX < (x - w/2)) || (ScreenX > (x + w/2))) {
			return false;
		}
		if ((ScreenY < (y - h/2)) || (ScreenY > (y + h/2))) {
			return false;
		}
		//delay(debounce);	
		return true;

	}

	void show() {

		visible = true;
		draw();
	}


	void hide() {

		if (ct == CORNER_AUTO){
			d->fillRoundRect(x - (w/2), y - (h/2), w, h, min(w,h)/4,backcolor);
		}
		else if(ct == CORNER_SQUARE) {
			d->fillRect(x - (w/2), y - (h/2), w, h, backcolor);
		}
		else {
			d->fillRoundRect(x - (w/2), y - (h/2), w, h, ct, backcolor);
		}

		visible = false;
	}

	void disable() { 
		enabled = false;
		
	}
	void enable() { 
		enabled = true;
		
	}


	void resize(int16_t ButtonX, int16_t ButtonY, uint8_t ButtonW, uint8_t ButtonH) {
		//hide();
		//draw();
		//show();
		x = ButtonX;
		y = ButtonY;
		w = ButtonW;
		h = ButtonH;
	
	}

	void setColors(uint16_t OutlineColor, uint16_t ButtonColor, uint16_t TextColor, uint16_t BackgroundColor, uint16_t DisabledTextColor, uint16_t DisabledButtonColor) {
		
		outlinecolor = OutlineColor;
		fillcolor = ButtonColor;
		textcolor = TextColor;
		backcolor = BackgroundColor;
		disablecolorfill = DisabledButtonColor;
		disablecolortext = DisabledTextColor;
		
	}

	void setFont(int TextOffsetX, int TextOffsetY, const ILI9341_t3_font_t &TextFont) {
		x_offset = TextOffsetX;
		y_offset = TextOffsetY;
		f = TextFont;
	}

	void setText(const char *ButtonText) {
		strncpy(label, ButtonText, 20);
	}

	void setCornerRadius(int radius) {
		newcorner = true;
		ct = radius;
	}
	
	void setBorderThickness(byte BorderThickness) {
		newcorner = true;
		bt = BorderThickness;
	}
	
	bool isEnabled() { 
		return enabled;
	}

	bool isVisible() { 
		return visible;
	}

	void setPressDebounce(byte Debounce) { 
		debounce = Debounce;
	}
	

	int value;

private:
	ILI9341_t3 *d;
	ILI9341_t3_font_t f;
	
	void draw565Bitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {

	  uint32_t offset = 0;

	  int j, i;

	  for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
		  d->drawPixel(j + x, i + y, bitmap[offset]);
		  offset++;
		}
	  }

	}
	
	void drawMonoBitmap(uint16_t x, uint16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color) {

	  uint8_t sbyte = 0;
	  uint8_t byteWidth = 0;
	  uint16_t jj, ii;

	  byteWidth = (w + 7) / 8;

	  for (jj = 0; jj < h; jj++) {
		for (ii = 0; ii < w; ii++) {
		  if (ii & 7)  sbyte <<= 1;
		  else sbyte   = pgm_read_byte(bitmap + jj * byteWidth + ii / 8);
		  if (sbyte & 0x80) d->drawPixel(x + ii, y + jj, color);
		}
	  }
	}
	
	int16_t x, y;
	int16_t w, h;
	int16_t x_offset, y_offset;
	int16_t iconh, iconw;
	bool redraw;
	uint16_t outlinecolor, fillcolor, textcolor, backcolor, disablecolorfill, disablecolortext;
	char label[20];
	bool Has565Icon;
	bool HasMonoIcon;
	const uint16_t *upicon;
	const uint16_t *dnicon;
	const unsigned char *monoicon;
	bool drawit;
	bool enabled;
	int ct;
	bool visible;
	uint8_t debounce, bt;
	bool newcorner;


};


/*

Checkbox class

*/


class CheckBox {
public:
	CheckBox(ILI9341_t3 *Display) {d = Display; }

	void init(int16_t ButtonX, uint16_t ButtonY, uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const char *Text, const ILI9341_t3_font_t &TextFont ) {

		x = ButtonX ;
		y = ButtonY + CHECKBOX_SIZE;
		s = CHECKBOX_SIZE;
		ct = 3;
		oc = OutlineColor;
		uc = UPColor;
		dc = DownColor;
		tc = TextColor;
		bc = BackgroundColor;
		doc = C_DISABLE_LIGHT;
		duc = C_DISABLE_MED;
		ddc = C_DISABLE_DARK;
		dtc = C_DISABLE_MED;
		strncpy(label,Text, 60);
		tox = TextOffsetX;
		toy = TextOffsetY;
		f = TextFont;
		enabled = true;
		state = true; // true=up, false=down
		visible = true;
		value = -1;
		debounce = TFT_DEBOUNCE;

	}

	void draw(bool val) {

		uint16_t fill, outline, tcolor;

		if (!visible) {
			d->fillRoundRect(x, y-s, s, s, ct, bc);
			return;
		}
		if (val){
			// last exit was no press so restore val from internal saved state
			val = state;
		}
		state = val;
		// store the variable in the public variable
		value = val;
		if (state) {
			fill = uc;
			outline = oc;
			tcolor = tc;
		} 
		else {
			fill =  dc;
			outline = oc;
			tcolor = tc;
		}

		if (!enabled){
			if (state) {
				fill = duc;
				outline = doc;
				tcolor = dtc;
			} 
			else {
				fill =  ddc;
				outline = doc;
				tcolor = dtc;
			}
		}
	
		d->fillRoundRect(x, y-s, s, s, ct, fill);
		d->drawRoundRect(x, y-s, s, s, ct, outline);
		
		d->setFont(f);
		d->setTextColor(tcolor);
		
		if (toy == 0) {
			d->setCursor(x + tox+(s/2), y - (s/2)- (d->measureTextHeight(label)/2));
		}
		else {
			d->setCursor(x + tox+(s/2), y - s + toy);
		}
		
		d->print(label);
	}

	bool press(int16_t SceenX, int16_t ScreenY) {
		bool pressed = false;

		if ((!visible) || (!enabled)) {
			return pressed;
		}

		if (   ( (SceenX >= x) && (SceenX <= (x + s) )) && ((ScreenY >= y-s) && (ScreenY <= (y)))   ) {	
			state = !state;
			draw(state);
			//delay(debounce);
			// store the variable in the public variable
			value = state; 
			pressed = true;
		}
		return pressed;
	}

	void show() {
		visible = true;
	}


	void hide() {
		visible = false;
	}

	void disable() { 
		enabled = false;
	}
	void enable() { 
		enabled = true;
	}


	void resize(int16_t ButtonX, int16_t ButtonY, uint8_t Size) {
		hide();
		draw(state);
		x = ButtonX;
		y = ButtonY;
		s = Size;
		show();
		draw(state);
		
	}

	void setColors(uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t BackgroundColor, uint16_t DisableOutlineColor,  uint16_t DisableTextColor, uint16_t DisableUPColor, uint16_t DisableDownColor) {
		
		oc = OutlineColor;
		uc = UPColor;
		dc = DownColor;
		bc = BackgroundColor;
		doc = DisableOutlineColor;
		duc = DisableUPColor;
		ddc = DisableDownColor;
		dtc = DisableTextColor;
		
	}

	void setText(int TextOffsetX,int TextOffsetY, const char *Text, const ILI9341_t3_font_t &TextFont) {

		tox = TextOffsetX;
		toy = TextOffsetY;
		strncpy(label,Text, 60);
		f = TextFont;

	}

	void setCornerRadius(int val) {
		ct = val;

	}
		
	bool isEnabled() { 
		return enabled;
	}
	
	bool isVisibled() { 
		return visible;
	}

	void setPressDebounce(byte Debounce) { 
		debounce = Debounce;
	}


	bool value;

private:
	ILI9341_t3 *d;
	char label[60];
	ILI9341_t3_font_t f;
	int16_t x, y;
	uint16_t s, ct;
	uint16_t oc, uc, dc, bc, doc, duc, ddc, dtc, tc;
	bool state;
	int tox, toy;
	bool enabled;
	bool visible;
	byte debounce;
};


/*

Checkbox class

*/

class OptionButton {

	
public:
	OptionButton(ILI9341_t3 *Display) {d = Display; }

	void init(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, int16_t TextColor, uint16_t BackgroundColor, int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont) {

		r = OPTION_BUTTON_RADIUS;
		oc = OutlineColor;
		sc = SelectedColor;
		uc = UnSelectedColor;
		bc = BackgroundColor;
		tc = TextColor;
		dsc = C_DISABLE_MED;
		duc = C_DISABLE_DARK;
		doc = C_DISABLE_LIGHT;
		tox = TextOffsetX;
		toy = TextOffsetY;
		dtc = C_DISABLE_MED;
		f = TextFont;
		enabled = true;
		current = 0;
		ID = 0;
		visible = true;
		value = current;
		debounce = TFT_DEBOUNCE;
	}
	
	int add(uint16_t ButtonX, uint16_t ButtonY,const char *Text, float OptionValue = -32001 ) {

		strncpy(label[ID],Text, 60);
		
		x[ID] = ButtonX;
		y[ID] = ButtonY;
		// this next part gives users to specify what the value will be
		// you can defaul to the ID,or specify it, good use is if option ID=3 is pressed and you want value to be 24
		// very odd value but chances of someone wanting this val?
		if (OptionValue == -32001) {
			rv[ID] = ID;
		}
		else {
			rv[ID] = OptionValue;
		}

		ID++;
		return(ID-1);
		
	}


	void draw(int OptionID) {

		uint16_t toc, tsc, tuc, tcolor;

		current = OptionID;

		if (!visible) {

			for (i = 0; i < ID; i++){
				d->fillCircle(x[i]+r/2, y[i]+r/2, r, bc);
				d->setCursor(x[i] + tox, y[i] + toy);
				d->setFont(f);
				d->setTextColor(bc);
				d->print(label[i]);
			}
			return;
		}

		toc = oc;
		tsc =  sc;
		tuc =  uc;
		tcolor = tc;

		if (!enabled){
			toc = doc;
			tsc = dsc;
			tuc = duc;
			tcolor = dtc;
		}
		
		for (i = 0; i < ID; i++){
			if (i == current) {
				d->fillCircle(x[i]+r/2, y[i]+r/2, r, tsc);
				value =  rv[i];
				option =  i;
			}
			else {
				d->fillCircle(x[i]+r/2, y[i]+r/2, r, tuc);
			}
			d->drawCircle(x[i]+r/2, y[i]+r/2, r, toc);
			d->setCursor(x[i] + tox, y[i] + toy);
			d->setFont(f);
			d->setTextColor(tcolor);
			d->print(label[i]);
		}

		// store the variable in the public variable
		

	}

	bool press(uint16_t ScreenX, uint16_t ScreenY) {
		bool pressed = false;
		
			


		for (i = 0; i < ID; i++){
			if (  (   (ScreenX >= (x[i] - 2*r) ) && (ScreenX <= (x[i] +  2*r))  ) && (  (ScreenY >= (y[i] -  2*r)) && (ScreenY <= (y[i] +  2*r))    )       ) {
				current = i; 
				if ((!visible) || (!enabled)) {
					pressed = false;
				}
				else {
					value = rv[i];
					option =  i;
					pressed = true;
					break;
				}
			}
		}
		if (pressed){
		draw(current);
		}
		//delay(debounce);
		
		return pressed;	

	}

	void select(int val) {
		value = rv[val];
		option = val;
		current = val;
	}
	
	void show() {
		visible = true;
	}
	
	void hide() {
		visible = false;
	}

	void disable() { 
		enabled = false;
	}

	void enable() { 
		enabled = true;
	}

	void resize(byte radius) {
		// caution, only all this before first draw--code will not remember old size
		r = radius;
	}

	void setColors(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, uint16_t TextColor, uint16_t BackgroundColor, uint16_t DisableOutlineColor, uint16_t DisableSelColor, uint16_t DisableUnSelColor, uint16_t DisableTextColor) {
		
		oc = OutlineColor;
		sc = SelectedColor;
		uc = UnSelectedColor;
		tc = TextColor;
		bc = BackgroundColor;
		doc = DisableOutlineColor;
		dsc = DisableSelColor;
		duc = DisableUnSelColor;
		dtc = DisableTextColor;
				
	}

	void setFont(int TextOffsetX,int TextOffsetY, const ILI9341_t3_font_t &TextFont) {
		
		tox = TextOffsetX;
		toy = TextOffsetY;
		f = TextFont;
				
	}

	void setText(int ID, const char *Text) {
		strncpy(label[ID],Text, 60);
	}

	int selected() { 
		return current;
	}
	
	bool isEnable() { 
		return enabled;
	}

	bool isVisible() { 
		return visible;
	}

	void setPressDebounce(byte Debounce) { 
		debounce = Debounce;
	}

	float value;
	int option;

private:
	ILI9341_t3 *d;
	char label[MAX_OPTION][60];
	ILI9341_t3_font_t f;
	uint16_t x[MAX_OPTION], y[MAX_OPTION];
	float rv[MAX_OPTION];
	uint16_t r;
	int i, tox, toy;
	int ID;
	int current;
	uint16_t oc, sc, uc, bc, tc, doc, dsc, duc, dtc;
	bool enabled;
	bool visible;
	byte debounce;
};


class SliderD {

 public:

	SliderD(ILI9341_t3 *Display); // class constructor
  
	void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderR, float SweepAngle, float ScaleLow, float ScaleHi, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);	// initializer
   
	void draw(float val);						// method to draw complete slider
   
	bool slide(uint16_t ScreenX, uint16_t ScreenY);	   // method to move handle as user drags finger over handle, this method automatically looks for a valid range press
    
	void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor);	// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t HandleColor);		// method to just draw the handle (useful for showing handle in green for OK value

	void setHandleSize(int val);

	void drawSliderColor(bool val);

	void setDisableColor(uint16_t HandleColor, uint16_t SliderColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setScale(float ScaleLow, float ScaleHi);

	void setRingThickness(byte Thickness);

	void disable();

	void enable();

	void show();

	void hide();

	void setPressDebounce(byte Debounce);

	float value;
	bool state;

private:

	ILI9341_t3 *d;			// the display object
	uint16_t sColor;		// the slider color
	uint16_t bColor;		// the slider background color
	uint16_t hColor;		// the sliders drag handle
	uint16_t dsColor;
	uint16_t dhColor;
	uint16_t tsColor;
	uint16_t thColor;
	uint16_t x;			// the left coordinate of the scale
	uint16_t y;			// the top coordinate of the scale
	uint16_t r;			// the left coordinate of the scale
	float sa, as, ae; // sweep angle, start angle, end angle
	byte dt;
	float sl;				// the scale lower value
	float sh;				// the scale upper value
	float angle, oangle, hx, hy;				// the screen coordinate position
	float i;				// loop counter
	float dist;
	bool colorscale;		// flag to draw slider in handle color
	bool pressed = false;
	float MapFloat(float x, float fromLow, float fromHigh, float toLow, float toHigh);// why Arduino has no mapping for floats is beyond me, here it is...
	int handlesize;
	bool enabled;
	bool visible, redraw;
	byte debounce;
	void DrawRing(float start, float end, uint16_t color);
	void DrawHandle(float angle, uint16_t hColor, uint16_t sColor);
  };


#endif
