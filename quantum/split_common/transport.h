#pragma once

#include "common/matrix.h"

void transport_master_init(void);
void transport_slave_init(void);

// returns false if valid data not received from slave
bool transport_master(matrix_row_t matrix[]);
void transport_slave(matrix_row_t matrix[]);

#ifdef EXTRA_SPLIT_DATA_ENABLE
#    ifdef EXTRA_SPLIT_DATA_M2S_KB
bool get_extra_split_data_m2s_kb(uint8_t data[EXTRA_SPLIT_DATA_M2S_KB]);
void handle_extra_split_data_m2s_kb(uint8_t data[EXTRA_SPLIT_DATA_M2S_KB]);
#    endif
#    ifdef EXTRA_SPLIT_DATA_M2S_USER
bool get_extra_split_data_m2s_user(uint8_t data[EXTRA_SPLIT_DATA_M2S_USER]);
void handle_extra_split_data_m2s_user(uint8_t data[EXTRA_SPLIT_DATA_M2S_USER]);
#    endif
#    ifdef EXTRA_SPLIT_DATA_S2M_KB
bool get_extra_split_data_s2m_kb(uint8_t data[EXTRA_SPLIT_DATA_S2M_KB]);
void handle_extra_split_data_s2m_kb(uint8_t data[EXTRA_SPLIT_DATA_S2M_KB]);
#    endif
#    ifdef EXTRA_SPLIT_DATA_S2M_USER
bool get_extra_split_data_s2m_user(uint8_t data[EXTRA_SPLIT_DATA_S2M_USER]);
void handle_extra_split_data_s2m_user(uint8_t data[EXTRA_SPLIT_DATA_S2M_USER]);
#    endif
#endif
