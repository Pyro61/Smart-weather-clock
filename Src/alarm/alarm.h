#pragma once

#include "alarm_interface.h"
#include <stdbool.h>


/* API functions */
void alarm_init(const struct alarm_time_interface *alarm_time_interface);
bool is_alarm_set(void);
void alarm_set(const struct time time);
void alarm_start(void);
void alarm_stop(void);

