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
#include <inttypes.h>
#include "superalttab.h"
#include "cvvv.h"
#include "split_common/transport.h"

#include "pointing_device.h"
#include "trackball.h"

// NOTE: if you change this, also change EXTRA_SPLIT_DATA_M2S_USER
typedef struct extra_m2s_user {
    uint8_t oled_brightness;
    uint8_t highest_layer;
} __attribute__((packed)) extra_m2s_user_t;

// NOTE: if you change this, also change EXTRA_SPLIT_DATA_S2M_USER
typedef struct extra_s2m_user {
    trackball_state_t trackball; // check that this is 5 bytes
} __attribute__((packed)) extra_s2m_user_t;

enum custom_keycodes {
  ST_MACRO_Sleep = SAFE_RANGE,
  KC_CVVV,
  KC_OBRT,
  TBALLTG,
};

enum layers {
    _BASE = 0,
    _LOWER,
    _RAISE,
    _ADJUST
};

static uint8_t mouse_buttons_held = 0;

#define XLAYERX _______

// Space-cadet-style shift keys for ` and -
#define KC_SGRV LSFT_T(KC_GRAVE)
#define KC_SMIN RSFT_T(KC_MINS)
// Ctrl+Alt+Delete
#define KC_CADL LALT(LCTL(KC_DEL))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// clang-format off
    [_BASE] = LAYOUT_stack(
//    _______, _______, _______, _______, _______, _______, _______, _______, _______,
      KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,
      KC_EQL,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,
      KC_SGRV, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V, LT(_RAISE,KC_ENT), ALT_T(KC_DEL),
                              KC_CVVV, KC_LGUI, LT(_LOWER,KC_ESC), KC_SPC, CTL_T(KC_BSPC),

//    _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSLASH,
                                 KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
      KC_BTN1, KC_LEAD,          KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_SMIN,
      RCTL_T(KC_ENT), OSM(MOD_RSFT), TT(_LOWER), TT(_RAISE), KC_MEH
    ),
    [_LOWER] = LAYOUT_stack(
      _______, KC_EXLM, KC_DLR,  KC_LCBR, KC_RCBR, KC_AMPR,
      _______, KC_AT,   KC_PERC, KC_LPRN, KC_RPRN, KC_ASTR,
      _______, KC_HASH, KC_CIRC, KC_LBRC, KC_RBRC, KC_GRV,  XLAYERX, _______,
                                 _______, KC_APP,  XLAYERX, _______, _______,

                                 XXXXXXX, KC_KP_7, KC_KP_8, KC_KP_9, _______, _______,
                                 XXXXXXX, KC_KP_4, KC_KP_5, KC_KP_6, KC_KP_ASTERISK, KC_KP_SLASH,
      _______, KC_KP_DOT,        XXXXXXX, KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_PLUS, KC_KP_MINUS,
      KC_KP_ENTER, KC_KP_0, XLAYERX, XLAYERX, _______
    ),
    [_RAISE] = LAYOUT_stack(
      KC_MUTE, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
      KC_VOLU, KC_WH_L, KC_BTN4, KC_MS_U, KC_BTN5, KC_WH_U,
      KC_VOLD, KC_WH_R, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, XLAYERX, KC_BTN3,
                                 _______, XXXXXXX, XLAYERX, KC_BTN1, KC_BTN2,

                                 KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MPLY,
                                 XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_MNXT,
      KC_RALT, XXXXXXX,          XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_MPRV,
      KC_RCTL, KC_CAPS, XLAYERX, XLAYERX, _______
    ),
    [_ADJUST] = LAYOUT_stack(
      KC_NLCK, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,
      KC_SLCK, KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, EEP_RST, XXXXXXX, KC_OBRT, NK_TOGG, CG_TOGG, XLAYERX, KC_INS,
                          _______, ST_MACRO_Sleep, XLAYERX, XXXXXXX, KC_CADL,

                                 KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_PAUS,
                                 XXXXXXX, DEBUG,   XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR,
      XXXXXXX, XXXXXXX,          TBALLTG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_SYSREQ,
      XXXXXXX, XXXXXXX, XLAYERX, XLAYERX, _______
    ),
