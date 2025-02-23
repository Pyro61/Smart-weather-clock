#include "time.h"
#include "../events/events.h"


/* Hw functions holder */
static struct time_interface *time_hw_funs;


/* 1s elapsed callback */
static void second_elapsed_cb(void)
{
    events_notify_subscribers(EVENT_1S_ELAPSED);
}


/* API functions */
void time_init(const struct time_interface *time_interface)
{
    time_hw_funs = time_interface;
    time_hw_funs -> time_start_1s(second_elapsed_cb);
}


void time_set(const struct time time)
{
    time_hw_funs -> time_set(time);
}


struct time time_get(void)
{
    return time_hw_funs -> time_get();
}