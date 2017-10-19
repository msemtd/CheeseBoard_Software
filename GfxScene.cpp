#include "GfxScene.h"

GfxScene::GfxScene() 
{
    for(uint8_t i=0; i<MaxItems; i++) {
        _items[i] = NULL;
    }
}

bool GfxScene::add(GfxItem* item) {
    uint8_t i;
    for(i=0; i<MaxItems && _items[i] != NULL; i++) {;}
    if (i<MaxItems) {
        _items[i] = item;
        return true;
    } else {
        return false;
    }
}

void GfxScene::draw() {
    for(uint8_t i=0; i<MaxItems; i++) {
        if (_items[i] != NULL) {
            _items[i]->draw();
        }
    }
}

