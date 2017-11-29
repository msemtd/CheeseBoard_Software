# NetworkScanner Example

This sketch provides a little WiFi scanning tool.

## Implemented

+ List box showing what networks are availabale
+ Signal strength wigdet

## TODO

+ Secure connection indicator
+ Add ZoomDraw to NetInfo - show all available
+ Removing old items from the list
+ Sorting the list by: ssid or signal
+ Use EspApConfigurator to allow entry to passphrases
+ Use RGB LEDs to display signal strength of selected network
+ Signal chase mode (green = warmer, red = colder)

## Build instructions

1. Install pre-requisites as described in the main README.md file in the root of this project
2. Open NetworkScanner.ino in the Arduino IDE
3. Click build
4. Connect the CheeseBoard with the connector on the NodeMCU
5. Upload

## Usage

* Twist the Rotary Input to choose a network
* Press the Rotary Input to select network and see details [planned]
* Long press of the Rotary Input to enable function menu [planned]
   * Enable/Disable AP mode [planned]
   * Connect to [known or open] networks to see traffic flows [planned]

