#pragma once

#include "../types.h"
#include <stdbool.h>


typedef void (*alarm_init_t)(void);
typedef bool (*is_alarm_set_t)(void);
typedef void (*alarm_set_t)(struct time time, cb_t cb);
typedef void (*alarm_start_t)(void);
typedef void (*alarm_stop_t)(void);


struct alarm_interface
{
    alarm_init_t alarm_init;
    is_alarm_set_t is_alarm_set;
    alarm_set_t alarm_set;
    alarm_start_t alarm_start;
    alarm_stop_t alarm_stop;
};