#include <ILI9341_t3.h>           // fast display driver lib
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>           // custom fonts that ships with ILI9341_t3.h

// you must create and pass fonts to the function
#define FONT_TITLE Arial_16
#define FONT_DATA Arial_10

// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

// defines for graph location and scales
#define X_ORIGIN    50
#define Y_ORIGIN    200
#define X_WIDE 		  250
#define Y_HIGH 		  150
#define X_LOSCALE 	0
#define X_HISCALE 	10
#define X_INC 	    2
#define Y_LOSCALE 	-1
#define Y_HISCALE 	1
#define Y_INC 	    0.2

#define TEXTCOLOR C_WHITE
#define GRIDCOLOR C_GREY
#define AXISCOLOR C_YELLOW
#define BACKCOLOR C_BLACK
#define PLOTCOLOR C_DKGREY
#define VOLTSCOLOR C_RED
#define SINCOLOR C_GREEN
#define COSCOLOR C_BLUE

// used to monitor elaspsed time
unsigned long oldTime;

// create a variable for each data data point
float x, volts;

// create an ID for each data to be plotted
int VoltID, SinID, CosID;

// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);


// create the cartesian coordinate graph object
CGraph MyGraph(&Display, X_ORIGIN, Y_ORIGIN, X_WIDE, Y_HIGH, X_LOSCALE, X_HISCALE, X_INC, Y_LOSCALE, Y_HISCALE, Y_INC);


void setup() {

  Serial.begin(57600);

  // fire up the display
  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(C_BLACK);


  // initialize the graph object
  MyGraph.init("Teensy Graphing", "Time [sec]", "Volts", TEXTCOLOR, GRIDCOLOR, AXISCOLOR, BACKCOLOR, PLOTCOLOR, FONT_TITLE, FONT_DATA);

  // use the add method to create a plot for each data
  // PlotID = MyGraph.Add(data title, data color);
  // 
  VoltID = MyGraph.add("Volts", VOLTSCOLOR);
  SinID = MyGraph.add("sin(x)", SINCOLOR);

  // these call are all optional
  MyGraph.drawGraph();		// draw empty graph
  MyGraph.setMarkerSize(VoltID, 0); 
  MyGraph.setMarkerSize(SinID, 1);
  // MyGraph.setYAxis(-1.5, 2.5, 0.5);  // reset the y axis at any time in your program
  // MyGraph.showTitle(false);          //  hide the title--good for big graph in tight space
  // MyGraph.showLegend(false);         //  hide the legend--good for big graph in tight space
  // MyGraph.showXScale(false);         //  hide the x scale--good for big graph in tight space
  // MyGraph.showYScale(false);         //  hide the y scale--good for big graph in tight space

}

void loop() {

  // set the x value, should be in order and match the x scale (again you will
  // need to do your own math to get the plot to match the updte time in this loop

  volts = analogRead(A0) / 1024.0;
  MyGraph.setX(x);

  MyGraph.plot(VoltID, volts);
  MyGraph.plot(SinID, sin(x));


  // bump the x value
  x += .01;
  delay(10);

}
