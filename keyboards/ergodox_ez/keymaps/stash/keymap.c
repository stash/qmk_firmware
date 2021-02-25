#include "../../ergodox_ez.h"
#include "version.h"
#include "superalttab.h"
#include "cvvv.h"

#ifdef AUTO_SHIFT_ENABLE
#  include "process_auto_shift.h"
#endif
#include "blinky.h"

#ifndef _______
#  define _______ KC_TRANSPARENT
#endif

enum custom_keycodes {
  RGB_SLD = EZ_SAFE_RANGE,
  ST_MACRO_Sleep,
  KC_CVVV,
  KC_SUPERALTTAB,
  KC_SuAT = KC_SUPERALTTAB,
};

enum my_layers {
  _BASE,
  _QWERTY,
  _FNUM,
  _ADJUST,
};

#define WKSP_NEW LCTL(LGUI(KC_D))
#define WKSP_NEXT LCTL(LGUI(KC_RIGHT))
#define WKSP_PREV LCTL(LGUI(KC_LEFT))
#define WKSP_DEL LCTL(LGUI(KC_F4))

// space-cadet style minus (except the space cadet code doesn't actually work for it)
#define KC_SMIN RSFT_T(KC_MINS)
#define KC_SGRV LSFT_T(KC_GRAVE)

