# Thingspeak Gateway Example

This example uses the HC12 wireless serial module option to upload data to Thingspeak.com.  Incoming data is read from the HC12 serial module in CSV format. The fields in the CSV file correspond to field 1, field 2 ... field 8 of your Thingspeak channel. Thingspeak actually saves all these values as text. There is currently no support for values with commas in the value.

## Build instructions

You'll need to have an HC-12 serial WiFi module connected to your CheeseBoard.

1. Install pre-requisites as described in the main CheeseBoard README.md
2. Open ThingspeakGateway.ino in the Arduino IDE
3. Click build
4. Connect the CheeseBoard with the connector on the NodeMCU
5. Upload

## Usage

To use upload to Thingspeak you will need an write API key. To get one, sign up for a [free] Thingspeak account, create a channel and go to the channel's API KEYS page. Then put the CheeseBoard in AP mode (press the button), connect to it and set up the WiFi and other settings (including API key).

The sketch uses TLS (https) to connect to the thingspeak servers, but by default does not validate the certificate fingerprint. To enable fingerprint verification, check the "Verify Certificate" setting, and provide the Certificate SHA hash.


