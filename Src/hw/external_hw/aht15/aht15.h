#pragma once

#include "stdint.h"


void aht15_start_measuring(void);
void aht15_init(void);
int32_t aht15_get_humidity(void);
int32_t aht15_get_temp(void);
void aht15_read_data(void);
