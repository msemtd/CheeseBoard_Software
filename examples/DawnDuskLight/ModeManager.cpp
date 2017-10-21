#include <MutilaDebug.h>
#include "ModeManager.h"

ModeManagerClass ModeManager;

ModeManagerClass::ModeManagerClass() :
    _mode(NULL)
{
}

void ModeManagerClass::begin(EventMode* initialMode)
{
    DBLN(F("ModeManager::begin"));
    switchMode(initialMode);
}

void ModeManagerClass::update()
{
    if (_mode != NULL) {
        _mode->update();
    }
}

void ModeManagerClass::switchMode(EventMode* newMode, bool force)
{
    if ((newMode != _mode) || force) {
        if (_mode != NULL) {
            _mode->stop();
        }   
        _mode = newMode;
        _mode->start();
    }
}

bool ModeManagerClass::isFinished()
{
    if (_mode != NULL) {
        return _mode->isFinished();
    } else {
        return false;
    }
}

void ModeManagerClass::pushEvent(uint16_t durationMs)
{
    if (_mode != NULL) {
        _mode->pushEvent(durationMs);
    }
}

void ModeManagerClass::twistEvent(int8_t diff, int32_t value)
{
    if (_mode != NULL) {
        _mode->twistEvent(diff, value);
    }
}

void ModeManagerClass::pushTwistEvent(int8_t diff, int32_t value)
{
    if (_mode != NULL) {
        _mode->pushTwistEvent(diff, value);
    }
}


