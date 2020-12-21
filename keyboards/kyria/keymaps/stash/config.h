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

#undef PRODUCT_ID
#define PRODUCT_ID 0x4B79 /* "Ky" for Kyria */
#undef DEVICE_VER
#define DEVICE_VER 0x0103

#ifdef OLED_DRIVER_ENABLE
  #define OLED_DISPLAY_128X64
  #define OLED_UPDATE_INTERVAL 250
#endif

// EC11K encoders:
#define ENCODER_RESOLUTION 2

// Make TT work like ergodox
#undef TAPPING_TOGGLE
#define TAPPING_TOGGLE 1
#define TAPPING_TERM 200

#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 5

#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY 7

#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 2

#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 90

#undef MOUSEKEY_WHEEL_INTERVAL
#define MOUSEKEY_WHEEL_INTERVAL 200

#undef MOUSEKEY_WHEEL_MAX_SPEED
#define MOUSEKEY_WHEEL_MAX_SPEED 2

#define AUTO_SHIFT_TIMEOUT 135
#define NO_AUTO_SHIFT_ALPHA

#define LEADER_PER_KEY_TIMING
#define LEADER_TIMEOUT 500

// Disable mechanically locking keys. Saves 174 firmware bytes!!
#undef LOCKING_SUPPORT_ENABLE

#define SUPER_ALT_TAB_TIMEOUT 1000

#define EXTRA_SPLIT_DATA_ENABLE
#define EXTRA_SPLIT_DATA_M2S extra_split_data_m2s
struct extra_split_data_m2s {
    uint8_t oled_brightness;
    uint8_t highest_layer;
    struct {
        uint8_t red, green, blue, white;
    } trackball;
};
#define EXTRA_SPLIT_DATA_S2M extra_split_data_s2m
struct extra_split_data_s2m {
    struct {
        int16_t x_offset, y_offset;
        uint8_t button;
    } trackball;
};
