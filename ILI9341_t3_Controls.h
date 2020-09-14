/*
The MIT License (MIT)

library writen by Kris Kasprzak

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

On a personal note, if you develop an application or product using this library
and make millions of dollars, I'm happy for you!
*/



#ifndef ILI9341_T3_CONTROLS_H
#define ILI9341_T3_CONTROLS_H

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

#define HANDLE_NONE 0
#define HANDLE_CIRCLE 1
#define HANDLE_SQUARE 2
#define HANDLE_TRIANGLE_1 3
#define HANDLE_TRIANGLE_2 4
#define HANDLE_TRIANGLE_3 5

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
#define C_DKGREY      	0x4A49

#define C_MDGREY      	0x7BCF

class BarChartH {

public:

	BarChartH(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc);

	void init(const char *Title, uint16_t TextColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );

	void setBarColor(uint16_t val = 0xF800);

	void plot(float val);

	void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);

	void showTitle(bool val);

	void showScale(bool val);

	


private:
		ILI9341_t3			*d;
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

		float	gx;
		float	gy;
		float	gw;
		float	gh;

		uint16_t tc;
		uint16_t rc;
		uint16_t bc;
		uint16_t ac;
		bool redraw;
		float stepval, range, TempY, level, i, data;
		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};


class BarChartV {

public:

	BarChartV(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc);

	void init(const char *Title, uint16_t TextColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor,const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont );

	void setBarColor(uint16_t val = 0xF800);

	void plot(float val);

	void setScale(float ScaleLow, float ScaleHigh, float ScaleInc);

	void showTitle(bool val);

	void showScale(bool val);

private:
		ILI9341_t3			*d;
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

		float	gx;
		float	gy;
		float	gw;
		float	gh;
		
		uint16_t tc;
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

	void showTitle(bool val);

	void showLegend(bool val);

	void showXScale(bool val);

	void showYScale(bool val);

private:

		ILI9341_t3			*d;
		ILI9341_t3_font_t	tf;
		ILI9341_t3_font_t	af;
		int ID = 0;
		float x, y;
		float	i, j;
		bool	st, sl, sxs, sys;
		float	Delta;
		int k;
		float	XLow, XHigh, XInc;
		float	YLow, YHigh, YInc;
		bool RedrawGraph = true;
		bool HaveFirstPoint = false;
		float		XPoint, YPoint, oXPoint, oYPoint[10], TextHeight;
		float		XDec = 0.0, YDec = 0.0;
		char	text[30];
		byte	oOrientation = 0;
		float	gx, gy, gw, gh;
		int StartPointX, StartPointY;
		char buf0[20], buf1[20], buf2[20], buf3[20], buf4[20], buf5[20], buf6[20], buf7[20], buf8[20], buf9[20];
		char    *dl[20] = {buf0, buf1, buf2, buf3, buf4, buf5, buf6, buf6, buf8, buf9};
		char	t[20];
		char	xa[20];
		char	ya[20];
		uint16_t tc;
		uint16_t dc[10];
		uint16_t ac;
		uint16_t gc;
		uint16_t bc;
		uint16_t pc;
	
		void drawGraph();
		float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);


};

class Dial {
public:
	Dial(ILI9341_t3 *disp, int CenterX, int CenterY, int DialRadius, float LowVal , float HiVal , float ValInc, float SweepAngle);
	
	void init(uint16_t NeedleColor, uint16_t DialColor, uint16_t TextColor, uint16_t TickColor, const char *Title, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &DataFont );

	void draw(float val);
		
private:
	
	bool Redraw = true;
	ILI9341_t3			*d;			
	ILI9341_t3_font_t	tf;
	ILI9341_t3_font_t	df;
	char t[10];
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

	SliderH(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t wide, uint16_t sliderColor, uint16_t backColor, uint16_t handleColor);		// class constructor
		
	void init(float scaleLow, float scaleHi, float scale = 0.0, float snap = 0.0);		// initializer
  
	void draw(float val);					// method to draw complete slider
		
	float slide(float x, float y);			// method to move handle as user drags finger over handle, this method automatically looks for a valid range press
  
	bool changed();

	void change();
	
	void setColors(uint16_t sliderColor, uint16_t backColor, uint16_t handleColor);		// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t handleColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void setHandleSize(int val);

	void setHandleShape(byte val);

	void drawSliderColor(bool val);

private:

	ILI9341_t3 *d;			// the display object
	uint16_t sColor;		// the slider color
	uint16_t bColor;		// the slider background color
	uint16_t hColor;		// the sliders drag handle
	uint16_t l;			// the left coordinate of the scale
	uint16_t t;			// the top coordinate of the scale
	uint16_t w;			// the width of the scale
	float ox;			// the old screen x value where user pressed
	float sl;				// the scale lower value
	float sh;				// the scale upper value
	float pos;				// the position on the scale
	float sn;				// the snap increment
	float sc;				// the scale increment
	float ce;				// the tick mark where zero is (for drawing heavy line on +/- scales
	float i;				// loop counter
	int handlesize;
	byte handleshape;
	bool ch;			 // flag if control was changed by user
	bool colorscale;		// flag to draw slider in handle color
	float MapFloat(float x, float fromLow, float fromHigh, float toLow, float toHigh); // why Arduino has no mapping for floats is beyond me, here it is...
	
	
  };

class SliderV {

 public:

	SliderV(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t high, uint16_t sliderColor, uint16_t backColor, uint16_t handleColor); // class constructor
  
	void init(float scaleLow, float scaleHi, float scale = 0.0, float snap= 0.0);	// initializer
   
	void draw(float val);						// method to draw complete slider
   
	float slide(uint16_t x, uint16_t y);	   // method to move handle as user drags finger over handle, this method automatically looks for a valid range press
    
	bool changed(); 

	void change();
	
	void setColors(uint16_t sliderColor, uint16_t backColor, uint16_t handleColor);	// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t handleColor);		// method to just draw the handle (useful for showing handle in green for OK value

	void setHandleSize(int val);
	
	void setHandleShape(byte val);

	void drawSliderColor(bool val);

private:

	ILI9341_t3 *d;			// the display object
	uint16_t sColor;		// the slider color
	uint16_t bColor;		// the slider background color
	uint16_t hColor;		// the sliders drag handle
	uint16_t l;			// the left coordinate of the scale
	uint16_t t;			// the top coordinate of the scale
	uint16_t w;			// the with of the scale
	uint16_t h;			// the with of the scale
	uint16_t oy;			// the old screen y value where user pressed
	float sl;				// the scale lower value
	float sh;				// the scale upper value
	float pos;				// the screen coordinate position
	float sn;				// the snap increment
	float sc;				// the scale increment
	float ce;				// the tick mark where zero is (for drawing heavy line on +/- scales
	float i;				// loop counter
	byte tl;
	bool ch;			// flag if control was changed by user
	bool colorscale;		// flag to draw slider in handle color
	float MapFloat(float x, float fromLow, float fromHigh, float toLow, float toHigh);// why Arduino has no mapping for floats is beyond me, here it is...
	int tLen, tHi;
	int handlesize;
	byte handleshape;

  };

class SliderOnOff {

 public:
	
	SliderOnOff(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t width, uint16_t height, uint16_t sliderColor, uint16_t backColor, uint16_t OnColor, uint16_t OffColor);// class constructor
  
	void draw(bool val);			// method to draw complete slider
   
	bool slide(float x,float y);	// method to move handle as user drags finger over handle, this method automatically looks for a valid range press

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

#endif