#pragma once

#include "../../../time/time_interface.h"
#include "../../../alarm/alarm_interface.h"


/* API functions */
void rtc_init(void);
const struct time_interface *rtc_get_time_funs(void);
const struct alarm_time_interface *rtc_get_alarm_time_funs(void);
