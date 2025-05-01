#pragma once

#include "sys_comm_types.h"


void sys_comm_send_write_length(buf_t buf, req_resp_length_t len);
void sys_comm_send_write_slave_id(buf_t buf, slave_id_t id);
enum sys_comm_recv_err sys_comm_recv(buf_t buf);