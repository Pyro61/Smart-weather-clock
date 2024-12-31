#pragma once

#include "sys_comm_types.h"


typedef void (*cb_t)(void);

typedef void (*init_t)(void);
typedef void (*send_t)(buf_t buf, req_resp_length_t len);
typedef void (*recv_t)(buf_t buf, req_resp_length_t len, cb_t cb);

struct sys_comm_hw_funs
{
    init_t init;
    send_t send;
    recv_t recv;
};