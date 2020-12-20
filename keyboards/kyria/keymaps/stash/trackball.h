#pragma once

#ifndef TRACKBALL_ADDRESS
#    define TRACKBALL_ADDRESS 0x0A
#endif

bool trackball_read(uint16_t *x_axis, uint16_t *y_axis, uint8_t *button);
void trackball_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void trackball_reset();
