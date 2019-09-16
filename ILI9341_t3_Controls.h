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


#include <ILI9341_t3.h>   

#define BALL_DIA 10

class SliderH {

 public:

	SliderH(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t wide, uint16_t sliderColor, uint16_t backColor, uint16_t handleColor);		// class constructor
		
	void init(float scaleLow, float scaleHi, float scale = 0.0, float snap = 0.0);		// initializer
  
	void draw(float val);					// method to draw complete slider
		
	float slide(float x, float y);			// method to move handle as user drags finger over handle, this method automatically looks for a valid range press
  
	bool changed();
	
	void setColors(uint16_t sliderColor, uint16_t backColor, uint16_t handleColor);		// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t handleColor);	// method to just draw the handle (useful for showing handle in green for OK value

	void drawSliderColor(bool val);

private:

	ILI9341_t3 *_d;			// the display object
	uint16_t _sColor;		// the slider color
	uint16_t _bColor;		// the slider background color
	uint16_t _hColor;		// the sliders drag handle
	uint16_t _l;			// the left coordinate of the scale
	uint16_t _t;			// the top coordinate of the scale
	uint16_t _w;			// the width of the scale
	uint16_t _ox;			// the old screen x value where user pressed
	float _sl;				// the scale lower value
	float _sh;				// the scale upper value
	float _pos;				// the position on the scale
	float _sn;				// the snap increment
	float _sc;				// the scale increment
	float _ce;				// the tick mark where zero is (for drawing heavy line on +/- scales
	float i;				// loop counter
	bool _changed;			// flag if control was changed by user
	bool _colorscale;		// flag to draw slider in handle color
	float mapF(float x, float fromLow, float fromHigh, float toLow, float toHigh); // why Arduino has no mapping for floats is beyond me, here it is...
	
	
  };

class SliderV {

 public:

	SliderV(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t high, uint16_t sliderColor, uint16_t backColor, uint16_t handleColor); // class constructor
  
	void init(float scaleLow, float scaleHi, float scale = 0.0, float snap = 0.0);	// initializer
   
	void draw(float val);						// method to draw complete slider
   
	float slide(uint16_t x, uint16_t y);				// method to move handle as user drags finger over handle, this method automatically looks for a valid range press
    
	bool changed(); 
	
	void setColors(uint16_t sliderColor, uint16_t backColor, uint16_t handleColor);	// way to reset colors (useful for drawing enabled or disabled)

	void setHandleColor(uint16_t handleColor);		// method to just draw the handle (useful for showing handle in green for OK value
	
	void drawSliderColor(bool val);

private:

	ILI9341_t3 *_d;			// the display object
	uint16_t _sColor;		// the slider color
	uint16_t _bColor;		// the slider background color
	uint16_t _hColor;		// the sliders drag handle
	uint16_t _l;			// the left coordinate of the scale
	uint16_t _t;			// the top coordinate of the scale
	uint16_t _w;			// the with of the scale
	uint16_t _h;			// the with of the scale
	uint16_t _oy;			// the old screen y value where user pressed
	float _sl;				// the scale lower value
	float _sh;				// the scale upper value
	float _pos;				// the screen coordinate position
	float _sn;				// the snap increment
	float _sc;				// the scale increment
	float _ce;				// the tick mark where zero is (for drawing heavy line on +/- scales
	float i;				// loop counter
	bool _changed;			// flag if control was changed by user
	bool _colorscale;		// flag to draw slider in handle color
	float mapF(float x, float fromLow, float fromHigh, float toLow, float toHigh);// why Arduino has no mapping for floats is beyond me, here it is...
	

  };

class SliderOnOff {

 public:
	
	SliderOnOff(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t width, uint16_t height, uint16_t sliderColor, uint16_t backColor, uint16_t OnColor, uint16_t OffColor);// class constructor
  
	void draw(bool val);			// method to draw complete slider
   
	bool slide(float x,float y);	// method to move handle as user drags finger over handle, this method automatically looks for a valid range press

	bool changed();						// method to return if state change, useful for determining if a something should be done but not done unless state change
	
     
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
