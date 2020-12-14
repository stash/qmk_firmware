/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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
#define DEVICE_VER 0x0104

#ifdef OLED_DRIVER_ENABLE
  #define OLED_DISPLAY_128X64
#endif

// If you are using an Elite C rev3 on the slave side, uncomment the lines below:
// #define SPLIT_USB_DETECT
// #define NO_USB_STARTUP_CHECK

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
#define RGB_MATRIX_STARTUP_SPD 60

#define LEADER_PER_KEY_TIMING
#define LEADER_TIMEOUT 500

// Disable mechanically locking keys. Saves 174 firmware bytes!!
#undef LOCKING_SUPPORT_ENABLE
