#include "state_machine.h"
#include "../events/events.h"
#include "../safe_state/safe_state.h"


/* Currect and last state */
static enum state_status current_state = WEATHER_IN;
static enum state_status last_state = MODE_SELECTION;

/* States functions */
typedef const struct main_state_interface * (*main_state_get_t)(void);

static const main_state_get_t states[STATE_MAX] = 
{
	0 //only for compiling
};


static void state_change_if_needed(enum state_status status)
{
    /* Greater values than STATE_UNCHANGED are corresponding to state change */
    if (status > STATE_UNCHANGED)
    {
        /* Entry and exit functions must never require another change of state */
        states[current_state]() -> on_exit();
        /* Preventing the last state from being assigned as mode selection (Come back directly to last mode before changing bypassing mode selection) */
        if (current_state != MODE_SELECTION)
        {
            last_state = current_state;
        }
        
        current_state = status;
        states[current_state]() -> on_entry(last_state);
    }
    
}

static void state_machine_button_ok_mode_pressed_handle(void)
{
    enum state_status status = states[current_state]() -> on_ok_mode_button_pressed();
    state_change_if_needed(status);
}


static void state_machine_button_right_pressed_handle(void)
{
    enum state_status status = states[current_state]() -> on_right_button_pressed();
    state_change_if_needed(status);
}


static void state_machine_button_left_pressed_handle(void)
{
    enum state_status status = states[current_state]() -> on_left_button_pressed();
    state_change_if_needed(status);
}


static void state_machine_button_up_pressed_handle(void)
{
    enum state_status status = states[current_state]() -> on_up_button_pressed();
    state_change_if_needed(status);
}


static void state_machine_button_down_pressed_handle(void)
{
    enum state_status status = states[current_state]() -> on_down_button_pressed();
    state_change_if_needed(status);
}


static void state_machine_refresh_handle(void)
{
    enum state_status status = states[current_state]() -> on_refresh();
    state_change_if_needed(status);
}


static void state_machine_alarm_handle(void)
{
    enum state_status status = states[current_state]() -> on_alarm();
    state_change_if_needed(status);
}


static void state_machine_init_all_states(struct display_interface *display_funs)
{
    uint8_t i;
    for (i = 0; i < STATE_MAX; i++)
    {
        states[i]() -> init(display_funs); 
    }
}

void main_state_machine_init(struct display_interface *display_funs)
{
    /* Check for correct subscribes, otherwise program won't run correctly */
    if (events_subscribe(state_machine_button_ok_mode_pressed_handle, EVENT_BUTTON_OK_MODE_PRESSED) != SUBSCRIBE_SUCCESS) safe_state();
    if (events_subscribe(state_machine_button_right_pressed_handle, EVENT_BUTTON_RIGHT_PRESSED) != SUBSCRIBE_SUCCESS) safe_state();
    if (events_subscribe(state_machine_button_left_pressed_handle, EVENT_BUTTON_LEFT_PRESSED) != SUBSCRIBE_SUCCESS) safe_state();
    if (events_subscribe(state_machine_button_up_pressed_handle, EVENT_BUTTON_UP_PRESSED) != SUBSCRIBE_SUCCESS) safe_state();
    if (events_subscribe(state_machine_button_down_pressed_handle, EVENT_BUTTON_DOWN_PRESSED) != SUBSCRIBE_SUCCESS) safe_state();
    if (events_subscribe(state_machine_refresh_handle, EVENT_1S_ELAPSED) != SUBSCRIBE_SUCCESS) safe_state();
    if (events_subscribe(state_machine_alarm_handle, EVENT_ALARM) != SUBSCRIBE_SUCCESS) safe_state();

    /* Call all states init functions */
    state_machine_init_all_states(display_funs);

    /* Entry initial state */
    states[current_state]() -> on_entry(last_state);
}

