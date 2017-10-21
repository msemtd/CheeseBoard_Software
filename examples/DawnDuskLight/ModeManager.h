#pragma once

#include "EventMode.h"

class ModeManagerClass {
public:
    ModeManagerClass();

    //! Call during setup()
    //! \param initialMode the mode to start in
    void begin(EventMode* initialMode);

    //! Call frequently to update the current mode
    void update();

    //! Call to switch to another mode
    //! \param newMode the new mode to switch to
    //! \param force calling stop in current mode and start in new mode, even if they are the same
    void switchMode(EventMode* newMode, bool force=false);

    //! Find out if the current mode has finished what it's doing
    bool isFinished();

    //! Send a push event to the current mode
    void pushEvent(uint16_t durationMs);

    //! Send a twist event to the current mode
    void twistEvent(int8_t diff, int32_t value);

    //! Send a push-twist event to the current mode
    void pushTwistEvent(int8_t diff, int32_t value);


private:
    EventMode* _mode;

};

extern ModeManagerClass ModeManager;
