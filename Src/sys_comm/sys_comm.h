#pragma once

#include "sys_comm_types.h"


/* Request functions */
req_resp_length_t sys_comm_req_start_meas(buf_t buf);
req_resp_length_t sys_comm_req_stop_meas(buf_t buf);
req_resp_length_t sys_comm_req_get_data(buf_t buf);

/* Response functions */
req_resp_length_t sys_comm_resp_get_data(buf_t buf, buf_t data);