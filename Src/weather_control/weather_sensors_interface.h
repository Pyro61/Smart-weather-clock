#pragma once

#include "../types.h"


/* Read data */
typedef temp_Cdeg_t(*get_temp_t)(void);
typedef pressure_hPa_t(*get_pressure_t)(void);
typedef humidity_percent_t(*get_humidity_t)(void);
typedef enum rain_state(*is_raining_t)(void);

struct weather_sensors_get_data
{
    get_temp_t get_temp;
    get_pressure_t get_pressure;
    get_humidity_t get_humidity;
    is_raining_t is_raining;
};

/* Measure control */
typedef void (*start_measure_t)(void);
typedef void (*stop_measure_t)(void);
typedef void (*force_measure_t)(void);

struct weather_sensors_measure_control
{
    start_measure_t start;
    stop_measure_t stop;
    force_measure_t force;
};