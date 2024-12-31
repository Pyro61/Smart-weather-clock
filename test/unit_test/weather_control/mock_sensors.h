#pragma once

#include "types.h"

temp_Cdeg_t temp_get(void);
pressure_hPa_t pressure_get(void);
humidity_percent_t humidity_get(void);
enum rain_state rain_state_get(void);

void mock_set_temp(temp_Cdeg_t value);
void mock_set_pressure(pressure_hPa_t value);
void mock_set_humidity(humidity_percent_t value);
void mock_set_rain_state(enum rain_state state);

void mock_init_sensors(void);