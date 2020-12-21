#pragma once

// call this from an encoder change
void super_alt_tab_encoder(bool reverse);
// call from process_record_user, pass in record->event.pressed
void super_alt_tab_key(bool pressed);
// call from matrix_scan_user():
void check_super_alt_tab(void);
