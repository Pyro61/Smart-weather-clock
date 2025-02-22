#pragma once

#include "../types.h"


typedef void (*time_init_t)(void);
typedef void (*time_start_1s_t)(cb_t cb);
typedef void (*time_set_t)(struct time time);
typedef struct time (*time_get_t)(void);

struct time_interface
{
    time_init_t time_init;
    time_start_1s_t time_start_1s;
    time_set_t time_set;
    time_get_t time_get;
};