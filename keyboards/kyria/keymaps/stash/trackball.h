/* Copyright 2020 Jeremy Stashewsky <jstash@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#ifndef TRACKBALL_ADDRESS
#    define TRACKBALL_ADDRESS 0x0A
#endif

typedef struct _trackball_state_t {
    int16_t x, y;
    uint8_t button;
    // COULDDO: this could maybe be packed down to 21 bits (three bytes):
    //   x and y: 9 bits plus sign (so, 10 each) since they're the sum of two int8_t's
    //   button: one bit
} __attribute__((packed)) trackball_state_t;

typedef enum _trackball_status {
    TRACKBALL_OK,
    TRACKBALL_OFF,

    TRACKBALL_ERROR = -128,
    TRACKBALL_ERROR_BAD_CHIP_ID,
    TRACKBALL_ERROR_RESET,
    TRACKBALL_ERROR_INIT,
    TRACKBALL_ERROR_COLOR,
    TRACKBALL_ERROR_POLL,
} trackball_status_t;

#ifdef TRACKBALL_DEBUG
extern int8_t trackball_status;
#endif
bool trackball_init(void);
bool trackball_set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
// Polls trackball data over local I2C
bool trackball_poll(void);
// Use these to transfer data between split sides:
void trackball_get_raw(trackball_state_t *state);
void trackball_set_raw(trackball_state_t state);
// Clears the local trackball state:
void trackball_clear(void);
// Reports movement and buttons to host. Calls trackball_clear at its end.
void trackball_report(bool scrolling, uint8_t buttons_forced);
