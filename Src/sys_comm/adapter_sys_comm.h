#pragma once

#include "../types.h"
#include "sys_comm_hw_interface.h"
#include <stdint.h>


void adapter_sys_comm_init(struct sys_comm_hw_funs *funs);
void adapter_sys_comm_send_resp_data(struct weather_data *data);
void adapter_sys_comm_send_req_start_meas(void);
void adapter_sys_comm_send_req_stop_meas(void);
void adapter_sys_comm_send_req_get_data(void);
void adapter_sys_comm_copy_data(uint8_t *dest_buf);
