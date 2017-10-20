#include "CbRotaryInput.h"
#include "MutilaDebug.h"
#include "Config.h"

CbRotaryInputClass CbRotaryInput(ROT_PUSH_PIN, ROT_A_PIN, ROT_B_PIN);

CbRotaryInputClass::CbRotaryInputClass(uint8_t buttonPin, uint8_t aPin, uint8_t bPin) :
#ifdef BOARD_V0
    _button(buttonPin, false),
#else
    _button(buttonPin),
#endif
    _encoder(aPin, bPin),
    _buttonCb(NULL),
    _rotatyCb(NULL)
{
}

void CbRotaryInputClass::begin(t_buttonCb buttonCb, t_rotaryCb rotaryCb)
{
    _button.begin();
    _position = _encoder.read()/4;
    _buttonCb = buttonCb;
    _rotatyCb = rotaryCb;
}

void CbRotaryInputClass::update()
{
    // Update the button and handle presses
    _button.update();
    uint16_t tapDuration = _button.tapped();
    if (tapDuration > 0) {
        DBLN(F("CbRotaryInputClass::update button press"));
        if (_buttonCb != NULL) {
            _buttonCb(tapDuration);
        }
    }

    int32_t newPosition = _encoder.read()/4;
    if (newPosition != _position) {
        int8_t diff = newPosition - _position;
        if (_rotatyCb != NULL) {
            _rotatyCb(diff, newPosition);
        }
        _position = newPosition;
    }
}

int32_t CbRotaryInputClass::getEncoderPosition()
{
    return _position;
}

bool CbRotaryInputClass::buttonPushed()
{
    return _button.on();
}

