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

extern uint8_t trackball_chip_id_h;
extern uint8_t trackball_chip_id_l;
extern uint8_t trackball_chip_version;
bool trackball_init(void);
bool trackball_present(void);
bool trackball_read(uint16_t *x_axis, uint16_t *y_axis, uint8_t *button);
bool trackball_set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
bool trackball_reset(void);
