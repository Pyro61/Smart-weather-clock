#pragma once
#include "../types.h"


typedef void (*run_t)(void);
typedef void (*sleep_t)(void);
typedef struct weather_data (*get_data_t)(void);
typedef void (*send_data_t)(struct weather_data *);
typedef void (*force_meas_t)(void);


struct app_control
{
    run_t run;
    sleep_t sleep;
    force_meas_t force_meas;
    get_data_t get_data;
    send_data_t send_data;
};