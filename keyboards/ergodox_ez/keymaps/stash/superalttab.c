#include "quantum.h"
#include "keycode_config.h"
#include "superalttab.h"

extern keymap_config_t keymap_config;

// based off of https://github.com/qmk/qmk_firmware/blob/master/keyboards/kyria/keymaps/j-inc/keymap.c
#ifndef SUPER_ALT_TAB_TIMEOUT
#    define SUPER_ALT_TAB_TIMEOUT 500
#endif
static uint16_t super_alt_tab_timer = 0; // 0 = unset, so be sure to set it to 1 when active

static void start_super_alt_tab_timer(void) {
    super_alt_tab_timer = timer_read() | 1; // force non-zero
}

static uint8_t super_alt_tab_modifier(void) {
    // depending if it appears to be a Mac or not:
    return keymap_config.swap_lctl_lgui ? KC_LGUI : KC_LALT;
}

// call this from an encoder change
void super_alt_tab_encoder(bool reverse) {
    if (super_alt_tab_timer == 0) {
        register_code(super_alt_tab_modifier());
    }
    start_super_alt_tab_timer();
    tap_code16(reverse ? LSFT(KC_TAB) : KC_TAB);
}

// call from process_record_user, pass in record->event.pressed
void super_alt_tab_key(bool pressed) {
    if (pressed) {
        register_code(super_alt_tab_modifier());
        tap_code16(KC_TAB);
        // stop the timeout while the key is down
        super_alt_tab_timer = 0;
    } else { // released
        start_super_alt_tab_timer();
    }
}

// call this from matrix_scan_user()
void check_super_alt_tab(void) {
    if (super_alt_tab_timer != 0 && timer_elapsed(super_alt_tab_timer) > SUPER_ALT_TAB_TIMEOUT) {
        unregister_code(super_alt_tab_modifier());
        super_alt_tab_timer = 0;
    }
}

