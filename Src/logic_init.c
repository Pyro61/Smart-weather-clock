#include "logic_init.h"

#include "weather_control/weather_sensors_control.h"
//#include "hw/external_hw/BME280/BME280.h"
//#include "hw/external_hw/YL83/YL83.h"
//#include "hw/mpu_hw/tim/tim.h"
//#include "hw/external_hw/HC05/HC05.h"
#include "sys_comm/adapter_sys_comm.h"
#include "weather_meas_state_machine/state_machine.h"


/* Depends on implementation */
void logic_init(void)
{
    //weather_sensors_get_data_init(BME280_temp_get, BME280_pressure_get, BME280_humidity_get, YL83_rain_state_get);
    //weather_sensors_measure_control_init(tim_periodic_start, tim_periodic_stop, tim_periodic_generate_update);
    //adapter_sys_comm_init(HC05_get_funs());
    //state_machine_init();
}
