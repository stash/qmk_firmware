#include "quantum.h"
#include "keycode_config.h"
#include "cvvv.h"

extern keymap_config_t keymap_config;
static uint16_t cvvv_timer = 0;

void process_cvvv(bool pressed) {
    if (pressed && cvvv_timer == 0) {
        cvvv_timer = timer_read() | 1;  // force non-zero
    } else if (!pressed && cvvv_timer > 0) {
        if (timer_elapsed(cvvv_timer) > TAPPING_TERM) {  // Hold, paste
            // depending if it appears to be a Mac or not:
            tap_code16(keymap_config.swap_lctl_lgui ? LGUI(KC_V) : LCTL(KC_V));
        } else {  // Tap, copy, not already timed out
            tap_code16(keymap_config.swap_lctl_lgui ? LGUI(KC_C) : LCTL(KC_C));
        }
        cvvv_timer = 0;
    }
}

// Call from matrix_scan_user
void check_cvvv(void) {
    if (cvvv_timer > 0 && timer_elapsed(cvvv_timer) > TAPPING_TERM) {
        tap_code16(keymap_config.swap_lctl_lgui ? LGUI(KC_V) : LCTL(KC_V));
        cvvv_timer = 0;
    }
}
