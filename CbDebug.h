#pragma once

#include <Arduino.h>

#ifndef DEBUGSTREAM
#define DEBUGSTREAM Serial
#endif

#ifndef DEBUGBAUD
#define DEBUGBAUD 115200
#endif

#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#ifdef CBDEBUG
#define CBDEBUG 1
#else
#define CBDEBUG 0
#endif

//! Call this from your setup() function to automatically init the 
//! stream.  If you do, you don't need to automatically call Serial.begin()
//! or similar
#define DBBEGIN do { DEBUGSTREAM.begin(DEBUGBAUD); } while (0)

//! Debug output (without newline)
//! \param ... the same parameters as would be passed to Serial.print(...)
#define DB(...) \
            do { if (DEBUG) DEBUGSTREAM.print(__VA_ARGS__); } while (0)

//! Debug output (with newline)
//! \param ... the same parameters as would be passed to Serial.print(...)
#define DBLN(...) \
            do { if (DEBUG) DEBUGSTREAM.println(__VA_ARGS__); } while (0)

//! Debug output (with printf like functionality) - only availabale where 
//! Serial.printf is defined, e.g. for ESP8266 and ESP32 builds
//! \param ... the same parameters as would be passed to Serial.printf(...)
#define DBF(...) \
            do { if (DEBUG) DEBUGSTREAM.printf(__VA_ARGS__); } while (0)


// These are for debugging of files provided with the Cheeseboard code
// They are considered "internal". The only difference is that they are
// turns on and off with a separate #define, specifically CBDEBUG
#define _DB(...) \
            do { if (CBDEBUG) DEBUGSTREAM.print(__VA_ARGS__); } while (0)

#define _DBLN(...) \
            do { if (CBDEBUG) DEBUGSTREAM.println(__VA_ARGS__); } while (0)

#define _DBF(...) \
            do { if (CBDEBUG) DEBUGSTREAM.printf(__VA_ARGS__); } while (0)

