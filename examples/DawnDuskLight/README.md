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

* When off (StandbyMode):
   * Press the rotary input to turn the light on (enter OnMode)
   * Twist to adjust the wake time
   * Long press to enter the menu
* When on (OnMode):
   * Twist to change brightness
   * Push to turn enter GoToSleepMode
* When in GoToSleepMode:
   * Twist to change brightness
   * Push+twist to set the fade time
   * Push to turn off
* When in the config menu (twist to change item, press to select item):
   * The first item will be to turn on/off the morning light
   * The second item enables AP mode for network configuration
   
## TODO

+ Clock display off at night
  - enable for 10 seconds with any input
  - only enter this "display off" state after GoToSleep mode has activated
  - display should be on in the daytime

+ Have a SET_POST_WAKE number of minutes to keep light on after end of wake up fade in

+ When adjusting wake time in StandbyMode, only start detecting the new time when the save() is called so that 
  it's possible to scoot "over" the wake period without immediately triggering WakeUpMode


+ WakeUpMode should not re-trigger if cancelled!

+ If RealTimeClock::dnsLookup fails three times in a row, abort (repeated blocking dnsLookups cause lock up of UI)

+ SetupMode - implement menus
  + set/cancel/auto DST
  + set/cancel/auto timezone
  + set offline mode - don't try to connect
  + auto dawn time (use long/lat to lookup dawn time)

+ EspApConfigurator: have option to turn off LED heartbeat

+ in GoToSleepMode:
  + when adjusting duration, current brightness should stay the same - the new duration should be from there til black

+ Auto DST&Timezone - use online DST API based on long/lat
  - Add latitude setting

+ Auto location based on IP

