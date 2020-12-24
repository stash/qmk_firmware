#include <string.h>
#include <stddef.h>

#include "config.h"
#include "matrix.h"
#include "quantum.h"

#define ROWS_PER_HAND (MATRIX_ROWS / 2)

#ifdef RGBLIGHT_ENABLE
#    include "rgblight.h"
#endif

#ifdef BACKLIGHT_ENABLE
#    include "backlight.h"
#endif

#ifdef ENCODER_ENABLE
#    include "encoder.h"
static pin_t encoders_pad[] = ENCODERS_PAD_A;
#    define NUMBER_OF_ENCODERS (sizeof(encoders_pad) / sizeof(pin_t))
#endif

__attribute__((weak)) bool get_extra_split_data_m2s_kb(uint8_t *data) { return false; }
__attribute__((weak)) void handle_extra_split_data_m2s_kb(uint8_t *data) {}
__attribute__((weak)) bool get_extra_split_data_m2s_user(uint8_t *data) { return false; }
__attribute__((weak)) void handle_extra_split_data_m2s_user(uint8_t *data) {}
__attribute__((weak)) void get_extra_split_data_s2m_kb(uint8_t *data) {}
__attribute__((weak)) void handle_extra_split_data_s2m_kb(uint8_t *data) {}
__attribute__((weak)) void get_extra_split_data_s2m_user(uint8_t *data) {}
__attribute__((weak)) void handle_extra_split_data_s2m_user(uint8_t *data) {}

#undef EXTRA_SPLIT_DATA_M2S
#if defined(EXTRA_SPLIT_DATA_M2S_KB) || defined(EXTRA_SPLIT_DATA_M2S_USER)
#    define EXTRA_SPLIT_DATA_M2S
#endif
#undef EXTRA_SPLIT_DATA_S2M
#if defined(EXTRA_SPLIT_DATA_S2M_KB) || defined(EXTRA_SPLIT_DATA_S2M_USER)
#    define EXTRA_SPLIT_DATA_S2M
#endif
#if defined(EXTRA_SPLIT_DATA_ENABLE) && !defined(SERIAL_USE_MULTI_TRANSACTION)
#    error "EXTRA_SPLIT_DATA_ENABLE requires SERIAL_USE_MULTI_TRANSACTION"
#endif

#if defined(USE_I2C)

#    include "i2c_master.h"
#    include "i2c_slave.h"
typedef struct _I2C_slave_buffer_t {
    matrix_row_t smatrix[ROWS_PER_HAND];
    uint8_t      backlight_level;
#    if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    rgblight_syncinfo_t rgblight_sync;
#    endif
#    ifdef ENCODER_ENABLE
    uint8_t encoder_state[NUMBER_OF_ENCODERS];
#    endif
#    ifdef WPM_ENABLE
    uint8_t current_wpm;
#    endif
    // TODO I2C S2M extra kb & user data
    // TODO I2C M2S extra kb & user data
} I2C_slave_buffer_t;

// TODO: check that I2C_slave_buffer_t is not over I2C_SLAVE_REG_COUNT size when adding extra data

static I2C_slave_buffer_t *const i2c_buffer = (I2C_slave_buffer_t *)i2c_slave_reg;

#    define I2C_BACKLIGHT_START offsetof(I2C_slave_buffer_t, backlight_level)
#    define I2C_RGB_START offsetof(I2C_slave_buffer_t, rgblight_sync)
#    define I2C_KEYMAP_START offsetof(I2C_slave_buffer_t, smatrix)
#    define I2C_ENCODER_START offsetof(I2C_slave_buffer_t, encoder_state)
#    define I2C_WPM_START offsetof(I2C_slave_buffer_t, current_wpm)

#    define TIMEOUT 100

#    ifndef SLAVE_I2C_ADDRESS
#        define SLAVE_I2C_ADDRESS 0x32
#    endif

