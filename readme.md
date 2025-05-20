<b><h2><center>Version 8.0 updated 5/20/2025</center></h1></b>
1. added support for individual bar segments in BarGraphH and BarGraphV (kind of a UV meter look)
2. added support for an arc shaped bar chart, BarGraphA (kind of a famcy UV meter look)
<b><h2><center>Version 6.1 updated 4/4/2023</center></h1></b>
1. added support for setting fill and outline for pressed button color
2. added support for legend offset

<b><h2><center>Version 6.0 updated 2/18/2023</center></h1></b>
1. added support for icons on buttons (color545 and mono)

<b><h2><center>Version 5.5 updated 11/18/2022</center></h1></b>
1. Improved centering for buttons and graphing text (now uses measureTextWidth(), measureTextHeight() from the ILI9341_t3 library)

<b><h2><center>Version 5.4 updated 12/20/2021</center></h1></b>
1. Added a ring slider

<b><h2><center>Version 5.31	updated 4/7/2021</center></h1></b>
1. Fixed snap issue in horizontal slider and draw issue with double triangle in horizontal slider

<b><h2><center>Version 5.3 updated 4/7/2021</center></h1></b>
1. added code to redraw changed handles, where handle size with setHandleSize(val), or shape with setHandleShape(val), was changed after the init(). Classic use case is where a triangle handle is to be used for low values, a square for high values. Note if handle color is changed dynamically, just call draw(and the value).

<b><h2><center>Slider Controls for Touchscreen Displays</center></h1></b>

This library will let programmers add graphkical objects such as 1) horizontal bar chart 2) vertical bar chart 3) cartesian graph 4) dial and 5) slider controlss to their projects. This library is intended to be used with ILI9341_t3 libraries and TFT LCD's that support touch. Typical applications are graphing temperatures, voltages, etc. as wel as providing UI controls for adjusting volume, brightness and on/off capability. Multiple controls can be added allow the UI creation par charts that report pin reads and sliders to adjust output levesl. Most of these objects support multiple ceation, location control, color control as other options. 

While this library was created for the ILI9341_t3 and intended for Teensy-based MCU's, the library may be adaptable to other display drivers. Note the driver must use the Adafruit_GFX libraries. To allow other display drivers, modify the lines in the .h and .cpp files to allow a different driver to be passed in. For example change call such as SliderH(ILI9341_t3 *disp,... to SliderH(Adafruit_RA8875 *disp,...
<br>
<br>

![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/BarChartH.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/BarChartV.jpg)
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/HBar.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/VBar.jpg)
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/BarChartA.jpg)
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/CGraph.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/Dial.jpg)
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/Equalizer.jpg)
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/ColorPicker.jpg)
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/RingSliders.jpg)

<br>
<br>

This library also has controls for 1) buttons 2) check boxes and 3) option buttons

![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/Button.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/Checkbox.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/OptionButton.jpg)
<br>
<br>
I have included a bonus example that shows how to create multi-screen interface
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/MultiSceen.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/Eq.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/EQSettings.jpg)


