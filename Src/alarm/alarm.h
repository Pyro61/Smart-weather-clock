#pragma once

#include "alarm_interface.h"
#include <stdbool.h>


/* API functions */
void alarm_init(struct alarm_interface *alarm_interface);
bool is_alarm_set(void);
void alarm_set(struct time time);
void alarm_start(void);
void alarm_stop(void);

