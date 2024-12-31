#include "state_run.h"
#include "../../app_control/control.h"


/* Control module functions */
typedef const struct app_control * (*app_control_funs_t)(void);
static const app_control_funs_t app_control_funs = app_control_get_control;

/* Data variable */
static struct weather_data data;


static void run_on_entry(void)
{
    app_control_funs() -> run();
    app_control_funs() -> force_meas();
}


static void run_on_get(void)
{
    data = app_control_funs() -> get_data();
}


static void run_on_send(void)
{
    app_control_funs() -> send_data(&data);
}


static void run_on_exit(void)
{

}


static const struct state_interface state_run = 
{
    run_on_entry,
    run_on_get,
    run_on_send,
    run_on_exit
};


const struct state_interface *state_run_get(void)
{
    return &state_run;
}