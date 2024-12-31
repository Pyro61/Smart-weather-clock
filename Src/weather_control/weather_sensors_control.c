#include "weather_sensors_control.h"


/* Read data from sensors functions */
static struct weather_sensors_get_data get_data;

/* Start, stop and force measurings */
static struct weather_sensors_measure_control measure_control;


/* Read data */
void weather_sensors_get_data_init(get_temp_t t_fun, get_pressure_t p_fun, get_humidity_t h_fun, is_raining_t r_fun)
{
    get_data.get_temp = t_fun;
    get_data.get_pressure = p_fun;
    get_data.get_humidity = h_fun;
    get_data.is_raining = r_fun;
}


struct weather_sensors_get_data *weather_sensors_get_data_get(void)
{
    return &get_data;
}


/* Measure control */
void weather_sensors_measure_control_init(start_measure_t start_fun, stop_measure_t stop_fun, force_measure_t force_fun)
{
    measure_control.start = start_fun;
    measure_control.stop = stop_fun;
    measure_control.force = force_fun;
}


struct weather_sensors_measure_control *weather_sensors_measure_control_get(void)
{
    return &measure_control;
}