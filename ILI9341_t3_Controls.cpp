#include "ILI9341_t3_Controls.h"
#include <ILI9341_t3.h>     // fast display driver lib

float degtorad = .0174532778;

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////
arc shaped bar chart
*///////////////////////////////////////////////////////////////////////////////////////////////////////////

BarChartA::BarChartA(ILI9341_t3 *Display){

	d = Display;
		
}


void BarChartA::init(uint16_t ArcRadius, uint16_t ArcCenterX, uint16_t BarWidth, uint16_t OffsetFromTop, uint16_t SweepAngle, uint8_t Segments, float ScaleLow, float ScaleHigh){

	rad = ArcRadius;
	xoffset = ArcCenterX;
	barwidth = BarWidth;
	topoffset= OffsetFromTop;
	sweepangle= SweepAngle;
	segments= Segments;
	low = ScaleLow;
	high = ScaleHigh;

}

void BarChartA::draw(float Value){
	
	yoffset = rad + topoffset;
	
	// very weird but displays +y is down so as opposed to handling that transform when drawing
	// i'm just rotating the draw transform coordinates by 270 degrees
	// this works and still honors the dislay.setRotation(1-4)
	// and as a result segment #0 is the clockwise most segment--hence we draw CW so reverse the draw order
	startangle = (gap / 2) + ((270.0f + (sweepangle / 2.0f)) * degtorad);
	arcangle = ((float)(sweepangle / (float)segments) * degtorad) - gap;
  
  
  	if (divider_1 < low){
		divider_1 = low;
	}

	if (divider_2 > high){
		divider_2 = high;
	}

	divider1 = map(divider_1, low, high, 0, bars);		
	divider2 = map(divider_2, low, high, 0, bars);
	
	
	for (i = segments; i > 0; i--) {
		drawangle = ((float) startangle - ((float)arcangle + gap) * (float) i); 
		p1x = ((rad)*cos(drawangle)) + xoffset;
		p1y = ((rad)*sin(drawangle)) + yoffset;
		p2x = ((rad - barwidth) * cos(drawangle)) + xoffset;
		p2y = ((rad - barwidth) * sin(drawangle)) + yoffset;
		p3x = ((rad - barwidth) * cos(drawangle + arcangle)) + xoffset;
		p3y = ((rad - barwidth) * sin(drawangle + arcangle)) + yoffset;
		p4x = ((rad)*cos(drawangle + arcangle)) + xoffset;
		p4y = ((rad)*sin(drawangle + arcangle)) + yoffset;

		// again another hack to get the colors drawing colockwise for increased Value
		// due to CCW most segment is the max segment count, but lowest scaled value
		tempval = MapFloat(Value, low, high, (float) segments + 1.0f, 0.0f);

		if (i > (divider1)) {
		  if (i > tempval) {
			barcolor = color_l;
		  } else {
			barcolor = color_v;
		  }
		} else if (i > (divider2)) {
		  if (i > tempval) {
			barcolor = color_m;
		  } else {
			barcolor = color_v;
		  }
		} else {
		  if (tempval < i) {
			barcolor = color_h;
		  } else {
			barcolor = color_v;
		  }
		}
		// display does not have a 4 point square so we're drawing 2 triangles
		// luckily the overlap between the triangles leaves no stray pixels
		d->fillTriangle(p1x, p1y, p2x, p2y, p3x, p3y, barcolor);
		d->fillTriangle(p3x, p3y, p4x, p4y, p1x, p1y, barcolor);
	}
	
}

// wanna reset the scale?
void BarChartA::setScale(float ScaleLow, float ScaleHigh){
	
	low = ScaleLow;
	high = ScaleHigh;
	
}

// need a different sweep angle, # of drawn segments, etc?
void BarChartA::setBars(uint16_t SweepAngle, uint16_t Segments, uint16_t BarWidth, float GapSize){
	
	sweepangle= SweepAngle;
	segments= Segments;
	barwidth = BarWidth;
	gap = GapSize;
	
}

// default is low=green, med=yellow, high=red and null is dark grey, change with this method
void BarChartA::setSectionColors(uint16_t ColorL, uint16_t ColorM, uint16_t ColorH, uint16_t ColorV){
	
	color_l= ColorL;
	color_m= ColorM;
	color_h = ColorH;	
	color_v = ColorV;
	
}

// default is 50 and 75% note Divider2 is not the difference from Divider2 but from 0. 
void BarChartA::setSectionSize(float Divider1, float Divider2){
	
	divider_1= Divider1;
	divider_2= Divider2;
		
}

// internal map--I sure wish Arduino platform did NOT make map with long ints
float BarChartA::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  if (in_min < 0) {
    in_max = in_max + abs(in_min);
    in_min = 0.0;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


horizontal bar chart


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

BarChartH::BarChartH(ILI9341_t3 *Display){

	d = Display;
		
}


void BarChartH::init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc, const char *Title, uint16_t TextColor, uint16_t BorderColor, uint16_t BarColor, uint16_t BarBColor, uint16_t BackColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont ){

	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	gx = GraphXLoc;

	gy = GraphYLoc;
	gw = GraphWidth;
	gh = GraphHeight;
	
	divider = 1; // distance between bars (we are just going to reduce width)
	bars = (High - ScaleLow) / ScaleInc;	
	barwidth = gw / bars;	
	
	strncpy(titxt, Title, 40);
	bartype = false;
	tf = TitleFont;
	sf = ScaleFont;
	tc = TextColor;
	oc = BorderColor;
	rc = BarColor;
	ac = BarBColor;
	bc = BackColor;

	redraw = true;

}

void BarChartH::draw(float val){
	
	stepval = (High-Low) / Inc;
		
	if (redraw == true) {
		redraw = false;		   

		// step val basically scales the hival and low val to the height
		// deducting a small value to eliminate round off errors
		// this val may need to be adjusted
		cnt = 0;
		
		if (ss){
			
			d->setFont(sf);

			// paint over previous y scale
			tHi = sf.cap_height * 2 + 8;
			d->fillRect(gx-10, gy + gh + 1, gw+30, tHi, bc);
			d->setTextColor(tc, bc);

			for (i = Low; i <= High; i += Inc) {

				d->drawFastVLine((cnt*barwidth) + gx, gy + gh+ 1,  5, tc);
				// draw lables
				if (High < .1) {
					Dec = 3;
				}
				else  if (High <= 1) {
					Dec = 2;
				}
				else  if (High <= 10) {
					Dec = 1;
				}
				else   {
					Dec = 0;
				}
				data =  i ;

				dtostrf(data, 0, Dec,text);
				tLen = d->strPixelLen(text) * 1.2;
				tHi =sf.cap_height;
				d->setCursor((cnt*barwidth) + gx - (tLen / 2) , gy + gh + 10);
				cnt++;
				d->print(text);
			}
		}

		if(st){
			d->setTextColor(tc, bc);
			d->setFont(tf);
			tHi = sf.cap_height * 2 + 12;
			d->setCursor(gx , gy -tHi );
			d->print(titxt);
		}

	}
	
	// compute level of bar graph that is scaled to the  height and the hi and low vals

	
  // draw the bar graph
  // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update
  
  if (!bartype){
	  
		if (val >= High) {
			val = High;
		}
		if (val <= Low) {
			val = Low;
		}
	  level = MapFloat( val, Low, High, gx, gx+gw);

	d->fillRect(level, gy + 1, gx+gw - level, gh - 2,ac);
	d->fillRect(gx, gy + 1 , level - gx,  gh - 2, rc);
	d->drawRect(gx , gy, gw, gh, oc);
  }
  else {
  
	stepval = MapFloat(val, Low, High, 0.0f, (float) bars);	 
	
	if (divider_1 < Low){
		divider_1 = Low;
	}

	if (divider_2 > High){
		divider_2 = High;
	}

	divider1 = map(divider_1, Low, High, 0, bars);		
	divider2 = map(divider_2, Low, High, 0, bars);
	
	Serial.print(bars);
	Serial.print(", ");
	Serial.print(Low);
	Serial.print(", ");
	Serial.print(High);
	Serial.print(", ");	
	Serial.print(divider_1);
	Serial.print(", ");
	Serial.print(divider_2);
	Serial.print(", ");
	Serial.print(divider1);
	Serial.print(", ");
	Serial.print(divider2);
	Serial.println();
	
		
		
	for (i = 1; i < bars; i++){		

		if (i <= (divider1)){
			if (i <= stepval){
				barcolor = color_l;
			}
			else {
				barcolor = color_v;
			}
		}
		else if (i <= (divider2)){
			if (i <= stepval){
				barcolor = color_m;
			}
			else {
				barcolor = color_v;
			}
		}
		else if (i <= bars){
			if (i <= stepval){
				barcolor = color_h;
			}
			else {
				barcolor = color_v;
			}
		}
		else {
			barcolor = color_v;
		}
		
		d->fillRect(gx + (i*(barwidth + divider)), gy, barwidth, gh,barcolor);
	}

  }

}

