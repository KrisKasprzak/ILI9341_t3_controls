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
  MyGraph.drawGraph();		// draw empty graph if you have a long delay before any plottable data
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
