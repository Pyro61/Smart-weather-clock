#pragma once

#include "../../../sys_comm/sys_comm_hw_interface.h"
#include <stdint.h>


void HC05_init(void);
void HC05_send(uint8_t *buf, uint8_t len);
void HC05_recv(uint8_t *buf, uint8_t len, cb_t cb);
struct sys_comm_hw_funs *HC05_get_funs(void);