void BarChartH::setBarColor(uint16_t BarColor){	
	rc = BarColor;		
}

void BarChartH::refresh(){
	
	redraw = true;
		
}


void BarChartH::setScale(float ScaleLow, float ScaleHigh, float ScaleInc){
	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	redraw = true;
}

void BarChartH::showTitle(bool val){	
	st = val;		
}

void BarChartH::showScale(bool val){
	
	ss = val;
		
}

void BarChartH::useSegmentBars(bool val){
	bartype = val;
}
void BarChartH::setBars(uint16_t NumberofBars,float BarWidth, uint8_t DividerSize){
	divider = DividerSize;
	barwidth = BarWidth;	
	bars = NumberofBars;

}
void BarChartH::setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV){	
	color_l = ColorL;
	color_m = ColorM;
	color_h = ColorH;
	color_v = ColorV;	
}
void BarChartH::setSectionSize(float Divider1, float Divider2){	
	divider_1 = Divider1;
	divider_2 = Divider2;
}

void BarChartH::setSize(uint16_t Left, uint16_t Top, uint16_t Wide, uint16_t High, uint8_t Divider){	
	gx = Left;
	gy = Top;
	gw = Wide;
	gh = High;
	divider = Divider;	
}

float BarChartH::getBars(){	
	return bars;
}
float BarChartH::getActualWidth(){	
	return (barwidth+divider) * bars;
}

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


vertical bar chart


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

BarChartV::BarChartV(ILI9341_t3 *Display){

	d = Display;
		
}

void BarChartV::init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh, float ScaleInc, const char *Title, uint16_t TextColor,uint16_t BorderColor, uint16_t BarColor, uint16_t BarBlankColor,uint16_t BackgroundColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &ScaleFont ){

	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	gx = GraphXLoc;
	gy = GraphYLoc;
	gw = GraphWidth;
	gh = GraphHeight;
	strncpy(ti, Title, 40);
	bartype = false;
	divider = 1; // distance between bars (we are just going to reduce width)
	bars = (High - Low) / Inc;	
	barheight = gh / bars;	
	
	tf = TitleFont;
	sf = ScaleFont;
	tc = TextColor;
	oc = BorderColor;
	rc = BarColor;
	ac = BarBlankColor;
	bc = BackgroundColor;
	redraw = true;
	
}


void BarChartV::draw(float val){
	
	stepval = (High-Low) / Inc;

	if (redraw == true) {
		redraw = false;	   

		// step val basically scales the hival and low val to the height
		// deducting a small value to eliminate round off errors
		// this val may need to be adjusted
		cnt = 0;
		
		if (ss) {
			d->setFont(sf);
			stepval = (High - Low) / Inc;
			
			// paint over previous y scale
			d->fillRect(gx + gw, gy - gh - 15, 70, gh + 30, bc);
			d->setTextColor(tc, bc);
			for (i = 0; i <= stepval; i++) {
				
				TempY =  gy - ((gh / stepval) * i);

				d->drawFastHLine(gx + gw , TempY,  5, tc);
				data = Low + (Inc * i);
				// draw lables
		 
				if (Inc < .1) {
					Dec = 2;
				}
				else  if (Inc < 1) {
					Dec = 1;
				}
				else  if (Inc < 10) {
					Dec = 0;
				}
				else   {
					Dec = 0;
				}

				dtostrf(data, 0, Dec,text);
				tLen = d->strPixelLen(text) * 1.2;
				tHi =sf.cap_height;
				d->setCursor(gx + gw + 12, TempY - (tHi / 2) );
				d->print(text);
			}
		}
		if (st){
			d->setTextColor(tc, bc);
			d->setFont(tf);
			tHi =sf.cap_height * 2 + 5;
			d->setCursor(gx , gy - gh -tHi );
			d->print(ti);
		}
	}
	// compute level of bar graph that is scaled to the  height and the hi and low vals
	// this is needed to accompdate for +/- range
	if (!bartype){
		if (val >= High) {
			val = High;
		}
		if (val <= Low) {
			val = Low;
		}
		level = (gh * (((val - Low) / (High - Low))));

		// draw the bar graph
		// write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update

		d->fillRect(gx+1, gy - gh, gw - 2, gh - level, ac);
		d->fillRect(gx+1, gy - level , gw - 2,  level, rc);
		d->drawRect(gx , gy - gh - 1 , gw, gh+2, oc);
	}
	else {
		
		if (divider_1 < Low){
			divider_1 = Low;
		}

		if (divider_2 > High){
			divider_2 = High;
		}

		divider1 = map(divider_1, Low, High, 0, bars);		
		divider2 = map(divider_2, Low, High, 0, bars);

		stepval = MapFloat(val, Low, High, 0.0f, (float) bars);	
			
		for (int i = 1; i <= bars; i++){		

			if (i <= (divider1)){
				if (i <= stepval){
				barcolor = color_l;
				}
				else {
					barcolor = color_v;
				}
			}
			else if (i <= (divider2)){
				if (i <= stepval){
				barcolor = color_m;
				}
				else {
					barcolor = color_v;
				}
			}
			else if (i <= bars){
				if (i <= stepval){
				barcolor = color_h;
				}
				else {
					barcolor = color_v;
				}
			}
			else {
				barcolor = color_v;
			}		
			d->fillRect(gx , gy - (i*(barheight + divider)), gw, barheight, barcolor);

		}		
		
	}
}

void BarChartV::refresh(){
	
	redraw = true;
		
}

void BarChartV::setBarColor(uint16_t BarColor){
	
	rc = BarColor;
		
}

void BarChartV::showTitle(bool val){
	
	st = val;
		
}

void BarChartV::showScale(bool val){
	
	ss = val;
		
}


void BarChartV::setScale(float ScaleLow, float ScaleHigh, float ScaleInc){
	Low = ScaleLow;
	High = ScaleHigh;
	Inc = ScaleInc;
	redraw = true;
	gh = (barheight+divider) * bars;
}

void BarChartV::useSegmentBars(bool val){
	bartype = val;
}

void BarChartV::setBars(uint16_t NumberofBars,uint8_t BarHeight, uint8_t DividerSize){
	bars = NumberofBars;
	divider = DividerSize;
	barheight = BarHeight;	
	
}
void BarChartV::setSectionColors(uint16_t ColorL, uint16_t ColorM,uint16_t ColorH, uint16_t ColorV){	
	color_l = ColorL;
	color_m = ColorM;
	color_h = ColorH;
	color_v = ColorV;	
}

