palscreen
=========

Touchscreen software for Raspberry Pi + PiTFT


squares gui
===========

GUI design has not yet been implemented. Currently that is left up to the programmer.


GFX module
==========

The GFX module is a heavily modified version of the Adafruit-GFX library. It has been modified for C operation (all of the class members have been prefixed with GFX), and implements the library fully. The library opens a Linux framebuffer interface for a specified resolution (currently hardcoded), and the pixel write function writes directly to the framebuffer RAM. A function is provided to convert 3x bytes into a u16 color. The fast vline and hline functions have been implemented more efficiently. This pair of files (gfx.h and gfx.c) can be used in other projects easily.


Backlight module
================

The Backlight module is designed to control the backlight of the PiTFT (Resistive version). It currently uses the system calls method, which requires sudo permissions, and is very slow. A better interface would be good. It also provides a timer to automatically turn off the backlight, and a poke function to wake it up/keep it awake.

Touch module
============

The Touch module is designed to interface with the /dev/input/touchscreen interface directly. It provides two initialization functions (touch_init and touch_read_kal) which initialize the linux evdev driver and then read the calibration file respectively. A program called kalibrate is provided which uses the touch, gfx, and backlight modules to draw 4 crosshairs on the screen and calibrate from them, and the touch module can read in the calibration. For most applications, both functions should be called to provide display-adjusted data, otherwise the data returned is raw. Note that the Adafruit screen x/y are swapped (or appear to be), so the data returned from the x channel actually came from the y event message, and vice versa, and they are stored in the linux-native name for the cur_x/cur_y variables. 

Pressure data is also provided, and is always raw. 

The user must call touch_get_events() regularly to read the touch events from the linux buffers. The user can then call touch_get_x, touch_get_y, and touch_get_prs to read the latest x, y, pressure data from the touch screen. The user should read touch_down() before, as an indication of touch data present. Error checking on the x/y/prs outputs when touch_down() is false is not provided.
