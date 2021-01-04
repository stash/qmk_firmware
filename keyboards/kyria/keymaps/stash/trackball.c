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
#include "quantum.h"
#include <inttypes.h>
#include "trackball.h"
#include "i2c_master.h"
#include "pointing_device.h"

extern char oled_debug_str[32];

// For reasons I don't understand, this low bit always needs to be set, otherwise even reads will fail
#define TRACKBALL_ID ((TRACKBALL_ADDRESS << 1) | I2C_WRITE)
#ifndef TRACKBALL_TIMEOUT
#    define TRACKBALL_TIMEOUT 100
#endif
#ifndef TRACKBALL_ROTATION
#    define TRACKBALL_ROTATION 0
#endif
#if TRACKBALL_ROTATION == 90
#    define TRACKBALL_L_MAP 3
#    define TRACKBALL_R_MAP 2
#    define TRACKBALL_U_MAP 0
#    define TRACKBALL_D_MAP 1
#elif TRACKBALL_ROTATION == 180
#    define TRACKBALL_L_MAP 1
#    define TRACKBALL_R_MAP 0
#    define TRACKBALL_U_MAP 3
#    define TRACKBALL_D_MAP 2
#elif TRACKBALL_ROTATION == 270
#    define TRACKBALL_L_MAP 2
#    define TRACKBALL_R_MAP 3
#    define TRACKBALL_U_MAP 1
#    define TRACKBALL_D_MAP 0
#else
#    define TRACKBALL_L_MAP 0
#    define TRACKBALL_R_MAP 1
#    define TRACKBALL_U_MAP 2
#    define TRACKBALL_D_MAP 3
#endif

const uint16_t CHIP_ID = 0xBA11;
const uint8_t VERSION = 1;

const uint8_t REG_LED_RED = 0x00;
const uint8_t REG_LED_GRN = 0x01;
const uint8_t REG_LED_BLU = 0x02;
const uint8_t REG_LED_WHT = 0x03;

const uint8_t REG_LEFT = 0x04;
const uint8_t REG_RIGHT = 0x05;
const uint8_t REG_UP = 0x06;
const uint8_t REG_DOWN = 0x07;
const uint8_t REG_SWITCH = 0x08;
const uint8_t MSK_SWITCH_STATE = 0b10000000;

const uint8_t REG_USER_FLASH = 0xD0;
const uint8_t REG_FLASH_PAGE = 0xF0;
const uint8_t REG_INT = 0xF9;
const uint8_t MSK_INT_TRIGGERED = 0b00000001;
const uint8_t MSK_INT_OUT_EN = 0b00000010;
const uint8_t REG_CHIP_ID_L = 0xFA;
const uint8_t RED_CHIP_ID_H = 0xFB;
const uint8_t REG_VERSION = 0xFC;
const uint8_t REG_I2C_ADDR = 0xFD;
const uint8_t REG_CTRL = 0xFE;
const uint8_t MSK_CTRL_SLEEP = 0b00000001;
const uint8_t MSK_CTRL_RESET = 0b00000010;
const uint8_t MSK_CTRL_FREAD = 0b00000100;
const uint8_t MSK_CTRL_FWRITE = 0b00001000;


static bool initialized = false;
static struct {
    uint8_t red, green, blue, white;
} set_colors = {0,0,0,0};
int8_t trackball_status = TRACKBALL_OFF;
static trackball_state_t trackball_state = { .x = 0, .y = 0, .button = 0 };


static i2c_status_t force_reset(void) {
    uint8_t data[] = {REG_CTRL, MSK_CTRL_RESET};
    return i2c_transmit(TRACKBALL_ID, data, sizeof(data), TRACKBALL_TIMEOUT);
}

bool trackball_init(void) {
    i2c_status_t status;
    if (initialized) return true;

    status = force_reset();
    if (status != I2C_STATUS_SUCCESS) {
        trackball_status = TRACKBALL_ERROR_RESET;
        return false;
    }

    uint8_t buffer[3] = {0,0,0};
    status = i2c_readReg(TRACKBALL_ID, REG_CHIP_ID_L, buffer, sizeof(buffer), TRACKBALL_TIMEOUT);
    if (status == I2C_STATUS_SUCCESS) {
        uint16_t id = ((uint16_t)buffer[1] << 8) + buffer[0];
        if (id != CHIP_ID || buffer[2] != VERSION) {
            trackball_status = TRACKBALL_ERROR_BAD_CHIP_ID;
            return false;
        }
        initialized = true;
    } else {
        trackball_status = TRACKBALL_ERROR_INIT;
        return false;
    }

    // TODO: support interrupt pin, no pullup/down, input mode;
    // then need to use REG_INT on trackball to enable it on the trackball side

    trackball_status = TRACKBALL_OK;
    return true;
}

