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



#include "ILI9341_t3_Controls.h"
#include <ILI9341_t3.h>     // fast display driver lib

/*

class for a horizontal type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/
SliderH::SliderH(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t wide, uint16_t sliderColor, uint16_t backColor, uint16_t handleColor)

{
	// map arguements to class variables
	_d = disp;				
	_l = left;
	_t = top;
	_w = wide;
	_sColor = sliderColor;
	_bColor = backColor;
	_hColor = handleColor;

}

/*

class for a vertical type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/

SliderV::SliderV(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t high, uint16_t sliderColor, uint16_t backColor, uint16_t handleColor)

{
	// map arguements to class variables
	_d = disp;
	_l = left;
	_t = top;
	_h = high;
	_sColor = sliderColor;
	_bColor = backColor;
	_hColor = handleColor;

}

/*

class for a simpel slider-type on off switch, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/
SliderOnOff::SliderOnOff(ILI9341_t3 *disp, uint16_t left, uint16_t top, uint16_t width, uint16_t height, uint16_t sliderColor, uint16_t backColor, uint16_t OnColor, uint16_t OffColor)

{
	// map arguements to class variables
	_d = disp;
	_l = left;
	_t = top;
	_w = width;
	_h = height;
	_sColor = sliderColor;
	_bColor = backColor;
	_onColor = OnColor;
	_offColor = OffColor;

}

/*

initializer for the vertical slider, here we pass in the scale values

note user can specify a scale (0 for none), and a snap capability (0 for none). You can draw a 0-100 scale with tick marks every 25 but snap to each integer (1,2,3...)

*/

void SliderV::init(float scaleLow, float scaleHi, float scale, float snap) {
    
	// map arguements to class variables
	_sl = scaleLow;
	_sh = scaleHi;
	_sc = 0.0;
	_sn = 0.0;
	_colorscale = false;
	// compute scale increments and snap increments
	if (scale != 0) {
		_sc = (_sh - _sl) / scale;
		_ce = abs(_sh / scale);
	}
	if (snap != 0) {
		_sn = (_sh - _sl) / snap;
	}

}
/*

initializer for the horizontal slider, here we pass in the scale values

note user can specify a scale (0 for none), and a snap capability (0 for none). You can draw a 0-100 scale with tick marks every 25 but snap to each integer (1,2,3...)


*/

void SliderH::init(float scaleLow, float scaleHi, float scale, float snap ) {
	// map arguements to class variables
	_sl = scaleLow;
	_sh = scaleHi;
	_sc = 0.0;
	_sn = 0.0;
	_colorscale = false;

	if (scale != 0) {
		_sc =  (_sh - _sl ) /  scale ;
		_ce = abs(_sl / scale);
	}
	if (snap != 0) {
		_sn = (_sh - _sl) / snap;
	}
	
}

/*

method to set colors, this is useful for disabling a slider by setting it to a dimmed color and ignoring screen presses

*/

void SliderH::setColors(uint16_t sliderColor, uint16_t backColor, uint16_t handleColor) {

	// map arguements to class variables
	_sColor = sliderColor;
	_bColor = backColor;
	_hColor = handleColor;

}

/*

method to set colors, this is useful for disabling a slider by setting it to a dimmed color and ignoring screen presses

*/

void SliderV::setColors(uint16_t sliderColor, uint16_t backColor, uint16_t handleColor) {

	// map arguements to class variables
	_sColor = sliderColor;
	_bColor = backColor;
	_hColor = handleColor;

}

/*

method to set hust the handle color, this is useful for drawing handle in warning color

*/

void  SliderH::setHandleColor(uint16_t handleColor) {

	_hColor = handleColor;

}

/*

method to set hust the handle color, this is useful for drawing handle in warning color

*/
void  SliderV::setHandleColor(uint16_t handleColor) {

	_hColor = handleColor;

}