// clang-format on
};

#ifdef ENCODER_ENABLE
// index == 0 is left hand
void encoder_update_user(uint8_t index, bool direction) {
    switch (get_highest_layer(layer_state)) {
        case _LOWER:
            if (index == 0) {
                tap_code(direction ? KC_PGUP : KC_PGDN);
            } else {
                super_alt_tab_encoder(direction);
            }
            break;
        case _RAISE:
            if (index == 0) {
                tap_code(direction ? KC_VOLU : KC_VOLD);
            } else {
                tap_code(direction ? KC_MPRV : KC_MNXT);
            }
            break;
        default:
            if (index == 0) {
                tap_code(direction ? KC_UP : KC_DOWN);
            } else {
                tap_code(direction ? KC_LEFT : KC_RIGHT);
            }
    }
}
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    // Activate ADJUST layer when both LOWER and RAISE are being held
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

static void render_qmk_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
        0
    };
    oled_write_P(qmk_logo, false);
}

extern keymap_config_t keymap_config;

static void render_layer(void) {
    // Host Keyboard Layer Status
    uint8_t layer = get_highest_layer(layer_state);
    bool invert_layer_text = layer != _BASE;
    oled_write_P(PSTR("Layer: "), false);
    switch (layer) {
        case _BASE:
            oled_write_P(PSTR("Default"), invert_layer_text);
            trackball_set_color(0x10,0x10,0x00,0x30);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower"), invert_layer_text);
            trackball_set_color(0xff,0x00,0x00,0x00);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise"), invert_layer_text);
            trackball_set_color(0x00,0xff,0x00,0x00);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust"), invert_layer_text);
            trackball_set_color(0x00,0x7f,0x7f,0x00);
            break;
        default:
            oled_write_P(PSTR("OTHER"), invert_layer_text);
            trackball_set_color(0x7f,0x00,0x7f,0x00);
            break;
    }
    oled_advance_page(true); // newline
}

char oled_debug_str[32] = {0};

static void render_master(void) {
    render_qmk_logo();

    oled_write_P(PSTR("Kyria Stash Edition\n"), false);

    render_layer();

    // x18 x19 are up down arrows, 1a 1b are right left
    //oled_write_P(PSTR("Dial: \x1b\x1a\n"), false);
    oled_write(oled_debug_str, false);
    oled_advance_page(true); // newline

    // Mods status
    uint8_t mods = get_mods() | get_weak_mods();
    oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("SHFT ") : PSTR("     "), false);
    oled_write_P((mods & MOD_MASK_CTRL) ? PSTR("CTRL ") : PSTR("     "), false);
    oled_write_P((mods & MOD_MASK_ALT) ? PSTR("ALT ") : PSTR("    "), false);
    oled_write_P((mods & MOD_MASK_GUI) ? PSTR("GUI ") : PSTR("    "), false);
    oled_advance_page(true); // newline

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUMLK ") : PSTR("      "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPLK ") : PSTR("      "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLK ") : PSTR("      "), false);

    oled_write_P(keymap_config.swap_lctl_lgui ? PSTR("Mac") : PSTR("Win"), false);
}

static void render_slave(void) {
    render_qmk_logo();

    render_layer();

    oled_write_P(PSTR("Trackball: "), false);
    if (trackball_status == TRACKBALL_OK) {
        oled_write_P(PSTR("OK!"), true);
    } else {
        char err[8];
        snprintf(err, sizeof(err), "Err %02x", (uint8_t)trackball_status);
        oled_write(err, false);
    }
    oled_advance_page(true); // newline

    oled_write(oled_debug_str, false);
    oled_advance_page(true); // newline
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_master();
    } else {
        render_slave();
    }
}
#endif // OLED_DRIVER_ENABLE

void override_mousekey(uint16_t keycode, keyrecord_t *record) {
    int button0 = keycode - KC_MS_BTN1;
    uint8_t bit = 1 << button0;
    if (record->event.pressed) mouse_buttons_held |= bit;
    else mouse_buttons_held &= ~bit;
}

