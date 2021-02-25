#include QMK_KEYBOARD_H
#include "version.h"
#include "keymap_german.h"
#include "keymap_nordic.h"
#include "keymap_french.h"
#include "keymap_spanish.h"
#include "keymap_hungarian.h"
#include "keymap_swedish.h"
#include "keymap_br_abnt2.h"
#include "keymap_canadian_multilingual.h"
#include "keymap_german_ch.h"
#include "keymap_jp.h"
#include "keymap_bepo.h"
#include "keymap_italian.h"
#include "keymap_slovenian.h"
#include "keymap_danish.h"
#include "keymap_norwegian.h"
#include "keymap_portuguese.h"
#include "keymap_contributions.h"
#include "keymap_czech.h"
#include "keymap_romanian.h"
#include "keymap_russian.h"
#include "keymap_uk.h"
#include "keymap_estonian.h"

#define KC_MAC_UNDO LGUI(KC_Z)
#define KC_MAC_CUT LGUI(KC_X)
#define KC_MAC_COPY LGUI(KC_C)
#define KC_MAC_PASTE LGUI(KC_V)
#define KC_PC_UNDO LCTL(KC_Z)
#define KC_PC_CUT LCTL(KC_X)
#define KC_PC_COPY LCTL(KC_C)
#define KC_PC_PASTE LCTL(KC_V)
#define ES_LESS_MAC KC_GRAVE
#define ES_GRTR_MAC LSFT(KC_GRAVE)
#define ES_BSLS_MAC ALGR(KC_6)
#define NO_PIPE_ALT KC_GRAVE
#define NO_BSLS_ALT KC_EQUAL
#define LSA_T(kc) MT(MOD_LSFT | MOD_LALT, kc)
#define BP_NDSH_MAC ALGR(KC_8)

enum custom_keycodes {
  RGB_SLD = EZ_SAFE_RANGE,
  ST_MACRO_0,
  ST_MACRO_1,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_ergodox_pretty(
    KC_NO,          KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           TG(1),                                          TT(2),          KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_NO,
    KC_TAB,         KC_Q,           KC_W,           KC_F,           KC_P,           KC_B,           KC_LPRN,                                        KC_RPRN,        KC_J,           KC_L,           KC_U,           KC_Y,           KC_SCOLON,      KC_BSLASH,
    KC_EQUAL,       KC_A,           KC_R,           KC_S,           KC_T,           KC_G,                                                                           KC_H,           KC_N,           KC_E,           KC_I,           KC_O,           KC_QUOTE,
    LSFT_T(KC_GRAVE),KC_Z,           KC_X,           KC_C,           KC_D,           KC_V,           KC_ENTER,                                       KC_NO,          KC_K,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       RSFT_T(KC_MINUS),
    LALT(KC_TAB),   KC_QUOTE,       KC_LBRACKET,    KC_RBRACKET,    KC_ESCAPE,                                                                                                      KC_LEFT,        KC_DOWN,        KC_UP,          KC_RIGHT,       TT(3),
                                                                                                    KC_DELETE,      LGUI_T(KC_TAB), MT(MOD_MEH, KC_HOME),MT(MOD_MEH, KC_END),
                                                                                                                    OSM(MOD_LALT),  RALT_T(KC_PGUP),
                                                                                    KC_SPACE,       KC_BSPACE,      OSM(MOD_LCTL),  RCTL_T(KC_PGDOWN),KC_ENTER,       OSM(MOD_RSFT)
  ),
  [1] = LAYOUT_ergodox_pretty(
    KC_ESCAPE,      KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           _______,                                 _______, KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_BSPACE,
    _______, KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           KC_F9,                                          KC_F10,         KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           KC_BSLASH,
    _______, KC_A,           KC_S,           KC_D,           KC_F,           KC_G,                                                                           KC_H,           KC_J,           KC_K,           KC_L,           KC_SCOLON,      KC_QUOTE,
    KC_LSHIFT,      KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,           KC_F12,                                         KC_F11,         KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       KC_RSHIFT,
    KC_LCTRL,       KC_LALT,        KC_LBRACKET,    KC_RBRACKET,    KC_GRAVE,                                                                                                       KC_LEFT,        KC_DOWN,        KC_UP,          KC_RIGHT,       KC_RCTRL,
                                                                                                    _______, KC_INSERT,      _______, _______,
                                                                                                                    _______, _______,
                                                                                    KC_SPACE,       KC_ENTER,       _______, _______, _______, _______
  ),
  [2] = LAYOUT_ergodox_pretty(
    KC_NO,          KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          _______,                                 _______, KC_F6,          KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_NO,
    _______, KC_F11,         KC_F12,         KC_PSCREEN,     KC_NUMLOCK,     KC_PAUSE,       KC_LPRN,                                        KC_RPRN,        KC_AUDIO_VOL_UP,KC_KP_7,        KC_KP_8,        KC_KP_9,        KC_KP_SLASH,    KC_BSLASH,
    _______, KC_MEDIA_PREV_TRACK,KC_MEDIA_PLAY_PAUSE,KC_MEDIA_STOP,  KC_MEDIA_NEXT_TRACK,KC_SCROLLLOCK,                                                                  KC_AUDIO_MUTE,  KC_KP_4,        KC_KP_5,        KC_KP_6,        KC_KP_ASTERISK, KC_QUOTE,
    LSFT_T(KC_GRAVE),LCTL(KC_Z),     LCTL(KC_X),     LCTL(KC_C),     LCTL(KC_D),     LCTL(KC_V),     KC_ENTER,                                       _______, KC_AUDIO_VOL_DOWN,KC_KP_1,        KC_KP_2,        KC_KP_3,        KC_KP_MINUS,    RSFT_T(KC_MINUS),
    KC_NO,          ST_MACRO_0,     ST_MACRO_1,     KC_NO,          KC_ESCAPE,                                                                                                      KC_KP_0,        KC_KP_DOT,      KC_KP_DOT,      KC_KP_PLUS,     TT(3),
                                                                                                    _______, KC_NO,          _______, _______,
                                                                                                                    _______, _______,
                                                                                    KC_SPACE,       KC_BSPACE,      _______, _______, KC_KP_ENTER,    KC_KP_0
  ),
  [3] = LAYOUT_ergodox_pretty(
    MAGIC_TOGGLE_NKRO,_______, _______, _______, _______, LCTL(LGUI(KC_F4)),KC_NO,                                          KC_NO,          _______, _______, _______, _______, EEP_RST,        RESET,
    KC_NO,          KC_MS_BTN4,     KC_MS_UP,       KC_MS_BTN5,     KC_MS_WH_UP,    LCTL(LGUI(KC_RIGHT)),KC_ASTG,                                        KC_ASUP,        KC_NO,          KC_NO,          RGB_TOG,        LED_LEVEL,      TOGGLE_LAYER_COLOR,RGB_SLD,
    KC_MS_ACCEL0,   KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    KC_MS_WH_DOWN,  LCTL(LGUI(KC_LEFT)),                                                                RGB_HUI,        RGB_SAI,        RGB_VAI,        RGB_SPI,        RGB_MOD,        KC_NO,
    KC_LSHIFT,      KC_MS_WH_LEFT,  KC_NO,          KC_MS_WH_RIGHT, KC_NO,          LCTL(LGUI(KC_D)),KC_ASRP,                                        KC_ASDN,        RGB_HUD,        RGB_SAD,        RGB_VAD,        RGB_SPD,        LED_LEVEL,      KC_RSHIFT,
    _______, _______, _______, _______, _______,                                                                                                 KC_LEFT,        KC_DOWN,        KC_UP,          KC_RIGHT,       _______,
                                                                                                    _______, _______, _______, _______,
                                                                                                                    _______, _______,
                                                                                    KC_MS_BTN1,     KC_MS_BTN2,     KC_MS_BTN3,     _______, _______, _______
  ),
};


