/*
 * Copyright 2023 Jeremy Stashewsky <jstash@gmail.com>
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
#include "action_tapping.h"

extern keymap_config_t keymap_config;

enum layer_names { _WINDOWS, _MAC, _LOWER, _RAISE, _ADJUST };

enum custom_keycodes { SET_WIN = SAFE_RANGE, SET_MAC, SUPALTT, CVVV };

enum dance_codes { DANCE_TSHIFT = 0, DANCE_CVVV, DANCE_TEST, DANCE_MAX };
#define TD_TEST TD(DANCE_TEST)
#define TD_CVVV TD(DANCE_TEST)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_WINDOWS] = LAYOUT_split_4x6_5(
  TD_CVVV,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,        KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    MEH(KC_M),
  KC_TAB,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,        KC_J,   KC_L,   KC_U,    KC_Y,   KC_SCLN, KC_BSLS,
  KC_EQUAL, KC_A,   KC_R,    KC_S,    KC_T,    KC_G,        KC_H,   KC_N,   KC_E,    KC_I,   KC_O,    KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_D,    KC_V,        KC_K,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_MINS,
                      LALT_T(KC_ESC), KC_SPC,  KC_BSPC,     RCTL_T(KC_ENT), OSM(MOD_RSFT), OSM(MOD_MEH),
                                   KC_LCTL, TT(_RAISE),     TT(_LOWER), KC_RGUI
  ),

[_MAC] = LAYOUT_split_4x6_5(
  // NOTE: SET_MAC will exchange CTL and GUI so that tap dance works, so actually GUI & CTL below are swapped
  TD_CVVV,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,        KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    MEH(KC_M),
  KC_TAB,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,        KC_J,   KC_L,   KC_U,    KC_Y,   KC_SCLN, KC_BSLS,
  KC_EQUAL, KC_A,   KC_R,    KC_S,    KC_T,    KC_G,        KC_H,   KC_N,   KC_E,    KC_I,   KC_O,    KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_D,    KC_V,        KC_K,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_MINS,
                      LALT_T(KC_ESC), KC_SPC,  KC_BSPC,     RALT_T(KC_ENT), OSM(MOD_RSFT), OSM(MOD_MEH),
                                   KC_LCTL, TT(_RAISE),     TT(_LOWER), KC_RGUI
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
  _______, KC_GRV,  KC_LEFT, KC_DOWN, KC_RGHT, KC_ENT,      XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
  _______, KC_GRV,  KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN,     XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,
                             KC_LALT, KC_LCTL, KC_DEL,      KC_INS,  CW_TOGG, KC_MENU,
                                      KC_LGUI, _______,     _______, _______
  ),

[_ADJUST] = LAYOUT_split_4x6_5(
  QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SET_WIN,     SET_MAC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG,
  XXXXXXX, TO(0),   TO(1),   XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, RGB_SPI, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TD_TEST, XXXXXXX,     XXXXXXX, RGB_SPD, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,
  KC_LSFT, KC_NO, QK_REBOOT, EE_CLR,  XXXXXXX, XXXXXXX,     XXXXXXX, RGB_M_B, RGB_M_R, RGB_M_G, RGB_M_P, KC_RSFT,
                             _______, _______, _______,     _______, _______, _______,
                                      _______, _______,     _______, _______
  ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
    switch(get_highest_layer(state)) {
        // TODO: make these other layers respect the brightness setting (or use a suitably low one if brightness is all the way off)
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
            // TODO: make this do whatever is in eeprom
            rgb_matrix_sethsv_noeeprom(HSV_CYAN);
            break;
    }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SET_WIN:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_WINDOWS);
                keymap_config.raw = eeconfig_read_keymap();
                keymap_config.swap_lctl_lgui = keymap_config.swap_rctl_rgui = 0;
                eeconfig_update_keymap(keymap_config.raw);
                // TODO: set windows unicode mode, UC_WIN
                // set_unicode_input_mode(UNICODE_MODE_WINDOWS);
                clear_keyboard();
            }
            return false; // stop processing
        case SET_MAC:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_MAC);
                keymap_config.swap_lctl_lgui = keymap_config.swap_rctl_rgui = 1;
                eeconfig_update_keymap(keymap_config.raw);
                // TODO: set mac unicode mode, UC_MAC
                // set_unicode_input_mode(UNICODE_MODE_MACOS);
                clear_keyboard();
            }
            return false; // stop processing
    }
    return true; // continue processing
}


// Most copied from the QMK docs & inspiried by the ZSA configurator

typedef enum {
    NO_TAP = 0,
    SINGLE_TAP,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
} td_step_t;

typedef struct {
    uint16_t on_tap;
    uint16_t on_hold;
    uint16_t on_double_tap;
    uint16_t on_double_hold;
    uint16_t timing;
    bool hold_does_tap;
    td_step_t step;
} td_tap_t;

td_tap_t dance_state[DANCE_MAX] = {
    [DANCE_TSHIFT] = {
        .on_tap = OSM(MOD_RSFT),
        .on_hold = KC_RSFT,
        .on_double_tap = KC_CAPS,
        .on_double_hold = KC_RCTL,
        .timing = TAPPING_TERM,
        .hold_does_tap = false,
        .step = NO_TAP
    },
    [DANCE_CVVV] = {
        .on_tap = LCTL(KC_C),
        .on_hold = LCTL(KC_V),
        .on_double_tap = LCTL(LSFT(KC_C)),
        .on_double_hold = LCTL(LSFT(KC_V)),
        .timing = (TAPPING_TERM * 2),
        .hold_does_tap = true,
        .step = NO_TAP
    },
    [DANCE_TEST] = {
        .on_tap = LSFT(KC_A),
        .on_hold = (KC_B),
        .on_double_tap = LSFT(KC_C),
        .on_double_hold = (KC_D),
        .timing = 185,
        .hold_does_tap = true,
        .step = NO_TAP
    },
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... (QK_TAP_DANCE + DANCE_MAX - 1):;
            uint8_t index = keycode & 0xff;
            return dance_state[index].timing;
        default:
            return TAPPING_TERM;
    }
}

uint8_t dance_step(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}

// Honor the mod swaps for the current base layer.
uint16_t dance_code_mod_config(uint16_t code) {
    if (QK_MODS <= code && code <= QK_MODS_MAX) {
        uint16_t mods = (uint16_t)mod_config(QK_MODS_GET_MODS(code));
        code = (mods << 8) | QK_MODS_GET_BASIC_KEYCODE(code);
    }
    return code;
}

void dance_tap_code(uint16_t code) {
    tap_code16(dance_code_mod_config(code));
}
void dance_register_code(uint16_t code) {
    register_code16(dance_code_mod_config(code));
}
void dance_unregister_code(uint16_t code) {
    wait_ms(5);
    unregister_code16(dance_code_mod_config(code));
}

void on_dance(qk_tap_dance_state_t *state, void *user_data);
void dance_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance(qk_tap_dance_state_t *state, void *user_data) {
    td_tap_t *tap = (td_tap_t *)user_data;
    if (state->count < 3)
        return;
    uint16_t code = tap->on_tap;
    if(state->count == 3) { // catch up for triple-tap
        dance_tap_code(code);
        dance_tap_code(code);
        dance_tap_code(code);
    }
    if(state->count > 3) {
        dance_tap_code(code);
    }
}

void dance_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_tap_t *tap = (td_tap_t *)user_data;
    tap->step = dance_step(state);
    switch (tap->step) {
        case SINGLE_TAP:
            if (IS_QK_ONE_SHOT_MOD(tap->on_tap)) {
                uint16_t mods = QK_ONE_SHOT_MOD_GET_MODS(tap->on_tap);
                // see quantum/action.c near ACT_LMODS_TAP for some of this OSM logic.
                // Basically, tryinig to emulate OSM(MOD_RSFT) on single tap.
                set_oneshot_mods(get_oneshot_mods() | mod_config(mods));
            }
            else {
                dance_tap_code(tap->on_tap);
            }
            break;
        case SINGLE_HOLD:
            if (tap->hold_does_tap)
                dance_tap_code(tap->on_hold);
            else
                dance_register_code(tap->on_hold);
            break;
        case DOUBLE_TAP:
            dance_tap_code(tap->on_double_tap);
            break;
        case DOUBLE_HOLD:
            if (tap->hold_does_tap)
                dance_tap_code(tap->on_double_hold);
            else
                dance_register_code(tap->on_double_hold);
            break;
        case DOUBLE_SINGLE_TAP:
            dance_tap_code(tap->on_double_hold);
            break;
        default:
            break;
    }
}

void dance_reset(qk_tap_dance_state_t *state, void *user_data) {
    td_tap_t *tap = (td_tap_t *)user_data;
    switch (tap->step) {
        case SINGLE_HOLD:
            if (!tap->hold_does_tap)
                dance_unregister_code(tap->on_tap);
            break;
        case DOUBLE_HOLD:
            if (!tap->hold_does_tap)
                dance_unregister_code(tap->on_double_hold);
            break;
        default:
            break;
    }
    tap->step = NO_TAP;
}

#define REGISTER_DANCE(x) [x] = {.fn = {on_dance, dance_finished, dance_reset}, .user_data = &(dance_state[x])}
qk_tap_dance_action_t tap_dance_actions[] = {
    REGISTER_DANCE(DANCE_TSHIFT),
    REGISTER_DANCE(DANCE_CVVV),
    REGISTER_DANCE(DANCE_TEST),
};
