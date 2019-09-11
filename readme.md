<b><h2><center>Slider Controls for Touchscreen Displays</center></h1></b>

This library will let programmers add slider-type controls to their projects. This library is intended to be used with ILI9341_t3 libraries and TFT LCD's that support touch. Users can add vertical, horizontal, or on/off sliders. Typical applications are volume controls, brightness controls, and on/off like buttons. Multiple controls can be added allow the UI creation of a graphic equalizer. Users can control location, length of scale, colors, display of tick marks, and scale snapping. Users specify the length of slider and both low and high limits, and the slider control draws a drag handle and upon drag returns the scaled value that will lay between the limits. Scale can be floats to bytes. 

<b><h3>General code usage</b></h3> 

1. Create a slider object

2. initialize (not needed for on/off slider)

3. draw the slider object

4. process any screen touch events (see the TFT LCD for touch operations)
5. pass screen press x and y into slider objects on the current screen
6. process each call back

<b><h3>Tips on usage</b></h3> 

The objects are smart and automatically filter out press outside the controls range. So you will not need to test a press in order to make a call to each control. There is a changed() method that can be called to test if a slider is pressed but I recommend only using this in cases where a sliders result is passed to a long function call or slider handle color needs to be change for high values.

While this library was created for the ILI9341_t3 and intended for Teensy-based MCU's, the library may be adaptable to other display drivers. Note the driver must use the Adafruit_GFX libraries. To allow other display drivers, modify the lines in the .h and .cpp files to allow a different driver to be passed in. For example change call such as SliderH(ILI9341_t3 *disp,... to SliderH(Adafruit_RA8875 *disp,...
<br>
<br>
<img src="www.github.com/KrisKasprzak/ILI9341_t3_controls/master/Equalizer.bmp"> <img src="ColorPicker.bmp">
![Equalizer]https://github.com/KrisKasprzak/ILI9341_t3_controls/Equalizer.bmp)