void BarChartV::setSectionSize(float Divider1, float Divider2){	
	
	divider_1 = Divider1;		
	divider_2 = Divider2;		
	
}

void BarChartV::setSize(uint16_t Left, uint16_t Top, uint16_t Wide, uint16_t High, uint8_t Divider){	
	gx = Left;
	gy = Top;
	gw = Wide;
	gh = High;
	divider = Divider;	
}

float BarChartV::getBars(){	
	return bars;
}
float BarChartV::getActualHeight(){	
	return (barheight+divider) * bars;
}
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


cartesian style graphing functions


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

CGraph::CGraph(ILI9341_t3 *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float XAxisLow, float XAxisHigh, float XAxisInc, float YAxisLow, float YAxisHigh, float YAxisInc){

		d = disp;

		XLow = XAxisLow;
		XHigh = XAxisHigh;
		XInc = XAxisInc;

		YLow = YAxisLow;
		YHigh = YAxisHigh;
		YInc = YAxisInc;

		gx = GraphXLoc;
		gy = GraphYLoc;
		gw = GraphWidth;
		gh = GraphHeight;
		
}


void CGraph::init(const char *Title, const char *XAxis, const char *YAxis, uint16_t TextColor, uint16_t GridColor, uint16_t AxisColor, uint16_t BackColor, uint16_t PlotColor, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &AxisFont ){

	strncpy(title, Title, 40);
	strncpy(xatitle, XAxis, 40);
	strncpy(yatitle, YAxis, 40);
	

	tf = TitleFont;
	af = AxisFont;
	tc = TextColor;

	gc = GridColor;
	ac = AxisColor;
	bc = BackColor;
	pc = PlotColor;

	st = true;
	sl = true;
	sal = true;
	sxs = true;
	sys = true;
	
	XScaleOffset = 5;
	YScaleOffset = 40;
	XTextScale = 1.0;
	YlegendOffset = 0;

	Delta = XHigh - XLow;

	RedrawGraph = true;

	TextHeight = tf.cap_height;
	ID = 0;
	}

int CGraph::add(const char *name, uint16_t color){
	
	// max number of plots is 10
	if (ID >= MAX_GRAPHS){
		return -1;
	}
	pdia[ID] = 0;
	linet[ID] = 1;	
	strncpy(dl[ID], name, 30);
	dc[ID] = color;
	HaveFirstPoint[ID] = false;
	ID++;	
		
	return ID-1;
	
}

void CGraph::setX(float xpoint){

		x = xpoint;

}

void CGraph::setLineColor(int ID, uint16_t LineColor){
	
	
	dc[ID] = LineColor;
	
}

void CGraph::setYLegendOffset(int val){
	
	YlegendOffset = val;
	
}

void CGraph::showAxisLabels(bool val){

		sal = val;

}

void CGraph::plot(int cID, float y){
	
	if (RedrawGraph){
		RedrawGraph = false;
		drawGraph();
	}
	
	// plot the data
	XPoint = MapFloat(x, XLow, XHigh, gx, gx + gw);
	YPoint = MapFloat(y, YHigh, YLow, gy - gh, gy);
	  
	if ((YPoint > gy)) {
		YPoint = gy;
	}
	if (YPoint < (gy - gh)) {
		YPoint = gy - gh;
	}
	

	if ((XPoint > gx) && (XPoint < gx + gw)) {
		if (HaveFirstPoint[cID]){
			for(j = 0; j < linet[cID]; j++){
				d->drawLine(oXPoint[cID], oYPoint[cID]+j, XPoint, YPoint+j, dc[cID]);
				
			}

			// d->drawLine(oXPoint[cID], oYPoint[cID], XPoint, YPoint, dc[cID]);
			if ( pdia[cID] > 1){
				d->fillCircle(XPoint, YPoint, pdia[cID],  dc[cID]);
				// d->fillRect(XPoint -  pdia[cID]/2, YPoint-  pdia[cID]/2, pdia[cID],   pdia[cID], dc[cID]);
			}
		}
	}

	HaveFirstPoint[cID] = true;
	oYPoint[cID] = YPoint;
	oXPoint[cID] = XPoint;

	// test to see if we need to redraw

	if (XPoint + 2 > gx + gw) {
		Delta = XHigh - (XLow);
		XLow = XHigh;
		XHigh = XHigh + Delta;
		RedrawGraph = true;
		HaveFirstPoint[cID] = false;
	
	}
 
}

void CGraph::setMarkerSize(int cID, byte val){

	pdia[cID] = val;

}

void CGraph::setLineThickness(int cID, byte val){
	
	linet[cID] = val;

}

void CGraph::setTitle(const char *Title){

	strncpy(title, Title, 40);

}

void CGraph::setXAxisName(const char *XAxis){

	strncpy(xatitle, XAxis, 40);

}

void CGraph::setXTextOffset(int val){
	
	XScaleOffset = val;

}

void CGraph::setYTextOffset(int val){
	
	YScaleOffset = val;
}

void CGraph::setXTextScale(float val){
	
	XTextScale = val;
}

void CGraph::setYAxisName(const char *YAxis){

	strncpy(yatitle, YAxis, 40);

}

void CGraph::resetStart(int ID){

	HaveFirstPoint[ID] = false;

}


void CGraph::drawLegend(byte Location){

	tl = Location;

}

void CGraph::drawGraph() {

	RedrawGraph = false;

	float xDiv =  ((XHigh-XLow)/XInc);
	float yDiv = ((YHigh-YLow)/YInc);



	float ylen = gh /  yDiv;
	float xlen = gw / xDiv;

	 d->setTextColor(tc, bc);

	// draw title
	if (st){
		d->setFont(tf);
		d->setCursor(gx, gy - gh - TextHeight-10);
		d->print(title);
	}

	// draw grid lines
	// first blank out xscale for redrawing
	d->fillRect(gx-10, gy+2, gw+20,25, bc);
	d->setFont(af);
	d->fillRect(gx, gy - gh-4, gw, gh+8, bc);
	d->fillRect(gx, gy - gh, gw, gh, pc);

	// draw vertical lines
	for (j = 0; j <= xDiv; j++) {
		if(j > 0){
			d->drawFastVLine(gx + ((0+j) * xlen), gy - gh, gh, gc);
		}
		if (xDiv < .1) {
			XDec = 2;
		}
		else if (xDiv < 1) {
			XDec = 1;
		}
		else {
			XDec = 0;
		}

		//get text offsets

		if (sxs){
			dtostrf((XLow+(XInc*j)) * XTextScale, 0, XDec,text);
			
			d->setCursor(gx + (j * xlen)- (d->measureTextWidth(text)/2), gy+XScaleOffset);
			
			d->print(text);
			
		}
	}

	d->fillRect(gx-30,  gy -gh-TextHeight+4, 27, gh+TextHeight, bc);

	// draw horizontal lines
	for (i = 0; i <= yDiv; i++) {

		d->drawFastHLine(gx, gy - (ylen * (0+i)), gw, gc);

		if (YInc < .1) {
			YDec = 2;
		}
		else if (YInc < 1) {
			YDec = 1;
		}
		else {
			YDec = 0;
		}
		if (sys){
			
			dtostrf(YLow+(YInc*i), 0, YDec,text);
			d->setCursor(gx-YScaleOffset, gy - (ylen * i)-(d->measureTextHeight(text)/2));
			
			//d->setCursor(gx + (j * xlen)- (d->measureTextWidth(text)/2), gy+XScaleOffset);
			
			
			d->print(text);

		}
	}

	// put the y axis at the zero point
	if ((YLow < 0) &&  (YHigh > 0)) {
		YPoint = MapFloat(0, YHigh, YLow, gy - gh, gy);
	}
	else {
		YPoint = MapFloat(YLow, YHigh, YLow, gy - gh, gy);
	}

	
	d->drawFastHLine(gx,  YPoint, gw, ac);
	d->drawFastHLine(gx,  YPoint-1, gw, ac);

	d->drawFastVLine(gx-1, gy - gh, gh+1, ac);
	d->drawFastVLine(gx-2, gy - gh, gh+1, ac);
	
	// draw legend
	if (sal){
		// draw y label
		oOrientation = d->getRotation();
		d->setTextColor(tc, bc);
		d->setRotation(oOrientation - 1);
		d->setCursor(d->width()-gy,gx-44);	
		d->print(yatitle);
		d->setRotation(oOrientation);
		//Serial.println(yatitle);

    	// draw x label
		d->setTextColor(tc, bc);
		d->setCursor(gx,gy+ TextHeight + 5);	
		d->print(xatitle);

		//Serial.println(xatitle);

	}
	if (sl) {
		// draw legend
		StartPointX = gx-20;
		
		StartPointX = gx + d->measureTextWidth(xatitle) + 10;
			
		if (tl == LOCATION_TOP){
			StartPointY = gy - gh;
		}
		else if (tl == LOCATION_BOTTOM) {
			StartPointY = gy+ TextHeight + YlegendOffset;	
		}

		for (k = 0; k <= ID; k++){
			d->setCursor(StartPointX, StartPointY);
			d->print(dl[k]);
			StartPointX = d->getCursorX();
			d->fillRect(StartPointX+3, StartPointY+TextHeight/4 , 20, linet[k], dc[k]);

			StartPointX += 30;
		}
		
	}
	
}