// TODO: this needs special treatment when in "Mac mode" to send Opt+BKSP instead
// Support a Enter+Bksp chord for quick word deletion
// On my Kyria Enter is RCTL_T(KC_ENT) already, but could use the same special treatment for Mac
#define KC_SNTR RCTL_T(KC_ENTER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_ergodox_pretty(
    KC_CVVV,        KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           TG(_QWERTY),                                    TT(_FNUM),      KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_MPLY,
    KC_TAB,         KC_Q,           KC_W,           KC_F,           KC_P,           KC_B,           KC_LPRN,                                        KC_RPRN,        KC_J,           KC_L,           KC_U,           KC_Y,           KC_SCOLON,      KC_BSLASH,
    KC_EQUAL,       KC_A,           KC_R,           KC_S,           KC_T,           KC_G,                                                                           KC_H,           KC_N,           KC_E,           KC_I,           KC_O,           KC_QUOTE,
    KC_SGRV,        KC_Z,           KC_X,           KC_C,           KC_D,           KC_V,           KC_ENTER,                                       KC_LEAD,        KC_K,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       KC_SMIN,
    KC_SuAT,        KC_QUOT,        KC_LBRACKET,    KC_RBRACKET,    KC_ESC,                                                                                                         KC_LEFT,        KC_DOWN,        KC_UP,          KC_RIGHT,       TT(_ADJUST),
                                                                                                    KC_DELETE,      LGUI_T(KC_TAB), MEH_T(KC_HOME), HYPR_T(KC_END),
                                                                                                                    OSM(MOD_LALT),  RALT_T(KC_PGUP),
                                                                                    KC_SPACE,       KC_BSPACE,      OSM(MOD_LCTL),  RCTL_T(KC_PGDN), KC_SNTR, OSM(MOD_RSFT)
  ),
  [_QWERTY] = LAYOUT_ergodox_pretty(
    KC_ESCAPE,      KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           _______,                                        _______,        KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_BSPACE,
    _______,        KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           KC_F9,                                          KC_F10,         KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           KC_BSLASH,
    _______,        KC_A,           KC_S,           KC_D,           KC_F,           KC_G,                                                                           KC_H,           KC_J,           KC_K,           KC_L,           KC_SCOLON,      KC_QUOTE,
    KC_LSHIFT,      KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,           KC_F12,                                         KC_F11,         KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       KC_RSHIFT,
    KC_LCTRL,       KC_LALT,        KC_LBRACKET,    KC_RBRACKET,    KC_GRAVE,                                                                                                       _______,        _______,        _______,        _______,        KC_RCTRL,
                                                                                                    KC_DELETE,      KC_INSERT,      _______,        _______,
                                                                                                                    KC_LALT,        _______,
                                                                                    KC_SPACE,       KC_ENTER,       KC_LCTRL,       _______,        _______,        _______
  ),
  [_FNUM] = LAYOUT_ergodox_pretty(
    KC_CVVV,        KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          _______,                                        _______,        KC_F6,         KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_MPLY,
    _______,        KC_F11,         KC_F12,         KC_PSCREEN,     KC_NUMLOCK,     KC_PAUSE,       KC_LPRN,                                        KC_RPRN,        KC_VOLU,       KC_KP_7,        KC_KP_8,        KC_KP_9,        KC_KP_SLASH,    KC_BSLASH,
    _______,        KC_MPRV,        KC_MPLY,        KC_MSTP,        KC_MNXT,        KC_SCROLLLOCK,                                                                  KC_MUTE,       KC_KP_4,        KC_KP_5,        KC_KP_6,        KC_KP_ASTERISK, KC_QUOTE,
    KC_LSHIFT,      LCTL(KC_Z),     LCTL(KC_X),     LCTL(KC_C),     LCTL(KC_D),     LCTL(KC_V),     KC_ENTER,                                       KC_LEAD,        KC_VOLD,       KC_KP_1,        KC_KP_2,        KC_KP_3,        KC_KP_MINUS,    KC_SMIN,
    KC_SuAT,        ST_MACRO_Sleep, ST_MACRO_Sleep, _______,        KC_GRAVE,                                                                                                      KC_KP_0,        KC_KP_DOT,      KC_KP_DOT,      KC_KP_PLUS,     TT(_ADJUST),
                                                                                                    KC_DELETE,      LGUI_T(KC_TAB), _______,        _______,
                                                                                                                    KC_LALT,        _______,
                                                                                    KC_SPACE,       KC_BSPACE,      KC_LCTRL,       _______,        KC_KP_ENTER,    KC_KP_0
  ),
  [_ADJUST] = LAYOUT_ergodox_pretty(
    NK_ON,          NK_OFF,         _______,        _______,        _______,        WKSP_DEL,       KC_NO,                                          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          EEP_RST,        RESET,
    KC_NO,          KC_MS_BTN4,     KC_MS_UP,       KC_MS_BTN5,     KC_MS_WH_UP,    WKSP_NEXT,      KC_ASTG,                                        KC_ASUP,        KC_NO,          KC_NO,          RGB_TOG,        LED_LEVEL,  TOGGLE_LAYER_COLOR, RGB_SLD,
    KC_MS_ACCEL0,   KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    KC_MS_WH_DOWN,  WKSP_PREV,                                                                      RGB_HUI,        RGB_SAI,        RGB_VAI,        RGB_SPI,        RGB_MOD,        KC_NO,
    KC_LSHIFT,      KC_MS_WH_LEFT,  KC_NO,          KC_MS_WH_RIGHT, KC_NO,          WKSP_NEW,       KC_ASRP,                                        KC_ASDN,        RGB_HUD,        RGB_SAD,        RGB_VAD,        RGB_SPD,        LED_LEVEL,      KC_RSHIFT,
    _______,        KC_NO,          KC_NO,          _______,        _______,                                                                                                        KC_LEFT,        KC_DOWN,        KC_UP,          KC_RIGHT,       _______,
                                                                                                    KC_MS_BTN3,     CG_SWAP,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    KC_MS_BTN1,     KC_MS_BTN2,     CG_NORM,        _______,        _______,        KC_CAPS
  ),

};


extern bool g_suspend_state;
extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
}

// MODIFIED from Oryx: pack ledmap; having just a higher layer set wastes a lot of space!
enum layer_ledmaps {
  _FNUM_LEDS
};
const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
    [_FNUM_LEDS] = { {146,224,255}, {146,224,255}, {146,224,255}, {146,224,255}, {146,224,255}, {35,255,255}, {0,204,255}, {0,204,255}, {0,204,255}, {169,120,255}, {35,255,255}, {0,204,255}, {0,204,255}, {0,204,255}, {169,120,255}, {35,255,255}, {0,204,255}, {0,204,255}, {0,204,255}, {169,120,255}, {0,0,0}, {0,0,0}, {0,204,255}, {169,120,255}, {146,224,255}, {146,224,255}, {146,224,255}, {146,224,255}, {146,224,255}, {14,222,242}, {14,222,242}, {14,222,242}, {146,224,255}, {146,224,255}, {14,222,242}, {154,255,255}, {249,228,255}, {85,203,158}, {154,255,255}, {0,205,155}, {0,205,155}, {0,205,155}, {0,205,155}, {0,205,155}, {0,0,0}, {0,0,0}, {169,120,255}, {169,120,255} },
};


