#pragma once

#include "common/matrix.h"

void transport_master_init(void);
void transport_slave_init(void);

// returns false if valid data not received from slave
bool transport_master(matrix_row_t matrix[]);
void transport_slave(matrix_row_t matrix[]);


#ifdef EXTRA_SPLIT_DATA_M2S
void get_extra_split_data_m2s_kb(struct EXTRA_SPLIT_DATA_M2S *data);
void get_extra_split_data_m2s_user(struct EXTRA_SPLIT_DATA_M2S *data);
void handle_extra_split_data_m2s_kb(struct EXTRA_SPLIT_DATA_M2S *data);
void handle_extra_split_data_m2s_user(struct EXTRA_SPLIT_DATA_M2S *data);
#endif
#ifdef EXTRA_SPLIT_DATA_S2M
void get_extra_split_data_s2m_kb(struct EXTRA_SPLIT_DATA_S2M *data);
void get_extra_split_data_s2m_user(struct EXTRA_SPLIT_DATA_S2M *data);
void handle_extra_split_data_s2m_kb(struct EXTRA_SPLIT_DATA_S2M *data);
void handle_extra_split_data_s2m_user(struct EXTRA_SPLIT_DATA_S2M *data);
#endif