void CGraph::setYAxis(float Ylow, float Yhigh, float Yinc){

	YLow = Ylow;
	YHigh = Yhigh;
	YInc = Yinc;
	RedrawGraph = true;
}

void CGraph::setXAxis(float XAxisLow, float XAxisHigh, float XAxisInc){

	XLow = XAxisLow;
	XHigh = XAxisHigh;
	XInc = XAxisInc;
	RedrawGraph = true;
}

void CGraph::showTitle(bool val){
	st = val;
}

void CGraph::showLegend(bool val){

	sl = val;
}
void CGraph::showXScale(bool val){
	sxs = val;
}
void CGraph::showYScale(bool val){
	sys = val;
}




/*//////////////////////////////////////////////////////////////////////////////////////////////////////////


dial type control


*///////////////////////////////////////////////////////////////////////////////////////////////////////////

Dial::Dial(ILI9341_t3 *disp, int CenterX, int CenterY, int DialRadius, float LowVal , float HiVal , float ValInc, float SweepAngle){

	degtorad = .0174532778;

	d = disp;			
	
	cx = CenterX;
	cy = CenterY;
	dr = DialRadius;
	lv = LowVal;
	hv = HiVal;
	inc = ValInc;
	sa = SweepAngle;
	
	// store initial values
	// this looks silly but we don't have needle size calculated yet but to keep needle from first draw being weird, just make a dot at the center
	px = CenterX;
	py = CenterY;
	pix = CenterX;
	piy = CenterY;
	plx = CenterX;
	ply = CenterY;
	prx = CenterX;
	pry = CenterY;

	Redraw = true;

}

void Dial::init(uint16_t NeedleColor, uint16_t DialColor, uint16_t TextColor, uint16_t TickColor, const char *Title, const ILI9341_t3_font_t &TitleFont , const ILI9341_t3_font_t &DataFont ) {

	tf = TitleFont;
	df = DataFont;
	strncpy(t, Title, 10);
	nc = NeedleColor;
	dc = DialColor;
	tc = TextColor;
	ic = TickColor;
	
}

void Dial::draw(float Val) {


	// draw the dial only one time--this will minimize flicker
	if ( Redraw == true) {
		Redraw = false;
		d->fillCircle(cx, cy, dr , dc);
		d->drawCircle(cx, cy, dr, ic);
		d->drawCircle(cx, cy, dr - 1, ic);
	}

	// draw the current value
	d->setFont(df);
	d->setTextColor(tc, dc);
	d->setCursor(cx - 25, cy + 20 );
	//disp.println(Format(curval, dig, dec));
	// center the scale about the vertical axis--and use this to offset the needle, and scale text
	offset = (270 +  (sa / 2)) * degtorad;
	// find hte scale step value based on the hival low val and the scale sweep angle
	// deducting a small value to eliminate round off errors
	// this val may need to be adjusted
	stepval = (inc) * (sa / (float (hv - lv)));
	// draw the scale and numbers
	// note draw this each time to repaint where the needle was
		
	// first draw the previous needle in dial color to hide it
	d->fillTriangle (pix, piy,plx, ply, prx, pry, dc);

	for (i = 0.0; i <= (sa + 0.0001); i += stepval) {

		angle = ( i  * degtorad);
		angle = offset - angle ;
		ox =  (float) (dr - 2) * cos(angle) + cx;
		oy =  (float) (dr - 2) * sin(angle) + cy;
		ix =  (float) (dr - 10) * cos(angle) + cx;
		iy =  (float) (dr - 10) * sin(angle) + cy;
		tx =  (float) (dr - 30) * cos(angle) + cx;
		ty =  (float) (dr - 30) * sin(angle) + cy;
		dx =  (float) (dr - 20) * cos(angle) + cx;
		dy =  (float) (dr - 20) * sin(angle) + cy;

		d->drawLine(ox, oy, ix, iy, ic);

	
		if (hv < .1) {
			dec = 3;
		}
		else  if (hv <= 1) {
			dec = 2;
		}
		else  if (hv <= 10) {
			dec = 1;
		}
		else   {
			dec = 0;
		}

		data = hv - ( i * (inc / stepval)) ;
		dtostrf(data, 0, dec,buf);
		tLen = d->strPixelLen(buf);
		tHi = df.cap_height;
		d->setCursor(dx - (tLen/2), dy - (tHi/2));
	
		d->print(buf);
	

	}

	// compute and draw the needle
	angle = (sa * (1.0 - (((Val - lv) / (hv - lv)))));
	angle = angle * degtorad;
	angle = offset - angle  ;

	// draw a triangle for the needle (compute and store 3 vertiticies)
	// 5.0 is width of needle at center
	ix =  (float)(dr - 10.0) * cos(angle) + cx;
	iy =  (float)(dr - 10.0) * sin(angle) + cy;
	lx =  6.0 * cos(angle - 90.0 * degtorad) + cx;
	ly =  6.0 * sin(angle - 90.0 * degtorad) + cy;
	rx =  6.0 * cos(angle + 90.0 * degtorad) + cx;
	ry =  6.0 * sin(angle + 90.0 * degtorad) + cy;
		
	// then draw the new needle in need color to display it
	d->fillTriangle (ix, iy, lx, ly, rx, ry, nc);

	// draw a cute little dial center
	d->fillCircle(cx, cy, 8, tc);

	// print the title
	d->setTextColor(tc, dc);
	d->setFont(tf);
	tLen = d->strPixelLen(t);
			
	d->setCursor(cx - tLen/2, cy + 10);
	d->println(t);
	
	//save all current to old so the previous dial can be hidden
	pix = ix;
	piy = iy;
	plx = lx;
	ply = ly;
	prx = rx;
	pry = ry;

}





