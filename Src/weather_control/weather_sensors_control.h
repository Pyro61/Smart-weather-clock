#pragma once

#include "weather_sensors_interface.h"


void weather_sensors_get_data_init(get_temp_t t_fun, get_pressure_t p_fun, get_humidity_t h_fun, is_raining_t r_fun);
void weather_sensors_measure_control_init(start_measure_t start_fun, stop_measure_t stop_fun, force_measure_t force_fun);
struct weather_sensors_get_data *weather_sensors_get_data_get(void);
struct weather_sensors_measure_control *weather_sensors_measure_control_get(void);
