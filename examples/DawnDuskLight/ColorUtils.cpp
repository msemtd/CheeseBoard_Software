#include "ColorUtils.h"

// This is a super simple implementation. Probably wants to be
// configurable or something.  daySeconds is number of seconds into
// the day, in localtime with DST accounted for.
uint32_t dayColor(long daySeconds)
{
    uint8_t hours = daySeconds/3601;
    switch (hours) {
        // Evening
        case 18:
        case 19:
        case 20:
        case 21:
            return 0xff8844;
            break;
        // Night
        case 22:
        case 23:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return 0xff8800;
            break;
        // Morning
        case 6:
        case 7:
        case 8:
        case 9:
            return 0xaa88ff;
            break;
        // daytime
        default:
            return 0xffeedd;
    }
}

uint32_t fadeColor(uint32_t color, float percent)
{
    uint8_t r = color >> 16;
    uint8_t g = (color >> 8) % 256;
    uint8_t b = color % 256;

    // TODO: implement per-color non-linear models 

    // sanity check
    if (percent < 0) percent = 0;
    if (percent > 100)  percent = 100;

    r = r * (percent/100.);
    g = g * (percent/100.);
    b = b * (percent/100.);

    color  = r; color <<= 8;
    color += g; color <<= 8;
    color += b;
    return color;
}