bool trackball_set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    if (!trackball_init()) return false;
    if (red == set_colors.red &&
        green == set_colors.green &&
        blue == set_colors.blue &&
        white == set_colors.white
    ) {
        return false;
    }
    uint8_t data[] = {REG_LED_RED, red, green, blue, white};
    i2c_status_t status = i2c_transmit(TRACKBALL_ID, data, sizeof(data), TRACKBALL_TIMEOUT);

    if (status == I2C_STATUS_SUCCESS) {
        set_colors.red = red;
        set_colors.green = green;
        set_colors.blue = blue;
        set_colors.white = white;
        trackball_status = TRACKBALL_OK;
        return true;
    }
    trackball_status = TRACKBALL_ERROR_COLOR;
    return false;
}

bool trackball_poll() {
    if (!initialized) return false;
    uint8_t buffer[5]; // LEFT, RIGHT, UP, DOWN, BUTTON
    i2c_status_t status = i2c_readReg(TRACKBALL_ID, REG_LEFT, buffer, sizeof(buffer), TRACKBALL_TIMEOUT);
    if (status != I2C_STATUS_SUCCESS) {
        trackball_status = TRACKBALL_ERROR_POLL;
        return false;
    }
    //snprintf(oled_debug_str, sizeof(oled_debug_str), "tbp %02x%02x%02x%02x%02x", (int)buffer[0], (int)buffer[1], (int)buffer[2], (int)buffer[3], (int)buffer[4]);

    trackball_state.x += buffer[TRACKBALL_R_MAP] - buffer[TRACKBALL_L_MAP];
    trackball_state.y += buffer[TRACKBALL_D_MAP] - buffer[TRACKBALL_U_MAP];
    trackball_state.button = (buffer[4] == 0x80) ? 1 : 0;
    snprintf(oled_debug_str, sizeof(oled_debug_str), "tbs %ld:%ld:%d", (long)trackball_state.x,(long)trackball_state.y,(int)trackball_state.button);
    return true;
}

static int8_t transfer_axis(int16_t *axis) {
    // HID pointer devices can send between -127 and 127 per report,
    // see https://docs.qmk.fm/#/feature_pointing_device
    if (*axis < -127) {
        *axis += 127;
        return -127;
    } else if (*axis > 127) {
        *axis -= 127;
        return 127;
    } else {
        int8_t value = (int8_t)*axis;
        *axis = 0;
        return value;
    }
}

void trackball_get_raw(trackball_state_t *state) {
    //memcpy(state, &trackball_state, sizeof(trackball_state_t));
    *state = trackball_state;
}
void trackball_set_raw(trackball_state_t state) {
    //memcpy(&trackball_state, &state, sizeof(trackball_state_t));
    trackball_state = state;
    dprintf("tb set %d %d %d\n", (int)state.x, (int)state.y, (int)state.button);
}
void trackball_clear(void) {
    trackball_state.x = trackball_state.y = 0;
    trackball_state.button = 0;
}

void trackball_report(bool scrolling, uint8_t buttons_forced) {
    static uint8_t buttons_state = 0;

    int16_t x = trackball_state.x;
    int16_t y = trackball_state.y;
    uint8_t buttons = trackball_state.button | buttons_forced;
    if (x == 0 && y == 0 && buttons == 0) return; // no change

    //dprintf("trackball set %04" PRIx16 " %04" PRIx16 " %02" PRIx8 "\n", trackball_state.x, trackball_state.y, trackball_state.button);
    //dprintf("tb x %d\n", (int)x);
    //dprintf("tb y %d\n", (int)y);
    //dprintf("tb b %d\n", (int)buttons);

    // Give movement a quadratic curve
    //x *= (x < 0) ? -x : x;
    //y *= (y < 0) ? -y : y;

#ifdef TRACKBALL_SCROLL_FACTOR
    // optionally scale scrolling speed
    if (scrolling) {
        x *= TRACKBALL_SCROLL_FACTOR;
        y *= TRACKBALL_SCROLL_FACTOR;
    }
#endif

    while (x != 0 || y != 0 || buttons != buttons_state) {
        int8_t px = transfer_axis(&x);
        int8_t py = transfer_axis(&y);
        report_mouse_t report = pointing_device_get_report();
        report.x = scrolling ? 0 : px;
        report.y = scrolling ? 0 : py;
        report.h = scrolling ? px : 0;
        report.v = scrolling ? py : 0;
        if (x == 0 && y == 0) {
            // Only adjust button state on very last report
            buttons_state = buttons;
        }
        report.buttons = buttons_state;

        pointing_device_set_report(report);
        pointing_device_send();
    }

    trackball_clear();
}