// Get rows from other half over i2c
bool transport_master(matrix_row_t matrix[]) {
    i2c_readReg(SLAVE_I2C_ADDRESS, I2C_KEYMAP_START, (void *)matrix, sizeof(i2c_buffer->smatrix), TIMEOUT);

    // TODO: read extra data over I2C and call handle_extra_split_data_s2m_kb
    // TODO: read extra data over I2C and call handle_extra_split_data_s2m_user

    // write backlight info
#    ifdef BACKLIGHT_ENABLE
    uint8_t level = is_backlight_enabled() ? get_backlight_level() : 0;
    if (level != i2c_buffer->backlight_level) {
        if (i2c_writeReg(SLAVE_I2C_ADDRESS, I2C_BACKLIGHT_START, (void *)&level, sizeof(level), TIMEOUT) >= 0) {
            i2c_buffer->backlight_level = level;
        }
    }
#    endif

#    if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    if (rgblight_get_change_flags()) {
        rgblight_syncinfo_t rgblight_sync;
        rgblight_get_syncinfo(&rgblight_sync);
        if (i2c_writeReg(SLAVE_I2C_ADDRESS, I2C_RGB_START, (void *)&rgblight_sync, sizeof(rgblight_sync), TIMEOUT) >= 0) {
            rgblight_clear_change_flags();
        }
    }
#    endif

#    ifdef ENCODER_ENABLE
    i2c_readReg(SLAVE_I2C_ADDRESS, I2C_ENCODER_START, (void *)i2c_buffer->encoder_state, sizeof(i2c_buffer->encoder_state), TIMEOUT);
    encoder_update_raw(i2c_buffer->encoder_state);
#    endif

#    ifdef WPM_ENABLE
    uint8_t current_wpm = get_current_wpm();
    if (current_wpm != i2c_buffer->current_wpm) {
        if (i2c_writeReg(SLAVE_I2C_ADDRESS, I2C_WPM_START, (void *)&current_wpm, sizeof(current_wpm), TIMEOUT) >= 0) {
            i2c_buffer->current_wpm = current_wpm;
        }
    }
#    endif

    // TODO: call get_extra_split_data_m2s_kb and call i2c_writeReg
    // TODO: call get_extra_split_data_m2s_kb and call i2c_writeReg

    return true;
}

void transport_slave(matrix_row_t matrix[]) {
    // Copy matrix to I2C buffer
    memcpy((void *)i2c_buffer->smatrix, (void *)matrix, sizeof(i2c_buffer->smatrix));

    // TODO: call get_extra_split_data_s2m_kb and place that into i2c_buffer
    // TODO: call get_extra_split_data_s2m_user and place that into i2c_buffer

// Read Backlight Info
#    ifdef BACKLIGHT_ENABLE
    backlight_set(i2c_buffer->backlight_level);
#    endif

#    if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    // Update the RGB with the new data
    if (i2c_buffer->rgblight_sync.status.change_flags != 0) {
        rgblight_update_sync(&i2c_buffer->rgblight_sync, false);
        i2c_buffer->rgblight_sync.status.change_flags = 0;
    }
#    endif

#    ifdef ENCODER_ENABLE
    encoder_state_raw(i2c_buffer->encoder_state);
#    endif

#    ifdef WPM_ENABLE
    set_current_wpm(i2c_buffer->current_wpm);
#    endif

    // TODO: read from I2C and call handle_extra_split_data_m2s_kb
    // TODO: read from I2C and call handle_extra_split_data_m2s_user
}

void transport_master_init(void) { i2c_init(); }

void transport_slave_init(void) { i2c_slave_init(SLAVE_I2C_ADDRESS); }

#else  // USE_SERIAL

#    include "serial.h"

typedef struct _Serial_s2m_buffer_t {
    // TODO: if MATRIX_COLS > 8 change to uint8_t packed_matrix[] for pack/unpack
    matrix_row_t smatrix[ROWS_PER_HAND];

#    ifdef ENCODER_ENABLE
    uint8_t      encoder_state[NUMBER_OF_ENCODERS];
#    endif
} Serial_s2m_buffer_t;

typedef struct _Serial_m2s_buffer_t {
#    ifdef BACKLIGHT_ENABLE
    uint8_t backlight_level;
#    endif
#    ifdef WPM_ENABLE
    uint8_t current_wpm;
#    endif
} Serial_m2s_buffer_t;

