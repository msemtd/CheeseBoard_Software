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

* Arduino IDE. I've only tested builds with version 1.8.5, but earlier versions may also work
* ESP8266 board (install using IDE boards manager, or manually from here: https://github.com/esp8266/Arduino)
* Adafruit NeoPixel library by Adafruit (install using IDE library manager)
* Encoder library by Paul Stoffregen (install using IDE library manager)
* Time library by Michael Margolis (install using IDE library manager, search for "Timekeeping")
* U8g2 library by Oliver Kraus (install using IDE library manager)
* Mutila library by Mouse (download ZIP from https://github.com/matthewg42/Mutila, use "Add .ZIP library")
* EspApConfigurator by Mouse (download ZIP from https://github.com/matthewg42/EspApConfigurator, use "Add .ZIP library")

Building
--------

Building your own sketeches or the examples provided may be accomplished using the Arduino IDE as follows:

* Install dependencies as described above
* Open the desired sketch in the IDE
* Select Tools [menu] -> Board -> "NodeMCU 1.0 (ESP-12E Module)"
* Edit the file src/CheeseboardConfig.h and make sure the correct CheeseBoard header for your hardware is used
* Click build/upload button in toolbar

Future
------

* Better method of selecting hardware
* Make sure all dependent libs are available in IDE library manager - including CheeseBoard
* Light metronome example