extern bool g_suspend_state;
extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
}

const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
    [2] = { {146,224,255}, {146,224,255}, {146,224,255}, {146,224,255}, {146,224,255}, {35,255,255}, {0,204,255}, {0,204,255}, {0,204,255}, {169,120,255}, {35,255,255}, {0,204,255}, {0,204,255}, {0,204,255}, {169,120,255}, {35,255,255}, {0,204,255}, {0,204,255}, {0,204,255}, {169,120,255}, {0,0,0}, {0,0,0}, {0,204,255}, {169,120,255}, {146,224,255}, {146,224,255}, {146,224,255}, {146,224,255}, {146,224,255}, {14,222,242}, {14,222,242}, {14,222,242}, {146,224,255}, {146,224,255}, {14,222,242}, {154,255,255}, {249,228,255}, {85,203,158}, {154,255,255}, {0,205,155}, {0,205,155}, {0,205,155}, {0,205,155}, {0,205,155}, {0,0,0}, {0,0,0}, {169,120,255}, {169,120,255} },

};

void set_layer_color(int layer) {
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
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

void rgb_matrix_indicators_user(void) {
  if (g_suspend_state || keyboard_config.disable_layer_led) { return; }
  switch (biton32(layer_state)) {
    case 2:
      set_layer_color(2);
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case ST_MACRO_0:
    if (record->event.pressed) {
      SEND_STRING(SS_LGUI(SS_TAP(X_X)) SS_DELAY(100) SS_TAP(X_U) SS_DELAY(100) SS_TAP(X_S));

    }
    break;
    case ST_MACRO_1:
    if (record->event.pressed) {
      SEND_STRING(SS_LGUI(SS_TAP(X_X)) SS_DELAY(100) SS_TAP(X_U) SS_DELAY(100) SS_TAP(X_S));

    }
    break;
    case RGB_SLD:
      if (record->event.pressed) {
        rgblight_mode(1);
      }
      return false;
  }
  return true;
}

uint32_t layer_state_set_user(uint32_t state) {

  uint8_t layer = biton32(state);

  ergodox_board_led_off();
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
  return state;
};
