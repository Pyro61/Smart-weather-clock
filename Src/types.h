#pragma once

#include "stdint.h"

typedef int32_t temp_Cdeg_t;
typedef int32_t pressure_hPa_t;
typedef int32_t humidity_percent_t;

enum rain_state
{
	SENSOR_NOT_APPLIED = -1,
	NOT_RAINING = 0,
	RAINING = 1
};

struct weather_data
{
	temp_Cdeg_t temp;
	pressure_hPa_t pressure;
	humidity_percent_t humidity;
	enum rain_state is_raining;
};