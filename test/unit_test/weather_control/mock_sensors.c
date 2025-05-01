#include "mock_sensors.h"

static temp_Cdeg_t temp;
static pressure_hPa_t pressure;
static humidity_percent_t humidity;
static enum rain_state is_raining;

temp_Cdeg_t temp_get(void)
{
    return temp;
}

pressure_hPa_t pressure_get(void)
{
    return pressure;
}

humidity_percent_t humidity_get(void)
{
    return humidity;
}

enum rain_state rain_state_get(void)
{
    return is_raining;
}

void mock_set_temp(temp_Cdeg_t value)
{
    temp = value;
}

void mock_set_pressure(pressure_hPa_t value)
{
    pressure = value;
}

void mock_set_humidity(humidity_percent_t value)
{
    humidity = value;
}

void mock_set_rain_state(enum rain_state state)
{
    is_raining = state;
}

void mock_init_sensors(void)
{
    mock_set_temp(0);
    mock_set_pressure(0);
    mock_set_humidity(0);
    mock_set_rain_state(NOT_RAINING);
}

