#include "control.h"
#include "../weather_control/weather_sensors_control.h"
#include "../../Src/sys_comm/adapter_sys_comm.h"

typedef struct weather_sensors_get_data * (*data_funs_t)(void);
typedef struct weather_sensors_measure_control * (*measure_control_funs_t)(void);

static const data_funs_t data_funs = weather_sensors_get_data_get;
static const measure_control_funs_t measure_funs = weather_sensors_measure_control_get;


static void start_continuous_measurement(void)
{
    measure_funs() -> start();
}


static void stop_continuous_measurement(void)
{
    measure_funs() -> stop();
}


static void force_measurement(void)
{
    measure_funs() -> force();
}


static struct weather_data get_data(void)
{
    struct weather_data data;
    
    data.temp = data_funs() -> get_temp();
    data.pressure = data_funs() -> get_pressure();
    data.humidity = data_funs() -> get_humidity();
    data.is_raining = data_funs() -> is_raining();

    return data;
}


static void send_data(struct weather_data *data)
{
    adapter_sys_comm_send_resp_data(data);
}


static const struct app_control app_control = 
{
    start_continuous_measurement,
    stop_continuous_measurement,
    force_measurement,
    get_data,
    send_data,
};


const struct app_control *app_control_get_control(void)
{
    return &app_control;
}