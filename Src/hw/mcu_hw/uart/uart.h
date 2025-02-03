#pragma once

#include <stdint.h>

typedef void(*cb_t)(void);

/********************************************************************************************/
/*                                          UART4                                           */
/********************************************************************************************/
void uart4_init(void);
void uart4_send(uint8_t *buf, uint8_t len);
void uart4_recv(uint8_t *buf, uint8_t len, cb_t cb);
