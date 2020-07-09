<b><h2><center>Slider Controls for Touchscreen Displays</center></h1></b>

This library will let programmers add graphkical objects such as 1) horizontal bar chart 2) vertical bar chart 3) cartesian graph 4) dial and 5) slider controlss to their projects. This library is intended to be used with ILI9341_t3 libraries and TFT LCD's that support touch. Typical applications are graphing temperatures, voltages, etc. as wel as providing UI controls for adjusting volume, brightness and on/off capability. Multiple controls can be added allow the UI creation par charts that report pin reads and sliders to adjust output levesl. Most of these objects support multiple ceation, location control, color control as other options. 

While this library was created for the ILI9341_t3 and intended for Teensy-based MCU's, the library may be adaptable to other display drivers. Note the driver must use the Adafruit_GFX libraries. To allow other display drivers, modify the lines in the .h and .cpp files to allow a different driver to be passed in. For example change call such as SliderH(ILI9341_t3 *disp,... to SliderH(Adafruit_RA8875 *disp,...
<br>
<br>

![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/BarChartH.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/BarChartV.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/CGraph.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/Dial.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/Equalizer.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_controls/master/ColorPicker.jpg)
