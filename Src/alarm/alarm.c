#include "alarm.h"
#include "../events/events.h"

/* Hw functions holder */
static struct alarm_time_interface *alarm_time_hw_funs;


/* Alarm callback */
static void alarm_cb(void)
{
    events_notify_subscribers(EVENT_ALARM);
}


/* API functions */
void alarm_init(const struct alarm_time_interface *alarm_time_interface)
{
    alarm_time_hw_funs = alarm_time_interface;
}


bool is_alarm_set(void)
{
    return alarm_time_hw_funs -> is_alarm_set();
}


void alarm_set(const struct time time)
{
    alarm_time_hw_funs -> alarm_set(time, alarm_cb);
}


void alarm_start(void)
{

}


void alarm_stop(void)
{

}