#include "alarm.h"
#include "../events/events.h"

/* Hw functions holder */
static struct alarm_interface *alarm_hw_funs;


/* Alarm callback */
static void alarm_cb(void)
{
    events_notify_subscribers(EVENT_ALARM);
}


/* API functions */
void alarm_init(struct alarm_interface *alarm_interface)
{
    alarm_hw_funs = alarm_interface;
}


bool is_alarm_set(void)
{
    return alarm_hw_funs -> is_alarm_set();
}


void alarm_set(struct time time)
{
    alarm_hw_funs -> alarm_set(time, alarm_cb);
}


void alarm_start(void)
{
    alarm_hw_funs -> alarm_start();
}


void alarm_stop(void)
{
    alarm_hw_funs -> alarm_stop();
}