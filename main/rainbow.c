#include "rainbow.h"

static int _add_in_range(uint8_t* value, int step) {

    int tmp = *value;
    tmp += step;

    int ret = 0;

    if (tmp > 255) {
        ret = 255 - tmp;
        tmp = 255;
    }
    else if (tmp < 0) {
        ret = -tmp;
        tmp = 0;
    }

    *value = tmp;
    return ret;
}

static unsigned int _color_next(rainbow_color_t c, unsigned int step) {

    for (int i = 0; i <3; ++i) {

        uint8_t* curr = c + (i%3);

        if (*curr == 255) {

            uint8_t* next = c + ((i+1)%3);
            if (*next == 255)
                return _add_in_range(curr,-step);

            uint8_t* prev = c + ((i+2)%3);
            if (*prev > 0)
                return _add_in_range(prev,-step);

            return _add_in_range(next,step);

        }
    }

    //this should never happened
    rainbow_color_t i = RAINBOW_INIT;
    c = i;

    return 0;
}

void color_next(rainbow_color_t c, unsigned int step)
{
    while (step > 0) {
        step = _color_next(c,step);
    }
}