/*

method to draw the horizontal slider

blank out old handle scale (needed if user calls draw to refresh the slider)
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original

*/
void  SliderH::draw(float val) {
	
		_d->fillCircle(_ox, _t, BALL_DIA, _bColor);
		_pos = mapF(val, _sl, _sh, (float) _l, (float) _l + (float) _w);
		_ox = _pos;
		
		if (_colorscale){
			_d->fillRect(_l, _t, _ox - _l, BALL_DIA / 2, _hColor);
			_d->fillRect(_ox, _t, _w - _ox + _l, BALL_DIA / 2, _sColor);

		}
		else{
			_d->fillRect(_l, _t, _w, BALL_DIA / 2, _sColor);
		}

		if (_sc != 0.0) {
			for (i = 0; i <= _sc; i++){
				
				_d->fillRect((i * (_w / _sc) ) + _l, _t-2, 1, BALL_DIA, _sColor);

				if ((i == _ce) | (i == 0) | (i == _sc)) {
					_d->fillRect((i * (_w / _sc)) + _l, _t - 2, 4, BALL_DIA, _sColor);
				}
			}
		}
		_changed = false;
		
		_d->fillCircle(_pos,_t,BALL_DIA,_hColor);
		_d->drawCircle(_pos,_t,BALL_DIA,_sColor);
		_d->drawCircle(_pos,_t,BALL_DIA-1,_sColor);

		_ox = _pos;
		_pos = val;
	
}

/*

method to draw the slider

blank out old handle scale (needed if user calls draw to refresh the slider)
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

void  SliderV::draw(float val) {
	
	_d->fillCircle(_l, _oy, BALL_DIA, _bColor);
	_pos = mapF(val, _sl, _sh, (float) _t + (float) _h, (float) _t );
	_oy = _pos;

	if (_colorscale){
		_d->fillRect(_l - BALL_DIA / 4, _t, BALL_DIA / 2, _oy - _t, _sColor); // draw new slider
		_d->fillRect(_l - BALL_DIA / 4, _oy, BALL_DIA / 2, _h - _oy + _t, _hColor); // draw new slider
	}
	else {
		_d->fillRect(_l - (BALL_DIA/4), _t, BALL_DIA / 2, _h, _sColor);
	}
	
	if (_sc != 0.0) {
		for (i = 0; i <= _sc; i++){

			_d->fillRect(_l - 5,(i * (_h / _sc) ) + _t, BALL_DIA, 1, _sColor);

			if ((i == _ce) | (i == 0) | (i == _sc)) {
				_d->fillRect(_l - 5, (i * (_h / _sc)) + _t, BALL_DIA, 4, _sColor);
			}
		}
	}
	_changed = false;
	
	_d->fillCircle(_l,_pos,BALL_DIA,_hColor);
	_d->drawCircle(_l,_pos,BALL_DIA,_sColor);
	_d->drawCircle(_l,_pos,BALL_DIA-1,_sColor);

	_oy = _pos;
	_pos = val;
	
		   
}



void  SliderOnOff::draw(bool val) {

	_pos = val;

	_d->fillRoundRect(_l, _t, _w, _h, _h / 2, _bColor);
	_d->drawRoundRect(_l, _t, _w, _h, _h / 2, _sColor);
	_d->drawRoundRect(_l + 1, _t + 1, _w - 2, _h, _h / 2, _sColor);
	

	if (val) {
		// draw on button
		_d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);
	}

	else {
		// draw off button
		_d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);

	}

	_changed = false;

}

/*

method to more the slider

optionally get the snap increment
see if the press location is in the controls range
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

float  SliderH::slide(float x,float y){

	// get snap increment
	if (_sn != 0.0 ) {
 		x = x + BALL_DIA;  
		x = x - _l;
		x =  (int) (x /  (_w / _sn));
		x = (x *  (_w / _sn)) + _l;
	}
	// draw ball and scale
	if (x != _ox){

		if ((x >= _l) & (x <= (_l + _w))) {

			if ((abs(y - _t)) <= BALL_DIA) {

				// it's in range of ball
				_d->fillCircle(_ox, _t, BALL_DIA, _bColor);

				if (_colorscale){
					_d->fillRect(_l, _t, x-_l, BALL_DIA / 2, _hColor);
					_d->fillRect(x, _t, _w - x + _l, BALL_DIA / 2, _sColor);

				}
				else{
					_d->fillRect(_l, _t, _w, BALL_DIA / 2, _sColor);
				}
						

				if (_sc != 0.0) {
					for (i = 0; i <= _sc; i++){
						_d->fillRect((i * (_w / _sc)) + _l, _t - 2, 1, BALL_DIA, _sColor);
						if ((i == _ce) | (i == 0) | (i == _sc)) {
							_d->fillRect((i * (_w / _sc)) + _l, _t - 2, 4, BALL_DIA, _sColor);
						}
					}
				}

				_changed = true;
				_d->fillCircle(x, _t, BALL_DIA, _hColor);
				_d->drawCircle(x, _t, BALL_DIA, _sColor);
				_d->drawCircle(x, _t, BALL_DIA - 1, _sColor);
				_ox = x;

				// get scaled val and pass back
				_pos = mapF(x, (float) _l, (float) _l + (float) _w, _sl, _sh);
				
			}
		}
	}


	return _pos;
}

/*

method to more the slider
optionally get the snap increment
see if screen press is in controls range
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

float  SliderV::slide(uint16_t x, uint16_t y){
	
	if (_sn != 0.0) {
		y = y - _t;
		y = (int) (y /  (_h / _sn));
		y = (y *  (_h / _sn)) + _t;
	}

	if (y != _oy){

		if (abs(x - (_l - BALL_DIA)) < BALL_DIA) {

			if ((y >= _t) & (y <= (_t + _h))) {
				
				// it's in rage of ball
				_d->fillCircle(_l, _oy, BALL_DIA, _bColor);  // blaok out ball

				if (_colorscale){
					_d->fillRect(_l - BALL_DIA / 4, _t, BALL_DIA / 2, y - _t, _sColor); // draw new slider
					_d->fillRect(_l - BALL_DIA / 4, y, BALL_DIA / 2,_h- y +_t , _hColor); // draw new slider
				}
				else {
					_d->fillRect(_l - BALL_DIA / 4, _t, BALL_DIA / 2, _h, _sColor); // draw new slider
				}
				
				if (_sc != 0.0) {
					for (i = 0; i <= _sc; i++){

						_d->fillRect(_l - 5, (i * (_h / _sc)) + _t, BALL_DIA, 1, _sColor);

						if ((i == _ce) | (i == 0) | (i == _sc)) {
							_d->fillRect(_l - 5, (i * (_h / _sc)) + _t, BALL_DIA, 4, _sColor);
						}

					}
				}
				_changed = true;
				_d->fillCircle(_l, y, BALL_DIA, _hColor);
				_d->drawCircle(_l, y, BALL_DIA, _sColor);
				_d->drawCircle(_l, y, BALL_DIA - 1, _sColor);
				_oy = y;

				// get scaled val and pass back
				_pos = mapF(y, (float) _t, (float) _t + (float) _h, _sh, _sl);
				
			}
		}
	}
	
	return _pos;
}

/*

method to more the on/off slider

see if the screen press is in the controls range
optionally get the snap increment
draw the scale
draw optional tick marks
get the actual scaled value and map to the appropriate screen position
draw the handle in the scaled location
reset the position for redraw method
remember the postion for painting over the original


*/

