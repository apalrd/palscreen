palscreen
=========

Touchscreen software for Raspberry Pi + PiTFT


lcars gui
=========

GUI is layed out like an LCARS-style menu. The layered menus are arranged in a tree view. Each menu category is assigned a color. At the top of the screen, a banner displays colored banners noting the name of each level of the higher-level tree. Within each window, there are GUI elements which can be configured. The configuration of each window is done through a JSON file for each page. The screen system is heavily tied to the automation system protocol (MQTT). The page header is automatically generated (and animations to change page are generated as well). The page body is normally laid out similar to HTML Tables.

GUI elements available:
* Table generates a cell-table made up of sub-elements. Tables are the only available method of arranging pages.
* Button-Page forces a page transition to the page named
* Button-Event raises an event when pressed (by setting the MQTT topic to a random number)
* Button-NumberUp increases a variable by a fixed number each time it is pressed/released
* TextBox-Static draws a static string of text centered at a given point
* TextBox-Dynamic draws a text box using text from a MQTT topic. Text color values can be mapped from numerical values.
* BarGraph draws a bar graph from a number in an MQTT topic. Background color can be mapped from numerical values.
* ColorCell draws a fixed color cell based on a number in an MQTT topic. Discrete values or ranges are mapped to certain colors. Cell can be a square-box, round-box, or circle. Combined with a BarGraph, can be used to make a thermometer shape. 


The GUI is optimized for 320x240 (portrait) display such as the Adafruit PiTFT panel module

The system uses SDL for GUI drawing. Normally this is drawn using the Pi's second framebuffer but it can also draw to any display that SDL supports.