/*

class for a vertical type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/

SliderV::SliderV(ILI9341_t3 *Display)

{
	// map arguements to class variables
	d =Display;
	
	
}



/*

initializer for the vertical slider, here we pass in the scale values

note user can specify a scale (0 for none), and a snap capability (0 for none). You can draw a 0-100 scale with tick marks every 25 but snap to each integer (1,2,3...)

*/

void SliderV::init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderH, float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize,uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor ) {
    
	// map arguements to class variables
	sl = ScaleLow;
	sh = ScaleHi;
	sc = 0.0;
	sn = 0.0;
	oy = -1;
	colorscale = true;
	handlesize = SLIDER_HANDLE_SIZE;
	handlewidth = handlesize / 2;
	handleshape = HANDLE_CIRCLE;
	enabled = true;
	visible = true;
	l = SliderX;
	t = SliderY;
	h = SliderH;
	bt = 3;
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;
	debounce = TFT_DEBOUNCE;
	dsColor = C_DISABLE_MED;
	dhColor = C_DISABLE_LIGHT;

	// compute scale increments and snap increments
	if (ScaleSize != 0) {
		sc = (sh - sl) / ScaleSize;
		ce = abs(sh / ScaleSize);
	}
	if (SnapSize != 0) {
		sn = (sh - sl) / SnapSize;
	}

}


void SliderV::setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

	// map arguements to class variables
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;

}

