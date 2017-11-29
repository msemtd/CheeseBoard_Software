#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

/*! \brief HC12 Wireless Serial class
 *
 *  This class allows use of the HC12 serial module. Inheriting from SoftwareSerial, it 
 *  can be read from and written to using the usual members (write, println, read etc.).
 *  It also provides a mechanism for switching into and out of AT command mode.
 *
 *  In AT command mode, special command strings may be sent o the HC12 module to configure
 *  things like baud rate, transmit power and so on. See this 
 *  <a href="https://www.elecrow.com/download/HC-12.pdf">HC12 reference</a> for details.
 */
class CbHC12Class : public SoftwareSerial {
public:
    const static uint16_t CheckTimeoutMs = 250;

public:
    /*! Constructor.
     *
     *  \param rxPin The Pin connected to the RX pin of the HC12 module.
     *  \param txPin The Pin connected to the TX pin of the HC12 module.
     *  \param setPin The Pin connected to the SET pin of the HC12 module.
     */
    CbHC12Class(uint8_t rxPin, uint8_t txPin, uint8_t setPin);

    /*! Initialize.
     * 
     *  \param speed the baud rate of the HC12 module communications, typically 9600.
     */
    void begin(long speed);

    /*! Set AT command mode.
     *
     * Turn on or off command mode. When enabled, the HC12 accepts AT commands to control the device.
     *
     * \param switchOn when true, enter AT command mode, when false leave command mode.
     * \param force when true explictly set the SET pin even if its already set, or has unknown state.
     */
    void setCommandMode(bool switchOn, bool force=false);

    /*! Test if HC12 module comms are working.
     *
     *  Send "AT" in AT command mode, and verify we get "OK" back, then switch back to non-AT mode
     *  if that was the state before the call to check().
     *
     *  \return true if OK, else false.
     */
    bool check();

private:
    const uint8_t _setPin;
    bool _cmdMode;

};

extern CbHC12Class CbHC12;

