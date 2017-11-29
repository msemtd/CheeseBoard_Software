#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "GfxItem.h"

/*! Experimental object to tie other GfxItems together */
class GfxScene {
public:
    const static uint8_t MaxItems = 16;

public:
    GfxScene();

    //! \return true if item added to scene ok, else false
    bool add(GfxItem* item);

    //! Draws all items in a scene in order
    void draw();

protected:
    GfxItem *_items[MaxItems];

};

