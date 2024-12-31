#include "state_sleep.h"
#include "../../app_control/control.h"

/* Control module functions */
typedef const struct app_control * (*app_control_funs_t)(void);
static const app_control_funs_t app_control_funs = app_control_get_control;


static void sleep_on_entry(void)
{
    app_control_funs() -> sleep();
}


static void sleep_on_get(void)
{
    /* Don't get data in sleep mode, it's unactual */
    /* In normal work, this function should never be executed */
}


static void sleep_on_send(void)
{
    /* Don't send data in sleep mode, it's unactual */
    /* In normal work, this function should never be executed */
}


static void sleep_on_exit(void)
{

}


static const struct state_interface state_sleep = 
{
    sleep_on_entry,
    sleep_on_get,
    sleep_on_send,
    sleep_on_exit
};


const struct state_interface *state_sleep_get(void)
{
    return &state_sleep;
}