#define USER_DELAY_SHORT SS_DELAY(50)
#define USER_DELAY SS_DELAY(100)
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t copy_paste_timer = 0;
  switch (keycode) {

    case ST_MACRO_Sleep:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI(SS_TAP(X_X)) USER_DELAY SS_TAP(X_U) USER_DELAY SS_TAP(X_S));
      }
      return false;

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
      return false;

    case KC_OBRT:
      oled_set_brightness(oled_get_brightness() + 0x10);
      return false;

    case TBALLTG:
        // TODO
        return false;

    // Intercept mousekeys so they can be sent with trackball positioning
    case KC_MS_BTN1 ... KC_MS_BTN5:
        override_mousekey(keycode, record);
        return false; // override

    default:
      // true: process key normally
      return true;
  }
}


#ifdef EXTRA_SPLIT_DATA_ENABLE
#define M2S_POLL_INTERVAL 20
bool get_extra_split_data_m2s_user(uint8_t data[EXTRA_SPLIT_DATA_M2S_USER]) {
//  static uint16_t poll_timer = 0;
//  if (timer_elapsed(poll_timer) < M2S_POLL_INTERVAL) {
//      return false;
//  }
//  poll_timer = timer_read();
    extra_m2s_user_t *extra = (extra_m2s_user_t *)data;
    extra->oled_brightness = oled_get_brightness();
    extra->highest_layer = get_highest_layer(layer_state);
    return true;
}
void handle_extra_split_data_m2s_user(uint8_t data[EXTRA_SPLIT_DATA_M2S_USER]) {
    extra_m2s_user_t *extra = (extra_m2s_user_t *)data;
    if (extra->oled_brightness != oled_get_brightness()) {
        oled_set_brightness(extra->oled_brightness);
    }
    layer_state = 1 << extra->highest_layer;
}


bool get_extra_split_data_s2m_user(uint8_t data[EXTRA_SPLIT_DATA_S2M_USER]) {
    extra_s2m_user_t *s2m = (extra_s2m_user_t *)data;
    trackball_get_raw(&(s2m->trackball));
    //snprintf(oled_debug_str, sizeof(oled_debug_str), "tbsg %ld:%ld:%d", (long)s2m->trackball.x,(long)s2m->trackball.y,(int)s2m->trackball.button);
    //dprintf("tbsg %ld:%ld:%d", (long)s2m->trackball.x,(long)s2m->trackball.y,(int)s2m->trackball.button);
    trackball_clear();
    return true;
}

void handle_extra_split_data_s2m_user(uint8_t data[EXTRA_SPLIT_DATA_S2M_USER]) {
    //dprintf("handle start\n");
    //for (int i=0; i<EXTRA_SPLIT_DATA_S2M_USER; i++) {
    //    dprintf("d[%d] = %d\n", i, (int)data[i]);
    //}
    //dprintf("handle end\n");
    extra_s2m_user_t *s2m = (extra_s2m_user_t *)data;
    snprintf(oled_debug_str, sizeof(oled_debug_str), "tbsm %ld:%ld:%d", (long)s2m->trackball.x,(long)s2m->trackball.y,(int)s2m->trackball.button);

    //snprintf(oled_debug_str, sizeof(oled_debug_str), "tb %04" PRIx16 " %04" PRIx16 " %02" PRIx8, x, y, btn);
    //printf("tb %04" PRIx16 " %04" PRIx16 " %02" PRIx8 "\n", x, y, btn);
    trackball_set_raw(s2m->trackball);
}

void pointing_device_task(void) {
    trackball_report(
        (get_highest_layer(layer_state) >= _LOWER),
        mouse_buttons_held
    );
}
#endif

void keyboard_post_init_user(void) {
    //trackball_init();
}

// only called on master side
void matrix_scan_user(void) {
    check_super_alt_tab();
}

// Called after transfer of serial data from master
void matrix_slave_scan_user(void) {
    //snprintf(oled_debug_str, sizeof(oled_debug_str), "sscanuser %02x", (int)trackball_status);
    trackball_poll();
}

// from cwebster2's keymap:
void suspend_power_down_user() {
    //trackball_reset();
    oled_clear();
    oled_off();
}