#    ifdef EXTRA_SPLIT_DATA_ENABLE
typedef struct _Serial_s2m_extra_buffer_t {
#        ifdef EXTRA_SPLIT_DATA_S2M_KB
    uint8_t kb[EXTRA_SPLIT_DATA_S2M_KB];
#        endif
#        ifdef EXTRA_SPLIT_DATA_S2M_USER
    uint8_t user[EXTRA_SPLIT_DATA_S2M_USER];
#        endif
} Serial_s2m_extra_buffer_t;

typedef struct _Serial_m2s_extra_buffer_t {
#        ifdef EXTRA_SPLIT_DATA_M2S_KB
    uint8_t kb[EXTRA_SPLIT_DATA_M2S_KB];
#        endif
#        ifdef EXTRA_SPLIT_DATA_M2S_USER
    uint8_t user[EXTRA_SPLIT_DATA_M2S_USER];
#        endif
} Serial_m2s_extra_buffer_t;

volatile Serial_s2m_extra_buffer_t serial_s2m_extra_buffer = {};
volatile Serial_m2s_extra_buffer_t serial_m2s_extra_buffer = {};
uint8_t volatile status_extra                              = 0;
#    endif

#    if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
// When MCUs on both sides drive their respective RGB LED chains,
// it is necessary to synchronize, so it is necessary to communicate RGB
// information. In that case, define RGBLIGHT_SPLIT with info on the number
// of LEDs on each half.
//
// Otherwise, if the master side MCU drives both sides RGB LED chains,
// there is no need to communicate.

typedef struct _Serial_rgblight_t {
    rgblight_syncinfo_t rgblight_sync;
} Serial_rgblight_t;

volatile Serial_rgblight_t serial_rgblight = {};
uint8_t volatile status_rgblight           = 0;
#    endif

volatile Serial_s2m_buffer_t serial_s2m_buffer = {};
volatile Serial_m2s_buffer_t serial_m2s_buffer = {};
uint8_t volatile status0                       = 0;

enum serial_transaction_id {
    GET_SLAVE_MATRIX = 0,
#    if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    PUT_RGBLIGHT,
#    endif
#    ifdef EXTRA_SPLIT_DATA_ENABLE
    XCHG_EXTRA_DATA,
#    endif
};

SSTD_t transactions[] = {
    [GET_SLAVE_MATRIX] =
        {
            (uint8_t *)&status0,
            sizeof(serial_m2s_buffer),
            (uint8_t *)&serial_m2s_buffer,
            sizeof(serial_s2m_buffer),
            (uint8_t *)&serial_s2m_buffer,
        },
#    if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)
    [PUT_RGBLIGHT] =
        {
            (uint8_t *)&status_rgblight, sizeof(serial_rgblight), (uint8_t *)&serial_rgblight, 0, NULL  // no slave to master transfer
        },
#    endif
#    ifdef EXTRA_SPLIT_DATA_ENABLE
    [XCHG_EXTRA_DATA] =
        {
            (uint8_t *)&status_extra,
            sizeof(serial_m2s_extra_buffer),
            (uint8_t *)&serial_m2s_extra_buffer,
            sizeof(serial_s2m_extra_buffer),
            (uint8_t *)&serial_s2m_extra_buffer,
        },
#    endif
};

void transport_master_init(void) { soft_serial_initiator_init(transactions, TID_LIMIT(transactions)); }

void transport_slave_init(void) { soft_serial_target_init(transactions, TID_LIMIT(transactions)); }

#    if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_SPLIT)

// rgblight synchronization information communication.

void transport_rgblight_master(void) {
    if (rgblight_get_change_flags()) {
        rgblight_get_syncinfo((rgblight_syncinfo_t *)&serial_rgblight.rgblight_sync);
        if (soft_serial_transaction(PUT_RGBLIGHT) == TRANSACTION_END) {
            rgblight_clear_change_flags();
        }
    }
}

void transport_rgblight_slave(void) {
    if (status_rgblight == TRANSACTION_ACCEPTED) {
        rgblight_update_sync((rgblight_syncinfo_t *)&serial_rgblight.rgblight_sync, false);
        status_rgblight = TRANSACTION_END;
    }
}

