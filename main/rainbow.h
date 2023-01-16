#ifndef RAINBOW_H
#define RAINBOW_H

#include <stdint.h>

typedef uint8_t rainbow_color_t[3];
#define RAINBOW_INIT {255,0,0}

/**
 * Generates next color in the rainbow
 *   it is changing c
 *   c must be initialized (RAINBOW_INIT)
 **/
void color_next(rainbow_color_t c, unsigned int step);

#endif /* RAINBOW_H */