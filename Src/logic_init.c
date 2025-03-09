#include "logic_init.h"

#include "weather_control/weather_sensors_control.h"
#include "hw/external_hw/LPS25HB/LPS25HB.h"
#include "hw/external_hw/aht15/aht15.h"
#include "hw/external_hw/rain_sensor/rain_sensor.h"
#include "hw/mcu_hw/tim/tim.h"

#include "sys_comm/adapter_sys_comm.h"
#include "hw/external_hw/HC05/HC05.h"

#include "alarm/alarm.h"
#include "time/time.h"
#include "hw/mcu_hw/rtc/rtc.h"

#include "main_state_machine/state_machine.h"
#include "hw/external_hw/HD44780/HD44780.h"


/* Depends on implementation */
void logic_init(void)
{
    weather_sensors_get_data_init(LPS25HB_get_temp, LPS25HB_get_pressure, aht15_get_humidity, rain_sensor_ret_no_sensor);
    weather_sensors_measure_control_init(tim_periodic_start, tim_periodic_stop, tim_periodic_generate_update);
    adapter_sys_comm_init(HC05_get_funs());
    alarm_init(rtc_get_alarm_time_funs());
    time_init(rtc_get_time_funs());
    main_state_machine_init(HD44780_get_funs());
}
