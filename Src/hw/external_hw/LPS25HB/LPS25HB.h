#pragma once

#include <stdint.h>


void LPS25HB_start_measuring(void);
void LPS25HB_init(void);
int32_t LPS25HB_get_temp(void);
int32_t LPS25HB_get_pressure(void);