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
#include "trackball.h"
#include "i2c_master.h"

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

uint8_t trackball_chip_id_h = 0;
uint8_t trackball_chip_id_l = 0;
uint8_t trackball_chip_version = 0;

static bool initialized = false;
static struct {
    uint8_t red, green, blue, white;
} set_colors = {0,0,0,0};

static i2c_status_t force_reset(void) {
    uint8_t data[] = {REG_CTRL, MSK_CTRL_RESET};
    return i2c_transmit(TRACKBALL_ID, data, sizeof(data), TRACKBALL_TIMEOUT);
}

bool trackball_init(void) {
    i2c_status_t status;
    if (initialized) return true;

    status = force_reset();
    if (status != I2C_STATUS_SUCCESS) {
        trackball_chip_id_h = 0xee;
        trackball_chip_id_l = 0x05;
        trackball_chip_version = (uint8_t)status;
        return false;
    }

    uint8_t buffer[3] = {};
    status = i2c_readReg(TRACKBALL_ID, REG_CHIP_ID_L, buffer, sizeof(buffer), TRACKBALL_TIMEOUT);
    if (status == I2C_STATUS_SUCCESS) {
        trackball_chip_id_l = buffer[0];
        trackball_chip_id_h = buffer[1];
        trackball_chip_version = buffer[2];
        uint16_t id = ((uint16_t)buffer[1] << 8) + buffer[0];
        if (id != CHIP_ID || buffer[2] != VERSION) {
            trackball_chip_id_h = 0xee;
            trackball_chip_id_l = 0x01;
            trackball_chip_version = (uint8_t)status;
            return false;
        }
        initialized = true;
    } else {
        trackball_chip_id_h = 0xee;
        trackball_chip_id_l = 0x02;
        trackball_chip_version = (uint8_t)status;
        return false;
    }

    return initialized;

    // TODO: support interrupt pin, no pullup/down, input;
    // need to use REG_INT to enable it on the trackball side
}

bool trackball_present(void) {
    return initialized;
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
        return true;
    }
    return false;
}

bool trackball_reset() {
    if (!initialized) return false;
    i2c_status_t status = force_reset();
    initialized = false;
    return status == I2C_STATUS_SUCCESS;
}

bool trackball_read(int16_t *x, int16_t *y, uint8_t *buttons) {
    if (!initialized) return false;
    i2c_status_t status;
    uint8_t buffer[5] = {/* LEFT, RIGHT, UP, DOWN, BUTTON */};
    status = i2c_readReg(TRACKBALL_ID, REG_LEFT, buffer, sizeof(buffer), TRACKBALL_TIMEOUT);
    if (status != I2C_STATUS_SUCCESS) {
        trackball_chip_id_h = 0xee;
        trackball_chip_id_l = 0x11;
        trackball_chip_version = (uint8_t)status;
        return false;
    }
    *x = buffer[TRACKBALL_R_MAP] - buffer[TRACKBALL_L_MAP];
    *y = buffer[TRACKBALL_D_MAP] - buffer[TRACKBALL_U_MAP];
    *buttons = buffer[4] == (1<<7) ? 1 : 0;
    return true;
}
