#pragma once

#include <stdint.h>


void tim_delay_init(void);
void tim_periodic_init(void);

void delay_ms(uint32_t delay);
void tim_periodic_start(void);
void tim_periodic_stop(void);
void tim_periodic_generate_update(void);
