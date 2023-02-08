/*
 * Copyright 2021 Jeremy Stashewsky <jstash@gmail.com>
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

#include QMK_KEYBOARD_H
enum layer_names { _WINDOWS, _MAC, _LOWER, _RAISE, _ADJUST };

enum custom_keycodes { SET_WIN = SAFE_RANGE, SET_MAC, SUPALTT };

#define NUMBER_ROW               KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0
#define COLEMAK_D_ROW1 KC_TAB,   KC_Q, KC_W, KC_F, KC_P, KC_B, KC_J, KC_L, KC_U, KC_Y, KS_SCLN, KS_BSLS
#define COLEMAK_D_ROW2 KC_EQUAL, KC_A, KC_R, KC_S, KC_T, KC_G, KC_H, KC_N, KC_E, KC_I, KC_O, KS_QUOT
#define COLEMAK_D_ROW3 KC_LSFT,  KC_Z, KC_X, KC_C, KC_D, KC_V, KC_K, KC_M, KC_COMM, KC_DOT, KC_SLSH, KS_MINS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_WINDOWS] = LAYOUT_split_4x6_5(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,        KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    SUPALTT,
  KC_TAB,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,        KC_J,   KC_L,   KC_U,    KC_Y,   KC_SCLN, KC_BSLS,
  KC_EQUAL, KC_A,   KC_R,    KC_S,    KC_T,    KC_G,        KC_H,   KC_N,   KC_E,    KC_I,   KC_O,    KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_D,    KC_V,        KC_K,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_MINS,
                      LALT_T(KC_ESC), KC_SPC,  KC_BSPC,     RCTL_T(KC_ENT), OSM(MOD_RSFT), OSM(MOD_MEH),
                                   KC_LCTL, TT(_RAISE),     TT(_LOWER), KC_RGUI
  ),

[_MAC] = LAYOUT_split_4x6_5(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,        KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    SUPALTT,
  KC_TAB,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,        KC_J,   KC_L,   KC_U,    KC_Y,   KC_SCLN, KC_BSLS,
  KC_EQUAL, KC_A,   KC_R,    KC_S,    KC_T,    KC_G,        KC_H,   KC_N,   KC_E,    KC_I,   KC_O,    KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_D,    KC_V,        KC_K,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_MINS,
                      LALT_T(KC_ESC), KC_SPC,  KC_BSPC,     RALT_T(KC_ENT), OSM(MOD_RSFT), OSM(MOD_MEH),
                                   KC_LGUI, TT(_RAISE),     TT(_LOWER), KC_RCTL
  ),

[_LOWER] = LAYOUT_split_4x6_5(
  _______, _______, _______, _______, _______, _______,     _______, _______, KC_NUM,  KC_PSLS, KC_PAST, _______,
  _______, XXXXXXX, XXXXXXX, KC_UP,   KC_ENT,  XXXXXXX,     XXXXXXX, KC_P7,   KC_P8,   KC_P9,   KC_PMNS, _______,
  _______, KC_GRV,  KC_LEFT, KC_DOWN, KC_RGHT, KC_ENT,      XXXXXXX, KC_P6,   KC_P5,   KC_P4,   KC_PPLS, _______,
  _______, KC_GRV,  KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN,     XXXXXXX, KC_P3,   KC_P2,   KC_P1,   KC_PENT, _______,
                             _______, _______, KC_DEL,      KC_PENT, KC_P0,   KC_PDOT,
                                      _______, _______,     _______, _______
  ),

[_RAISE] = LAYOUT_split_4x6_5(
  KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  _______, XXXXXXX, XXXXXXX, KC_UP,   KC_ENT,  XXXXXXX,     KC_NUM,  KC_PSCR, KC_SCRL, KC_PAUS, XXXXXXX, _______,
  _______, KC_GRV,  KC_LEFT, KC_DOWN, KC_RGHT, KC_ENT,      KC_PGUP, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
  _______, KC_GRV,  KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN,     KC_PGDN, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,
                             KC_LALT, KC_LCTL, KC_DEL,      KC_INS,  KC_RALT, KC_MENU,
                                      _______, _______,     _______, _______
  ),

[_ADJUST] = LAYOUT_split_4x6_5(
  QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SET_WIN,     SET_MAC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG,
  XXXXXXX, TO(0),   TO(1),   XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, RGB_SPI, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,
  XXXXXXX, XXXXXXX, QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, RGB_SPD, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,
  KC_LSFT, KC_NO, QK_REBOOT, EE_CLR,  XXXXXXX, XXXXXXX,     XXXXXXX, RGB_M_B, RGB_M_R, RGB_M_G, RGB_M_P, KC_RSFT,
                             _______, _______, _______,     _______, _______, _______,
                                      _______, _______,     _______, _______
  ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
    switch(get_highest_layer(state)) {
        case _LOWER:
            rgb_matrix_sethsv_noeeprom(HSV_GOLD);
            break;
        case _RAISE:
            rgb_matrix_sethsv_noeeprom(HSV_BLUE);
            break;
        case _ADJUST:
            rgb_matrix_sethsv_noeeprom(HSV_RED);
            break;
        default:
            rgb_matrix_sethsv_noeeprom(HSV_CYAN);
            break;
    }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case SET_WIN:
                set_single_persistent_default_layer(_WINDOWS);
                return false; // stop processing
            case SET_MAC:
                set_single_persistent_default_layer(_MAC);
                return false; // stop processing
        }
    }
    return true; // continue processing
}