/*

method to set hust the handle color, this is useful for drawing handle in warning color

*/

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

	
	if  (!visible) {

		if (!visible) {
			d->fillRect(l - handlesize , t - (handlesize / 2), handlesize * 2, h+ handlesize, bColor); // erase it
		}
		return;
	}

	value = val;

	if (enabled) {
		tsColor = sColor;
		thColor = hColor;
	}
	else {
		tsColor = dsColor;
		thColor = dhColor;
	}

	pos = MapFloat(value, (float) sl, (float)sh, (float)(t + h), (float)t );

	// blannk ouut any previous--note draw can be called w/o slide in cases where sliders are tied together
	
	if ((pos != oy) && (oy >= 0)) {
				// blank out the old one
			if (handleshape == HANDLE_CIRCLE) {
				d->fillCircle(l, oy, handlesize/2, bColor);
			}
			else if (handleshape == HANDLE_SQUARE) {
				d->fillRect(l - (handlesize / 2), oy- (handlesize / 2), handlesize, handlesize, bColor);
			}
			else if (handleshape == HANDLE_TRIANGLE_1) {
				// weird i know but need to draw the black out slightly larger due to round off errors
				d->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
			}
			else if (handleshape == HANDLE_TRIANGLE_2) {
				// weird i know but need to draw the black out slightly larger due to round off errors
				d->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
			}
			else if (handleshape == HANDLE_TRIANGLE_3) {
				// weird i know but need to draw the black out slightly larger due to round off errors
				d->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
				d->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
			}
			else if (handleshape == HANDLE_RECTANGLE) {
				// weird i know but need to draw the black out slightly larger due to round off errors
				d->fillRect(l - (handlesize / 2), oy- (handlewidth / 2), handlesize, handlewidth, bColor);
			}
				
	}

	if (colorscale){
		d->fillRect(l - (bt/2) , t, bt, pos - t, tsColor); // draw new slider
		d->fillRect(l - (bt/2) , pos, bt, h - pos + t, thColor); // draw new slider
	}
	else {
		d->fillRect(l - (bt/2), t, bt, h, tsColor);
	}
	
	if (sc != 0.0) {
		for (i = 0; i <= sc; i++){

			d->fillRect(l - 3,(i * (h / sc) ) + t, 7, 1, tsColor);

			if ((i == ce) || (i == 0) || (i == sc)) {
				d->fillRect(l - 3, (i * (h / sc)) + t, 7, 4, tsColor);
			}
		}
	}




	if (handleshape == HANDLE_CIRCLE) {
		d->fillCircle(l,pos,handlesize/2,thColor);
		d->drawCircle(l,pos,handlesize/2,tsColor);
		d->drawCircle(l,pos,(handlesize/2)-1,tsColor);
	}
	else if (handleshape == HANDLE_SQUARE) {
		d->fillRect(l - (handlesize / 2), pos- (handlesize / 2), handlesize, handlesize, thColor);
		d->drawRect(l - (handlesize / 2),pos- (handlesize / 2),handlesize, handlesize,tsColor);
		d->drawRect(l - (handlesize / 2) +1,pos- (handlesize / 2)+1,handlesize-2,handlesize-2,tsColor);
	}

	else if (handleshape == HANDLE_TRIANGLE_1) {
		d->fillTriangle(l - handlesize,	  pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , thColor);
		d->drawTriangle(l - handlesize,   pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_2) {
		d->fillTriangle(l + handlesize,	  pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , thColor);
		d->drawTriangle(l + handlesize,   pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_3) {
		d->fillTriangle(l - handlesize,	  pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , thColor);
		d->fillTriangle(l + handlesize,	  pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , thColor);
		d->drawTriangle(l - handlesize,   pos- (handlesize / 2), l - handlesize,   pos+ (handlesize / 2),l ,  pos , tsColor);
		d->drawTriangle(l + handlesize,   pos- (handlesize / 2), l + handlesize,   pos+ (handlesize / 2),l ,  pos , tsColor);
	}
	else if (handleshape == HANDLE_RECTANGLE) {
		d->fillRect(l - (handlesize / 2),   pos- (handlewidth / 2),  handlesize,  handlewidth,  thColor);
		d->drawRect(l - (handlesize / 2),   pos- (handlewidth / 2),  handlesize,  handlewidth,  tsColor);
		d->drawRect(l - (handlesize / 2) +1,pos- (handlewidth / 2)+1,handlesize-2,handlewidth-2,tsColor);
	}
	oy = pos;

	pos = value;
	
		   
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

bool SliderV::slide(uint16_t ScreenX, uint16_t ScreenY){
	
	bool pressed = false;

	x = ScreenX;
	y = ScreenY;

	if ((!enabled) || (!visible)) {
		//return pos;
		return pressed;
	}

	if (sn != 0.0) {
		y = y - t;
		y =  (y /  (h / sn));
		y = (y *  (h / sn)) + t;
	}

	if (y != oy){
		
		if (abs(x -l ) <= handlesize) {
			
			if ((y >= t) & (y <= (t + h))) {
				pressed = true;
	
				// it's in rage of ball

				// blank out the old one
				if (handleshape == HANDLE_CIRCLE) {
					d->fillCircle(l, oy, handlesize/2, bColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					d->fillRect(l - (handlesize / 2), oy- (handlesize / 2), handlesize, handlesize, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					d->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					d->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					// weird i know but need to draw the black out slightly larger due to round off errors
					d->fillTriangle(l - handlesize, oy- (handlesize / 2)-1, l - handlesize, oy+ (handlesize / 2)+1,l+1 , oy  , bColor);
					d->fillTriangle(l + handlesize, oy- (handlesize / 2)-1, l + handlesize, oy+ (handlesize / 2)+1,l-1 , oy  , bColor);
				}
				else if (handleshape == HANDLE_RECTANGLE) {
					d->fillRect(l - (handlesize / 2), oy- (handlewidth / 2), handlesize, handlewidth, bColor);
				}
				// draw slider 
				if (colorscale){
					d->fillRect(l - (bt/2) , t, bt, y - t, sColor); // draw new slider
					d->fillRect(l - (bt/2) , y, bt, h - y + t, hColor); // draw new slider
				}
				else {
					d->fillRect(l - (bt/2), t, bt, h, sColor);
				}
	
				// draw tick marks
				if (sc != 0.0) {
					for (i = 0; i <= sc; i++){

						d->fillRect(l - 3,(i * (h / sc) ) + t, 7, 1, sColor);

						if ((i == ce) | (i == 0) | (i == sc)) {
							d->fillRect(l - 3, (i * (h / sc)) + t, 7, 4, sColor);
						}
					}
				}
				// draw new handle
				if (handleshape == HANDLE_CIRCLE) {
					d->fillCircle(l,y,handlesize/2,hColor);
					d->drawCircle(l,y,handlesize/2,sColor);
					d->drawCircle(l,y,(handlesize/2)-1,sColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					d->fillRect(l - (handlesize / 2), y- (handlesize / 2), handlesize, handlesize, hColor);
					d->drawRect(l - (handlesize / 2),y- (handlesize / 2),handlesize, handlesize,sColor);
					d->drawRect(l - (handlesize / 2) +1,y- (handlesize / 2)+1,handlesize-2,handlesize-2,sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					d->fillTriangle(l - handlesize,	  y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					d->drawTriangle(l - handlesize,   y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					d->fillTriangle(l + handlesize,	  y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					d->drawTriangle(l + handlesize,   y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					d->fillTriangle(l - handlesize,	  y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					d->fillTriangle(l + handlesize,	  y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , hColor);
					d->drawTriangle(l - handlesize,   y- (handlesize / 2), l - handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
					d->drawTriangle(l + handlesize,   y- (handlesize / 2), l + handlesize, y+ (handlesize / 2) ,l ,  y , sColor);
				}
				else if (handleshape == HANDLE_RECTANGLE) {
					d->fillRect(l - (handlesize / 2),   y- (handlewidth / 2),  handlesize,   handlewidth, hColor);
					d->drawRect(l - (handlesize / 2),   y- (handlewidth / 2),  handlesize,   handlewidth,sColor);
					d->drawRect(l - (handlesize / 2) +1,y- (handlewidth / 2)+1,handlesize-2, handlewidth-2,sColor);
				}
				oy = y;

				// get scaled val and pass back and store in the public variable--in case anyone needs easy access...
				
				pos = MapFloat(y, t, (t + h), sh, sl);
				value = pos;
				delay(debounce);
			}
		}
	}
	//return pos;
	return pressed;
}


/*

method to set hust the handle color, this is useful for drawing handle in warning color

*/
void  SliderV::setHandleColor(uint16_t HandleColor) {

	hColor = HandleColor;

}

void SliderV::drawSliderColor(bool state){

	colorscale = state;

}

void SliderV::setBarThickness(byte Thickness){

	bt = Thickness;


}

void  SliderV::setDisableColor(uint16_t HandleColor, uint16_t SliderColor) {

	dsColor = SliderColor;
	dhColor = HandleColor;

}

void SliderV::setScale(float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize ) {
    
	// map arguements to class variables
	sl = ScaleLow;
	sh = ScaleHi;
	sc = 0.0;
	sn = 0.0;
	// compute scale increments and snap increments
	if (ScaleSize != 0) {
		sc = (sh - sl) / ScaleSize;
		ce = abs(sh / ScaleSize);
	}
	if (SnapSize != 0) {
		sn = (sh - sl) / SnapSize;
	}

}


void  SliderV::enable() {

	enabled = true;

}

void  SliderV::disable() {
	enabled = false;

}

void  SliderV::show() {

	visible = true;

}

void  SliderV::hide() {

	visible = false;
	
}

void SliderV::setHandleSize(int value){

	if (value < 4) {
		handlesize = 4;
	}
	else if (value > 100) {
		handlesize = 100;
	}
	else {
		handlesize = value;
	}

}

void SliderV::setHandleSize(int value, int width){

	if (value < 4) {
		handlesize = 4;
	}
	else if (value > 40) {
		handlesize = 40;
	}
	else {
		handlesize = value;
	}

	if (width < 4) {
		handlewidth = 4;
	}
	else if (width > 100) {
		handlewidth = 100;
	}
	else {
		handlewidth= width;
	}

}
void SliderV::setHandleShape(byte value){

	if (value < 0) {
		handleshape = HANDLE_CIRCLE;
	}
	else if (value > 6) {
		handleshape = HANDLE_CIRCLE;
	}
	else {
		handleshape = value;
	}

}
void SliderV::setPressDebounce(byte Debounce) { 
	debounce = Debounce;
}





/*

class for a horizontal type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/
SliderH::SliderH(ILI9341_t3 *Display)

{
	// map arguements to class variables
	d = Display;				



}

/*

initializer for the horizontal slider, here we pass in the scale values

note user can specify a scale (0 for none), and a snap capability (0 for none). You can draw a 0-100 scale with tick marks every 25 but snap to each integer (1,2,3...)


*/

void SliderH::init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor ) {
	// map arguements to class variables
	sl = ScaleLow;
	sh = ScaleHi;
	sc = 0.0;
	sn = 0.0;
	l = SliderX;
	t = SliderY;
	w = SliderW;
	bt = 3;
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;
	debounce = TFT_DEBOUNCE;
	dsColor = C_DISABLE_MED;
	dhColor = C_DISABLE_LIGHT;
	enabled = true;
	visible = true;
	ox = -1;
	colorscale = true;
	handlesize = SLIDER_HANDLE_SIZE;
	handlewidth = handlesize / 2;
	handleshape = HANDLE_CIRCLE;

	if (ScaleSize != 0) {
		sc =  (sh - sl ) /  ScaleSize ;
		ce = abs(sl / ScaleSize);
	}
	if (SnapSize != 0) {
		sn = (sh - sl) / SnapSize;
	}
	
}


void SliderH::resetScale(float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize) {

	sl = ScaleLow;
	sh = ScaleHi;
	sc = 0.0;
	sn = 0.0;
	if (ScaleSize != 0) {
		sc =  (sh - sl ) /  ScaleSize ;
		ce = abs(sl / ScaleSize);
	}
	if (SnapSize != 0) {
		sn = (sh - sl) / SnapSize;
	}

}


/*

method to set colors, this is useful for disabling a slider by setting it to a dimmed color and ignoring screen presses

*/

void SliderH::setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

	// map arguements to class variables
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;

}

/*

method to set colors, this is useful for disabling a slider by setting it to a dimmed color and ignoring screen presses

*/


void  SliderH::setHandleColor(uint16_t HandleColor) {

	hColor = HandleColor;

}

void  SliderH::setBarThickness(byte Thickness) {

	bt = Thickness;

}

void  SliderH::setDisableColor(uint16_t DisableHandleColor, uint16_t DisableSliderColor) {

	dsColor = DisableSliderColor;
	dhColor = DisableHandleColor;
}

void  SliderH::enable() {

	enabled = true;

}

void  SliderH::disable() {

	enabled = false;


}

void  SliderH::show() {

	visible = true;

}

void  SliderH::hide() {

	visible = false;

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
	
	if (!visible){

		if(!visible){
			d->fillRect(l - handlesize , t - handlesize, w + (2*handlesize), handlesize * handlesize, bColor);
		}
		// no need to draw anything
		return;
	}

	value = val;
	
	if (enabled) {
		tsColor = sColor;
		thColor = hColor;
		ssColor = sColor;
	}
	else {
		tsColor = dsColor;
		thColor = dhColor;
		ssColor = dsColor;
	}




	////////////////////////

	// draw the slider
	pos = MapFloat(value, (float) sl, (float)sh, (float) l, (float)(w+l) );
	//////////////////////////
	// seems odd to blank out old but of draw is called in .ino, need to clean up old stuff
	if ((ox != pos) && (ox > 0)){
		if (handleshape == HANDLE_CIRCLE) {
			d->fillCircle(ox, t, handlesize/2, bColor);
		}
		else if (handleshape == HANDLE_SQUARE) {
			d->fillRect(ox- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_1) {
			d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_2) {
			d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
		}
		else if (handleshape == HANDLE_TRIANGLE_3) {
			d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
			d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
		}
		else if (handleshape == HANDLE_RECTANGLE) {
			d->fillRect(ox- (handlewidth / 2), t - (handlesize / 2), handlewidth, handlesize, bColor);
		}
	}



	if (colorscale){
		d->fillRect(l, t-(bt/2), abs(pos-l), bt, thColor);
		d->fillRect(pos, t-(bt/2), abs(w - pos + l), bt, tsColor);
	}
	else{
		d->fillRect(l, t-(bt/2), w + 1, bt, ssColor);
	}

	// draw any tick marks
	if (sc != 0.0) {
		for (i = 0; i <= sc; i++){
				
			d->fillRect((i * (w / sc) ) + l, t-3, 1, 7, tsColor);

			if ((i == ce) | (i == 0) | (i == sc)) {
				d->fillRect((i * (w / sc)) + l-1, t - 3, 3, 7, tsColor);
			}
		}
	}
	
	
	// draw new handle
	if (handleshape == HANDLE_CIRCLE) {
		d->fillCircle(pos, t, handlesize/2, thColor);
		d->drawCircle(pos, t, handlesize/2,tsColor);
		d->drawCircle(pos, t, (handlesize/2) - 1,tsColor);
	}
	else if (handleshape == HANDLE_SQUARE) {
		d->fillRect(pos- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, thColor);
		d->drawRect(pos- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, tsColor);
		d->drawRect(pos- (handlesize / 2)+1, t - (handlesize / 2)+1, handlesize-2, handlesize-2, tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_1) {
		d->fillTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos,t, thColor);
		d->drawTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos,t, tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_2) {
		d->fillTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos,t, thColor);
		d->drawTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos,t, tsColor);
	}
	else if (handleshape == HANDLE_TRIANGLE_3) {
		d->fillTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos,t, thColor);
		d->fillTriangle(ox - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos,t, thColor);

		d->drawTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos,t, tsColor);
		d->drawTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos,t, tsColor);

	}
	else if (handleshape == HANDLE_RECTANGLE) {
		d->fillRect(pos- (handlewidth / 2)  , t - (handlesize / 2)  , handlewidth, handlesize, thColor);
		d->drawRect(pos- (handlewidth / 2)  , t - (handlesize / 2)  , handlewidth, handlesize, tsColor);
		d->drawRect(pos- (handlewidth / 2)+1, t - (handlesize / 2)+1, handlewidth-2, handlesize-2, tsColor);
	}
	ox = pos;
	pos = value;
	
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

bool  SliderH::slide(float ScreenX,float ScreenY){

	bool pressed = false;

	x = ScreenX;
	y = ScreenY;

	if (!enabled) {
		return pressed;
	}

	if (enabled) {
		tsColor = sColor;
		thColor = hColor;
		ssColor = sColor;
	}
	else {
		tsColor = dsColor;
		thColor = dhColor;
		ssColor = dsColor;
	}

	// Serial.print("sn "); Serial.println(sn);
	if (sn != 0.0 ) {
 		
		x = x - l;
		// Serial.print("x b "); Serial.println(x);
		x = round( x / ( w / sn));
		//Serial.print("x a "); Serial.println(x);
		x = x * ( w / sn);
		x = x + l;
	}
	else {
		x = x; 
	}

	// draw ball and scale
	if (x != ox){
		
		if ((x >= l) & (x <= (l + w))) {

			if ((abs(y - t )) <= handlesize) {

				pressed = true;
				if (handleshape == HANDLE_CIRCLE) {
					d->fillCircle(ox, t, handlesize/2, bColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					d->fillRect(ox- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox,t, bColor);
					d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox,t, bColor);
				}
				else if (handleshape == HANDLE_RECTANGLE) {
					d->fillRect(ox- (handlewidth/ 2), t - (handlesize / 2), handlewidth, handlesize, bColor);
				}

				if (colorscale){
					d->fillRect(l, t-(bt/2), x-l, bt, hColor);
					d->fillRect(x, t-(bt/2), w - x + l, bt, sColor);
				}
				else{
					d->fillRect(l, t-(bt/2), w+1, bt, ssColor);
				}
			

				if (sc != 0.0) {
					for (i = 0; i <= sc; i++){
				
						d->fillRect((i * (w / sc) ) + l, t-3, 1, 7, sColor);

						if ((i == ce) | (i == 0) | (i == sc)) {
							d->fillRect((i * (w / sc)) + l-1, t - 3, 3, 7, sColor);
						}
					}
				}

				if (handleshape == HANDLE_CIRCLE) {
					d->fillCircle(x, t, handlesize/2, hColor);
					d->drawCircle(x, t, handlesize/2,sColor);
					d->drawCircle(x, t, (handlesize/2) - 1,sColor);
				}
				else if (handleshape == HANDLE_SQUARE) {
					d->fillRect(x- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, hColor);
					d->drawRect(x- (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, sColor);
					d->drawRect(x- (handlesize / 2)+1, t - (handlesize / 2)+1, handlesize-2, handlesize-2, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_1) {
					d->fillTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, hColor);
					d->drawTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_2) {
					d->fillTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, hColor);
					d->drawTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, sColor);
				}
				else if (handleshape == HANDLE_TRIANGLE_3) {
					d->fillTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, hColor);
					d->fillTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, hColor);

					d->drawTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,t, sColor);
					d->drawTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,t, sColor);

				}
				else if (handleshape == HANDLE_RECTANGLE) {
					d->fillRect(x- (handlewidth / 2)  , t - (handlesize / 2)  , handlewidth  , handlesize  , hColor);
					d->drawRect(x- (handlewidth / 2)  , t - (handlesize / 2)  , handlewidth  , handlesize  , sColor);
					d->drawRect(x- (handlewidth / 2)+1, t - (handlesize / 2)+1, handlewidth-2, handlesize-2, sColor);
				}
				ox = x;

				// get scaled val and pass back and store in the public variable in case anyone wants it
				
				pos = MapFloat(x, l, l + w, sl, sh);
				//Serial.print("pos "); Serial.println(pos);
				value = pos;
				//Serial.print("value "); Serial.println(value);
				delay(debounce);
			}
		}

	}
	
	return pressed;
}

void SliderH::drawSliderColor(bool color){

	colorscale = color;

}



void SliderH::setHandleSize(int size){

	if (size < 4) {
		handlesize = 4;
	}
	else if (size > 40) {
		handlesize = 40;
	}
	else {
		handlesize = size;
	}

}

void SliderH::setHandleSize(int size, int width){

	if (size < 4) {
		handlesize = 4;
	}
	else if (size > 40) {
		handlesize = 40;
	}
	else {
		handlesize = size;
	}

	if (width < 4) {
		handlewidth = 4;
	}
	else if (width > 40) {
		handlewidth = 40;
	}
	else {
		handlewidth = width;
	}


}

void SliderH::setHandleShape(byte shape){

	if (shape < 0) {
		handleshape = HANDLE_CIRCLE;
	}
	else if (shape > 6) {
		handleshape = HANDLE_CIRCLE;
	}
	else {
		handleshape = shape;
	}

}

void SliderH::setPressDebounce(byte Debounce) { 

	debounce = Debounce;

}

/*

class for a simpel slider-type on off switch, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/
SliderOnOff::SliderOnOff(ILI9341_t3 *display, uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, uint16_t SliderH, uint16_t SliderColor, uint16_t BackColor, uint16_t OnColor, uint16_t OffColor)

{
	// map arguements to class variables
	_d = display;
	_l = SliderX;
	_t = SliderY;
	_w = SliderW;
	_h = SliderH;
	_sColor = SliderColor;
	_bColor = BackColor;
	_onColor = OnColor;
	_offColor = OffColor;

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

bool SliderOnOff::slide(float ScreenX, float ScreenY){

	_changed = false;

	if ((ScreenX >= _l) && (ScreenX <= (_l + _w))) {


		if (abs(ScreenY - (_t + (_h / 2))) < _h) {

			_changed = true;

			// it's in range of slider ball
			if (ScreenX < (_l + (_w / 2))){
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



void  SliderOnOff::draw(bool state) {

	_pos = state;

	_d->fillRoundRect(_l, _t, _w, _h, _h / 2, _bColor);
	_d->drawRoundRect(_l, _t, _w, _h, _h / 2, _sColor);
	_d->drawRoundRect(_l + 1, _t + 1, _w - 2, _h, _h / 2, _sColor);
	

	if (state) {
		// draw on button
		_d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);
	}

	else {
		// draw off button
		_d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);

	}

}


/*

method to see of the user just turned the switch on or off

*/
bool SliderOnOff::changed(){

	return _changed;

}

bool SliderOnOff::getValue(){

	return _pos;

}






/*

the mapF for mapping float values

*/

float BarChartV::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  if (in_min < 0) {
    in_max = in_max + abs(in_min);
    in_min = 0.0;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float BarChartH::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  if (in_min < 0) {
    in_max = in_max + abs(in_min);
    in_min = 0.0;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float CGraph::MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float SliderH::MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {

  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

float SliderV::MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

float SliderD::MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
  return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

}

/*

class for a vertical type slider, you pass in the coordinates for placement and colors, we'll pass in scale during initilization as scale may depend on some computed value

*/

SliderD::SliderD(ILI9341_t3 *Display)

{
	// map arguements to class variables
	d =Display;
	
	
}



void SliderD::init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderR, float SweepAngle, float ScaleLow, float ScaleHi, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor ) {
    
	// map arguements to class variables
	x = SliderX;
	y = SliderY;
	r = SliderR;
	sl = ScaleLow;
	sh = ScaleHi;
	sa = SweepAngle;
	as = 180.0 + 90.0 -((360.0 - sa)/2.0);
	ae = -(90.0-(360.0-sa)/2.0);
	colorscale = true;
	handlesize = SLIDER_HANDLE_SIZE;
	enabled = true;
	visible = true;
	dt = 3;
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;
	debounce = TFT_DEBOUNCE;
	dsColor = C_DISABLE_LIGHT;
	dhColor = C_DISABLE_MED;
	redraw = true;
	state = true;
}


void SliderD::setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

	// map arguements to class variables
	sColor = SliderColor;
	bColor = BackgroundColor;
	hColor = HandleColor;

}

void SliderD::DrawRing(float Start, float End, uint16_t color){

	for (i = Start; i > End; i--){
		hx = (r * (cos(i * degtorad))) + x;
		hy = -(r * (sin(i * degtorad))) + y;

		d->fillCircle(hx, hy, dt, color); 

		if (color == bColor){
			d->fillCircle(hx, hy, dt+1, color); 
		}		
	}
}

void SliderD::DrawHandle(float angle, uint16_t hColor, uint16_t sColor){
	
	hx = (r * (cos(angle * degtorad))) + x;
	hy = -(r * (sin(angle * degtorad))) + y;

	if (hColor == bColor){
		d->fillCircle(hx, hy, handlesize, bColor); // erase it
	}
	else {
		d->fillCircle(hx, hy, handlesize, hColor); 
		d->drawCircle(hx, hy, handlesize-1, sColor); 
		d->drawCircle(hx, hy, handlesize-2, sColor); 
	}
}


void  SliderD::draw(float val) {
	
	pressed = false;

	if (!redraw){
		redraw = !redraw;
		return;
	}

	if ((val < sl) || (val > sh)){
		return;
	}
	if  (!visible) {
		if (!visible) {
			DrawRing(as, ae, bColor);
			DrawHandle(angle, bColor, bColor);
		}
		return;
	}

	value = val;

	if (enabled) {
		tsColor = sColor;
		thColor = hColor;
	}
	else {
		tsColor = dsColor;
		thColor = dhColor;
	}
	// get position angle
	angle = MapFloat(value, sl, sh, (float)as, (float)ae );
	
	if (colorscale){
		DrawRing(as, angle, thColor);
	}
	else {
		DrawRing(as, angle, tsColor);
	}

	DrawRing(angle, ae, tsColor);
	
	DrawHandle(angle, thColor, tsColor);
	
	oangle = angle;

	value = val;
		   
}

bool SliderD::slide(uint16_t ScreenX, uint16_t ScreenY){
	
	pressed = false;
	
	if ((!enabled) || (!visible)) {
		//return pos;
		return pressed;
	}

	// is press in range
	
	dist = sqrt( ((ScreenX - x)*(ScreenX - x)) + ((ScreenY - y)*(ScreenY - y))   );

	angle = (atan( (abs(y - (float)ScreenY)) / ( abs(x - (float) ScreenX)) )) / degtorad;
	// cases

	// quad 1
	if ((ScreenX >= x) && (ScreenY <= y)){
		angle = angle;
	}
	//quad 4
	else if ((ScreenX > x) && (ScreenY > y)){
		angle = -angle;
	}
	// quad 2
	else if ((ScreenX <= x) && (ScreenY <= y)){
		angle = 180-angle;
	}
	// quad 3
	else if ((ScreenX <= x) && (ScreenY >= y)){
		angle = 180+angle;
	}

	if (   (abs(angle - oangle) > 0 ) && (abs(dist-r)) < (handlesize/2) && (angle <= as) && (angle >= ae)){

		pressed = true;
		redraw = true;
		DrawHandle(oangle, bColor, bColor);

		if (colorscale){
			DrawRing(as, angle, hColor);
		}
		else {
			DrawRing(as, angle, sColor);
		}
		DrawRing(angle, ae, sColor);
	
		DrawHandle(angle, hColor, sColor);
	
		oangle = angle;

		value = MapFloat(angle,as,ae,sl, sh);
			
	}
	return pressed; 
}

void  SliderD::setHandleColor(uint16_t HandleColor) {

	hColor = HandleColor;

}

void SliderD::drawSliderColor(bool state){

	colorscale = state;

}

void SliderD::setRingThickness(byte Thickness){

	dt = Thickness;
	
}

void  SliderD::setDisableColor(uint16_t HandleColor, uint16_t SliderColor) {

	dsColor = SliderColor;
	dhColor = HandleColor;

}

void SliderD::setScale(float ScaleLow, float ScaleHi) {
    
	sl = ScaleLow;
	sh = ScaleHi;

}


void  SliderD::enable() {

	enabled = true;
	redraw = false;
	if(!state) {
		state = true;
		redraw = true;
	}
}

void  SliderD::disable() {
	enabled = false;
	redraw = false;
	if(state) {
		state = false;
		redraw = true;
	}
}

void  SliderD::show() {

	visible = true;

}

void  SliderD::hide() {

	visible = false;
	
}

void SliderD::setHandleSize(int value){

	if (value < 4) {
		handlesize = 4;
	}
	else if (value > 100) {
		handlesize = 100;
	}
	else {
		handlesize = value;
	}

}

void SliderD::setPressDebounce(byte Debounce) { 
	debounce = Debounce;
}


