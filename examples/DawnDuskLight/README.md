# DawnDuskLight Example

Light can play an important role in how we tranistion from wakefulness to sleep and back again. This sketch turns your CheeseBosrd into a smart night light, which uses colors appropriate to the time of day to maximise restful sleep.  

The light will slowly fade off over the course of several minutes to help you get to sleep gently.  

In the morning - at a time of your choosing - it will slowly fade on to full brigtness, simulating dawn light and helping you get your day started.

## Build instructions

1. Install pre-requisites as described in the main CheeseBoard README.md
2. Open DawnDuskLight.ino in the Arduino IDE
3. Click build
4. Connect the CheeseBoard with the connector on the NodeMCU
5. Upload

## Usage

NOTE: most of this is still just the goal, and not yet implemented...

TODO: write when UI completed

   
## TODO

+ Implement option to tun on/off the alarm!

+ Have a SET_POST_WAKE number of minutes to keep light on after end of wake up fade in

+ WakeUpMode should not re-trigger if cancelled!

+ If RealTimeClock::dnsLookup fails three times in a row, abort (repeated blocking dnsLookups cause lock up of UI)

+ SetupMode - implement menus
  + set/cancel/auto DST
  + set/cancel/auto timezone
  + set offline mode - don't try to connect
  + auto dawn time (use long/lat to lookup dawn time)

+ Auto DST&Timezone - use online DST API based on long/lat
  - Add latitude setting

+ Auto location based on IP

