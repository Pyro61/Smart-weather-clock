#include "mock_get_data.h"

void mock_get_data(struct weather_data *buf, temp_Cdeg_t t, pressure_hPa_t p, humidity_percent_t h, enum rain_state r)
{
    buf->temp = t;
    buf->pressure = p;
    buf->humidity = h;
    buf->is_raining = r;
}