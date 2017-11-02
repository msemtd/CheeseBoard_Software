CheeseBoard Library
===================

This library contains components for use with the CheeseBoard Cheddar and variants.  The examples show off some of the features of the boards - using WiFi, displaying messages and graphics on the OLED screen, using the RGB LEDs and getting input from the rotary encoder and it's push-button feature.

The library defines the following convenience classes:

* CbLeds - Global instance of CbNeoPixel - for controlling the RGB LEDs (inherits from Adafruit_NeoPixel)
* CbOledDisplay - Global instance of CbOledDisplayClass - U2g2 object which adds justified wrapped text 
* CbRotaryInput - Global instance of CbRotaryInputClass for interactive with the rotary encoder + button inputs
* GfxItem - base class for high-level objects which can be drawn on CbOledDisplay
* GfxSSIDListBox - a list box for displaying WiFi network information
* GfxNetInfo - an item for the GfxSSIDListBox
* GfxScene - experimental collection of GfxItem objects
* GfxSignalStrength - little signal strength meter for use in GfxNetInfo
* GfxStringListBox - generic listbox for strings (needs completion)
* GfxTextBox - show text in a box with optional border

Select your hardware by copying or linking the appropriate XXXCheeseboardConfig.h file to CheeseboardConfig.h in this directory.

Status
======

2017-10: Still under heavy development.

Documentation
-------------

Doxygen-generated documentation can be found here: https://matthewg42.github.io/CheeseBoard/

Dependencies
------------

* ESP8266 board (install using IDE boards manager, or manually from here: https://github.com/esp8266/Arduino)
* Adafruit_NeoPixel library (install using IDE library manager)
* Encoder library (install using IDE library manager)
* TimeLib (install using IDE library manager)
* U8g2 (install using IDE library manager)
* Mutila (install manually from https://github.com/matthewg42/Mutila)
* EspApConfigurator (install manually from https://github.com/matthewg42/EspApConfigurator)

Future
------

* Better method of selecting hardware
* Make sure all dependent libs are available in IDE library manager - including CheeseBoard

