#pragma once

#include "common/matrix.h"

void transport_master_init(void);
void transport_slave_init(void);

// returns false if valid data not received from slave
bool transport_master(matrix_row_t matrix[]);
void transport_slave(matrix_row_t matrix[]);

bool get_extra_split_data_m2s_kb(uint8_t *data);
void handle_extra_split_data_m2s_kb(uint8_t *data);
bool get_extra_split_data_m2s_user(uint8_t *data);
void handle_extra_split_data_m2s_user(uint8_t *data);
void get_extra_split_data_s2m_kb(uint8_t *data);
void handle_extra_split_data_s2m_kb(uint8_t *data);
void get_extra_split_data_s2m_user(uint8_t *data);
void handle_extra_split_data_s2m_user(uint8_t *data);
