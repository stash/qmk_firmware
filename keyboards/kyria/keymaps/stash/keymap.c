/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 * Copyright 2020 Jeremy Stashewsky <jstash@gmail.com>
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

enum custom_keycodes {
  ST_MACRO_Sleep = SAFE_RANGE,
  KC_CVVV,
  KC_OBRT,
};

enum layers {
    _BASE = 0,
    _LOWER,
    _RAISE,
    _ADJUST
};

#define ROT_L KC_NO
#define ROT_R KC_NO
#define XXLYRXX _______
// TODO: ctrl+alt+delete

#define LS_GRV LSFT_T(KC_GRV)
#define RS_MINS RSFT_T(KC_MINS)
#define KC_CADL LALT(LCTL(KC_TAB))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_stack(
//    _______, _______, _______, _______, _______, _______, _______, _______, _______,
      KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,
      KC_EQL,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V, LT(_RAISE,KC_ENT), ALT_T(KC_DEL),
                                 ROT_L, KC_LGUI, LT(_LOWER,KC_ESC), KC_SPC, CTL_T(KC_BSPC),

//    _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSLASH,
                                 KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
      KC_LEAD, KC_CVVV,          KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RS_MINS,
      RCTL_T(KC_ENT), OSM(MOD_RSFT), TT(_LOWER), TT(_RAISE), ROT_R
    ),
    [_LOWER] = LAYOUT_stack(
      _______, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_AMPR,
      _______, KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_ASTR,
      _______, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_GRV,  _______, KC_DEL,
                                   ROT_L, KC_APP,  XXLYRXX, _______, KC_BSPC,

                                 _______, KC_KP_7, KC_KP_8, KC_KP_9, XXXXXXX,     XXXXXXX,
                                 _______, KC_KP_4, KC_KP_5, KC_KP_6, KC_KP_MINUS, KC_KP_ASTERISK,
      _______, _______,          _______, KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_PLUS,  KC_KP_SLASH,
      KC_KP_ENTER, KC_KP_0,  XXLYRXX, KC_KP_DOT, ROT_R
    ),
    [_RAISE] = LAYOUT_stack(
      KC_VOLU, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
      KC_MUTE, KC_BTN4, KC_WH_L, KC_MS_U, KC_WH_R, KC_WH_U,
      KC_VOLD, KC_BTN5, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, XXLYRXX, _______,
                                   ROT_L, _______, KC_BTN2, KC_BTN1, KC_BTN3,

                                 KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MPLY,
                                 _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_MNXT,
      _______, _______,          _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_MPRV,
      _______, _______, _______, XXLYRXX, ROT_R
    ),
    [_ADJUST] = LAYOUT_stack(
    // TODO: Add GUI/CTL swaps, but somehow without having to enable bootmagic. Can maybe write to eeprom directly? Copy some code?
      KC_NLCK, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,
      KC_CAPS, KC_F11,  KC_F12,  _______, _______, KC_OBRT,
      KC_SLCK, EEP_RST, _______, _______, NK_TOGG, CG_TOGG, XXLYRXX, KC_CADL,
                          ROT_L,   ST_MACRO_Sleep, XXLYRXX, _______, _______,

                                 KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_PSCR,
                                 KC_OBRT, _______, _______, _______, _______, KC_PAUS,
      _______, _______,          _______, _______, _______, _______, _______, KC_SYSREQ,
      _______, _______, XXLYRXX, XXLYRXX, ROT_R
    ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    // Activate ADJUST layer when both LOWER and RAISE are being held
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

// from cwebster2's keymap:
void suspend_power_down_user() {
    oled_clear();
    oled_off();
}

static void render_kyria_logo(void) {
    static const char PROGMEM kyria_logo[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
        0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };
    oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
}

static void render_qmk_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write_P(qmk_logo, false);
}

extern keymap_config_t keymap_config;

static void render_status(void) {
    // QMK Logo and version information
    render_qmk_logo();

    oled_write_P(PSTR("Kyria Stash Edition\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust\n"), false);
            break;
        default:
            oled_write_P(PSTR("\n"), false);
    }
// x18 x19 are up down arrows, 1a 1b are right left
    oled_write_P(PSTR("Dial: \x1b\x1a\n"), false);

    uint8_t mods = get_mods() | get_weak_mods();
    oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("SHFT ") : PSTR("     "), false);
    oled_write_P((mods & MOD_MASK_CTRL) ? PSTR("CTRL ") : PSTR("     "), false);
    oled_write_P((mods & MOD_MASK_ALT) ? PSTR("ALT ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_GUI) ? PSTR("GUI ") : PSTR("    "), false);
    oled_write_P(PSTR("\n"), false);


    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUMLK ") : PSTR("      "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPLK ") : PSTR("      "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLK ") : PSTR("      "), false);

    oled_write_P(keymap_config.swap_lctl_lgui ? PSTR("Mc") : PSTR("PC"), false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_kyria_logo();
    }
}
#endif // OLED_DRIVER_ENABLE

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { // Left encoder
        tap_code(clockwise ? KC_UP : KC_DOWN);
    }
    else if (index == 1) { // Right encoder
        tap_code(clockwise ? KC_LEFT : KC_RIGHT);
    }
}
#endif

#define USER_DELAY_SHORT SS_DELAY(50)
#define USER_DELAY SS_DELAY(100)
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t copy_paste_timer = 0;
  switch (keycode) {

    case ST_MACRO_Sleep:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI(SS_TAP(X_X)) USER_DELAY SS_TAP(X_U) USER_DELAY SS_TAP(X_S));
      }
      break;

    // From https://github.com/qmk/qmk_firmware/blob/master/keyboards/kyria/keymaps/thomasbaart/keymap.c#L161
    // but reversed to prevent accidental pastes
    case KC_CVVV:  // One key copy/paste
      if (record->event.pressed) {
        copy_paste_timer = timer_read();
      } else {
        if (timer_elapsed(copy_paste_timer) > TAPPING_TERM) {  // Hold, paste
          tap_code16(LCTL(KC_V));
        } else { // Tap, copy
          tap_code16(LCTL(KC_C));
        }
      }
      break;

    case KC_OBRT:
      oled_set_brightness(oled_get_brightness() + 0x10);
      break;
  }
  return true;
}

#ifdef SPLIT_EXTRA_DATA_ENABLE
#include "split_common/transport.h"

#if defined(USE_I2C)
#    error I2C split transport not supported yet
#endif
void get_extra_split_data_m2s_user(struct EXTRA_SPLIT_DATA_M2S *data) {
    uint8_t brightness = oled_get_brightness();
    if (brightness != data->oled_brightness) {
        data->oled_brightness = brightness;
    }
}
void handle_extra_split_data_m2s_user(struct EXTRA_SPLIT_DATA_M2S *data) {
    if (data->oled_brightness != oled_get_brightness()) {
        oled_set_brightness(data->oled_brightness);
    }
}
#endif