void set_layer_color(int layer) {
  float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb( hsv );
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

// MODIFIED from Oryx reference: only set color when layer is actually changing
// Needed to preserve the colors set dynamically by Blinky
static int old_layer_ind = -1;
void rgb_matrix_indicators_user(void) {
  if (g_suspend_state || keyboard_config.disable_layer_led) { return; }

  int layer = biton32(layer_state);
  if (layer == old_layer_ind) return; // don't clear or re-set layers if they aren't changing

  switch (layer) {
    case _FNUM:
      set_layer_color(_FNUM_LEDS);
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
  old_layer_ind = layer;
}

LEADER_EXTERNS();

void matrix_scan_user(void) {
  check_super_alt_tab();
  check_cvvv();

  // instead of LEADER_DICTONARY(), try to match the pattern *before* the timeout is reached
  if (leading) {

    SEQ_ONE_KEY(KC_COMMA) { // Matched <Angle-brackets>
      SEND_STRING(SS_LSFT(SS_TAP(X_COMMA) SS_TAP(X_DOT)) SS_TAP(X_LEFT));
      goto end_leader;
    }
    SEQ_ONE_KEY(KC_LBRACKET) { // Matched {Curly-braces}
      SEND_STRING(SS_LSFT(SS_TAP(X_LBRACKET) SS_TAP(X_RBRACKET)) SS_TAP(X_LEFT));
      goto end_leader;
    }
    SEQ_ONE_KEY(KC_RBRACKET) { // Matched [Square-brackets]
      SEND_STRING(SS_TAP(X_LBRACKET) SS_TAP(X_RBRACKET) SS_TAP(X_LEFT));
      goto end_leader;
    }
    SEQ_ONE_KEY(KC_LPRN) { // Matched (Parentheses)
      SEND_STRING(SS_LSFT(SS_TAP(X_9) SS_TAP(X_0)) SS_TAP(X_LEFT));
      goto end_leader;
    }
    // TODO: double and single quotes, refactor all of the above into a function with two keycodes as parameters

#ifdef PRIVATE_LEADERS
#   include "private-leaders.c"
#endif

    // if no match, and we've reached the timeout, put an end to it
    if (timer_elapsed(leader_time) > LEADER_TIMEOUT)
      goto end_leader;
  }

  return;

end_leader:
  leading = false;
  leader_end();
}


#define USER_DELAY_SHORT SS_DELAY(50)
#define USER_DELAY SS_DELAY(100)
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ST_MACRO_Sleep:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_TAP(X_X)) USER_DELAY SS_TAP(X_U) USER_DELAY SS_TAP(X_S));
            }
            return false;

        case RGB_SLD:
            if (record->event.pressed) {
                rgblight_mode(1);
            }
            return false;

        // From https://github.com/qmk/qmk_firmware/blob/master/keyboards/kyria/keymaps/thomasbaart/keymap.c#L161
        // but reversed to prevent accidental pastes
        case KC_CVVV:  // One key copy/paste
            process_cvvv(record->event.pressed);
            return false;

        case KC_SuAT:
            super_alt_tab_key(record->event.pressed);
            return false;
    }
    return true;
}

uint32_t layer_state_set_user(uint32_t state) {
  uint8_t layer = get_highest_layer(state);

  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
  switch (layer) {
    case 1:
      ergodox_right_led_1_on();
      break;
    case 2:
      ergodox_right_led_2_on();
      break;
    case 3:
      ergodox_right_led_3_on();
      break;
    case 4:
      ergodox_right_led_1_on();
      ergodox_right_led_2_on();
      break;
    case 5:
      ergodox_right_led_1_on();
      ergodox_right_led_3_on();
      break;
    case 6:
      ergodox_right_led_2_on();
      ergodox_right_led_3_on();
      break;
    case 7:
      ergodox_right_led_1_on();
      ergodox_right_led_2_on();
      ergodox_right_led_3_on();
      break;
    default:
      break;
  }

#ifdef AUTO_SHIFT_ENABLE
  // Turn off auto-shift for QWERTY/gaming layer
  if (layer == _QWERTY) {
    autoshift_disable();
  } else if (!get_autoshift_state()) {
    autoshift_enable();
  }
#endif

  blinky_send_layer(layer);

  return state;
}