bool SliderOnOff::slide(float x, float y){

	if ((x >= _l) & (x <= (_l + _w))) {
		if (abs(y - (_t + (_h / 2))) < _h) {

			_changed = true;

			// it's in range of slider ball
			if (x < (_l + (_w / 2) - (_h / 2))){
				// press in the off range
				if (_pos){

					// clear on button
					_d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _bColor);
					// draw off button
					_d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);
					_pos = false;
				}
			}
			else {
				// it's in the on range
				if (!_pos){
					// clear on button
					_d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _bColor);
					// draw off button
					_d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);

					_pos = true;
				}
			}

		}
	}

	return _pos;

}

/*

method to see of the user just turned the switch on or off

*/
bool SliderOnOff::changed(){
	if (_changed){
		_changed = false;
			return true;
	}
	return false;

}

/*

method to see of the user just turned the switch on or off

*/
bool SliderV::changed(){

	if (_changed){
		_changed = false;
		return true;
	}
	return false;

}

bool SliderH::changed(){

	if (_changed){
		_changed = false;
		return true;
	}
	return false;

}
void SliderH::drawSliderColor(bool val){

	_colorscale = val;

}

void SliderV::drawSliderColor(bool val){

	_colorscale = val;

}



/*

the mapF for mapping float values

*/

float SliderH::mapF(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

float SliderV::mapF(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}



