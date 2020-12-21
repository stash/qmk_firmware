#include "quantum.h"
#include "superalttab.h"

// based off of https://github.com/qmk/qmk_firmware/blob/master/keyboards/kyria/keymaps/j-inc/keymap.c
#ifndef SUPER_ALT_TAB_TIMEOUT
#    define SUPER_ALT_TAB_TIMEOUT 500
#endif
static uint16_t super_alt_tab_timer = 0; // 0 = unset, so be sure to set it to 1 when active
static void start_super_alt_tab_timer(void) {
    super_alt_tab_timer = timer_read();
    // avoid wasting a bool by pretending the timer is 1ms in the future:
    if (super_alt_tab_timer == 0) super_alt_tab_timer = 1;
}

// call this from an encoder change
void super_alt_tab_encoder(bool reverse) {
    if (super_alt_tab_timer == 0) {
        register_code(KC_LALT);
    }
    start_super_alt_tab_timer();
    tap_code16(reverse ? LSFT(KC_TAB) : KC_TAB);
}

// call from process_record_user, pass in record->event.pressed
void super_alt_tab_key(bool pressed) {
    if (pressed) {
        register_code(KC_LALT);
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
        unregister_code(KC_LALT);
        super_alt_tab_timer = 0;
    }
}

