#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "GfxItem.h"
#include "GfxNetInfo.h"

class GfxSSIDListBox : public GfxItem {
public:
    const static uint8_t MaxItems = 32;

public:
    GfxSSIDListBox(uint16_t x=0, uint16_t y=0);
    ~GfxSSIDListBox();

    //! Adds a string item to the list box at the first free entry.
    //! \param s the string to add to the list box
    //! \return index the item was added at if successful, else -1
    int16_t add(const GfxNetInfo s);

    //! Remove item at index idx
    //! \param idx the index of the item to remove
    //! \param compactAfter if true, compact() will be called after the item is removed
    //! \return true if successful, else false
    bool remove(const uint8_t idx, bool compactAfter=true);

    //! Draw the GfxSSIDListBox (do not send buffer to CbOled)
    //! Items contains NULL strings will not be displayed
    void draw(uint16_t xOffset=0, uint16_t yOffset=0);
    uint16_t width();
    uint16_t height();
    uint16_t lineHeight();

    //! Get an GfxNetInfo pointer by index
    //! \param idx the index of the item to fetch
    //! \return NULL if there is not object at the specified index, or the index is out of range,
    //!         else returns a point to the GfxNetInfo object at the specified index
    GfxNetInfo* operator[](int16_t idx);

    //! Get in GfxNetInfo pointer by SSID
    //! \param siid the SSID of the item to fetch
    //! \return NULL if there is not object at the specified index, or the index is out of range,
    //!         else returns a point to the GfxNetInfo object at the specified index
    GfxNetInfo* operator[](const String& ssid);

    //! \return index of nth item which has the SSID ssid, or -1 if not found
    //! \param ssid the SSID of the object to find
    //! \param n the nth instance to search for
    int16_t find(const String& ssid, uint8_t n=1);

    //! Select item at index idx
    //! \return true if successful (item exists or idx was -1)
    //! \param idx the index of the item to select, or -1 to de-select
    bool select(int16_t idx);

    //! \return the index of the currently selected item (or -1 if none are selected)
    int16_t selected();

    //! Scroll down the display a number of rows
    //! \return the number of rows scrolled (e.g. 0 if already as far as can do)
    uint8_t scrollDown(uint8_t rows);

    //! Scroll up the display a number of rows
    //! \return the number of rows scrolled (e.g. 0 if already as far as can do)
    uint8_t scrollUp(uint8_t rows);

    //! scrolls the displayed items until idx is displayed
    //! Note: if idx is currently displayed, no scrolling is performed
    //! \return true if scrolled OK, else false
    bool scrollTo(uint8_t idx);

    //! Update the signal strength and channel for a specified ssid
    //! If the SSID doesn't exist, it will be added to the first 
    //! available slot in the list.
    //! \param ssid The SSID of the network to update/add
    //! \param channel the channel of the network
    //! \param signal the signal strength expressed as a percentage
    //! \return the index of the updated / added item, or -1 if the operation failed (e.g. no space in list to add a new item)
    int16_t update(String ssid, int8_t channel, uint8_t signal);

    //! \return the first index which is not NULL after "from", or -1 if not found
    //! \param from the index to search from
    int16_t findNext(uint8_t from=0);

    //! \return the first index which is not NULL before "from", or -1 if not found
    //! \param from the index to search from
    int16_t findPreceding(uint8_t from=MaxItems);

    //! \return how many non-NULL items are in the list
    uint8_t count();

protected:
    //! \return index of first "gap" (i.e. where item is NULL), or -1 if there are no gaps
    //! \param startAt index to start looking for gaps at
    int16_t findGap(uint8_t startAt=0);

    //! Sort the items contained in the list box using quicksort
    // TODO: void sort();

    //! \return true if there are no gaps between string items, else false
    bool isCompacted();

    //! Make used items contiguous (remove gaps)
    void compact();

    //! Find out if idx is currently visible on screen
    bool isOnScreen(uint8_t idx);

    //! Find out how many rows after there from idx on (which are non-NULL)
    uint8_t rowsFrom(uint8_t idx);


protected:
    GfxNetInfo *_items[MaxItems];
    int16_t _selected;
    uint8_t _screenLines;
    uint8_t _screenStart;
    uint8_t _lineHeight;
    // TODO: uint8_t _frameSize; uint8_t _frameStart;

};

