#pragma once

#include "../types.h"
#include <stdbool.h>


typedef void (*alarm_init_t)(void);
typedef bool (*is_alarm_set_t)(void);
typedef void (*alarm_set_t)(struct time time, cb_t cb);


struct alarm_time_interface
{
    alarm_init_t alarm_init;
    is_alarm_set_t is_alarm_set;
    alarm_set_t alarm_set;
};