#    else
#        define transport_rgblight_master()
#        define transport_rgblight_slave()
#    endif

#    ifdef EXTRA_SPLIT_DATA_ENABLE
void transport_extra_master(void) {
    bool txn_needed = false;
#        ifdef EXTRA_SPLIT_DATA_M2S_KB
    txn_needed |= get_extra_split_data_m2s_kb((uint8_t *)serial_m2s_extra_buffer.kb);
#        endif
#        ifdef EXTRA_SPLIT_DATA_M2S_USER
    txn_needed |= get_extra_split_data_m2s_user((uint8_t *)serial_m2s_extra_buffer.user);
#        endif
    if (!txn_needed) return;
    if (soft_serial_transaction(XCHG_EXTRA_DATA) == TRANSACTION_END) {
#        ifdef EXTRA_SPLIT_DATA_S2M_KB
        handle_extra_split_data_s2m_kb((uint8_t *)serial_s2m_extra_buffer.kb);
#        endif
#        ifdef EXTRA_SPLIT_DATA_S2M_USER
        handle_extra_split_data_s2m_user((uint8_t *)serial_s2m_extra_buffer.user);
#        endif
    }
}

void transport_extra_slave(void) {
    if (status_extra == TRANSACTION_ACCEPTED) {
        // because serial might mess with it during an interrupt?
        Serial_m2s_extra_buffer_t m2s_temp = serial_m2s_extra_buffer;
#        ifdef EXTRA_SPLIT_DATA_M2S_KB
        handle_extra_split_data_m2s_kb(&m2s_temp.kb[0]);
#        endif
#        ifdef EXTRA_SPLIT_DATA_M2S_USER
        handle_extra_split_data_m2s_user(&m2s_temp.user[0]);
#        endif

        Serial_s2m_extra_buffer_t s2m_temp;
#        ifdef EXTRA_SPLIT_DATA_S2M_KB
        get_extra_split_data_s2m_kb(&s2m_temp.kb[0]);
#        endif
#        ifdef EXTRA_SPLIT_DATA_S2M_USER
        get_extra_split_data_s2m_user(&s2m_temp.user[0]);
#        endif
        // likewise get as close to an atomic copy as possible
        serial_s2m_extra_buffer = s2m_temp;

        status_extra = TRANSACTION_END;
    }
}
#    else
#        define transport_extra_master()
#        define transport_extra_slave()
#    endif

bool transport_master(matrix_row_t matrix[]) {
#    ifndef SERIAL_USE_MULTI_TRANSACTION
    if (soft_serial_transaction() != TRANSACTION_END) {
        return false;
    }
#    else

    transport_extra_master();
    transport_rgblight_master();
    if (soft_serial_transaction(GET_SLAVE_MATRIX) != TRANSACTION_END) {
        return false;
    }
#    endif

    // TODO:  if MATRIX_COLS > 8 change to unpack()
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        matrix[i] = serial_s2m_buffer.smatrix[i];
    }

#    ifdef BACKLIGHT_ENABLE
    // Write backlight level for slave to read
    serial_m2s_buffer.backlight_level = is_backlight_enabled() ? get_backlight_level() : 0;
#    endif

#    ifdef ENCODER_ENABLE
    encoder_update_raw((uint8_t *)serial_s2m_buffer.encoder_state);
#    endif

#    ifdef WPM_ENABLE
    // Write wpm to slave
    serial_m2s_buffer.current_wpm = get_current_wpm();
#    endif

    return true;
}

void transport_slave(matrix_row_t matrix[]) {
    transport_rgblight_slave();
    transport_extra_slave();

    // TODO: if MATRIX_COLS > 8 change to pack()
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        serial_s2m_buffer.smatrix[i] = matrix[i];
    }

#    ifdef BACKLIGHT_ENABLE
    backlight_set(serial_m2s_buffer.backlight_level);
#    endif

#    ifdef ENCODER_ENABLE
    encoder_state_raw((uint8_t *)serial_s2m_buffer.encoder_state);
#    endif

#    ifdef WPM_ENABLE
    set_current_wpm(serial_m2s_buffer.current_wpm);
#    endif
}

#endif
