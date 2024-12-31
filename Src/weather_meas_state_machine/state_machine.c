#include "state_machine.h"

#include "state_sleep/state_sleep.h"
#include "state_run/state_run.h"
#include "../events/events.h"
#include "../safe_state/safe_state.h"


/* First state after power-up */
static enum state state = SLEEP;

/* States functions */
typedef const struct state_interface * (*state_get_t)(void);

static const state_get_t states[STATE_MAX] = 
{
	state_sleep_get,
	state_run_get,
};


static void state_machine_sleep(void)
{
    /* Assume that current state is RUN */
    states[state]() -> on_exit();
    state = SLEEP;
    states[state]() -> on_entry();
}


static void state_machine_run(void)
{
    /* Assume that current state is SLEEP */
    states[state]() -> on_exit();
    state = RUN;
    states[state]() -> on_entry();
}


static void state_machine_get(void)
{
    states[state]() -> on_get();
    /* There's no event for sending data, it is has to be executed right after on_get() function */
    states[state]() -> on_send();
}


void state_machine_init(void)
{
    /* Check for correct subscribes, otherwise program won't run correctly */
    if (events_subscribe(state_machine_sleep, EVENT_SLEEP) != SUBSCRIBE_SUCCESS)
    {
        safe_state();
    }
    if(events_subscribe(state_machine_run, EVENT_RUN) != SUBSCRIBE_SUCCESS)
    {
        safe_state();
    }
    if(events_subscribe(state_machine_get, EVENT_GET) != SUBSCRIBE_SUCCESS)
    {
        safe_state();
    }

    states[state]() -> on_entry();
}
