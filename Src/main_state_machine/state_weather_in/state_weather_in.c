#include "state_weather_in.h"
#include "../../display/display_interface.h"
#include "../../safe_state/safe_state.h"
#include "../../sys_comm/adapter_sys_comm.h"
#include "../../app_control/control.h"
#include "../../events/events.h"
#include "../../alarm/alarm.h"
#include "../../time/time.h"
#include "../../types.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


/* Buffer and signs info */
#define ERROR_SIGN                      '-'
#define BLINKING_SIGN                   ':'
#define BLINKING_SIGN_QUANTITY          2
#define ALARM_SIGN                      'A'
#define BLANK_SIGN                      ' '
#define BUFFER_MAX_SIZE                 80

/* Positions in buffer */
#define BUF_POSITION_HOUR_TENS          0
#define BUF_POSITION_HOUR_UNITS         1
#define BUF_POSITION_MINUTE_TENS        3
#define BUF_POSITION_MINUTE_UNITS       4
#define BUF_POSITION_SECOND_TENS        6
#define BUF_POSITION_SECOND_UNITS       7
#define BUF_POSITION_ALARM_SIGN         11
#define BUF_POSITION_TEMP_MINUS         14
#define BUF_POSITION_TEMP_TENS          15
#define BUF_POSITION_TEMP_UNITS         16
#define BUF_POSITION_HUM_HUNDREDS       26
#define BUF_POSITION_HUM_TENS           27
#define BUF_POSITION_HUM_UNITS          28
#define BUF_POSITION_PRES_THOUSANDS     32 
#define BUF_POSITION_PRES_HUNDREDS      33
#define BUF_POSITION_PRES_TENS          34
#define BUF_POSITION_PRES_UNITS         35
#define BUF_POSITION_RAIN_NO_N          44
#define BUF_POSITION_RAIN_NO_O          45
#define BUF_POSITION_RAIN_R             47
#define BUF_POSITION_RAIN_A             48
#define BUF_POSITION_RAIN_I             49
#define BUF_POSITION_RAIN_N             50
static const uint8_t blinking_sign_buf_position[BLINKING_SIGN_QUANTITY] = {2, 5};

/* Display window template */
static const char *weather_out_window_template = 
"hh:mm:ss   A\n\
mtt *C      hum %\n\
pres hPa    no rain\n\
";


/* Display buffer */
static char buf[BUFFER_MAX_SIZE];

/* Display functions holder */
static struct display_interface display;

/* Weather measurement functions */
typedef const struct app_control *(*weather_functions_t)(void);
static const weather_functions_t weather_functions = app_control_get_control;

/* Weather data holder */
static struct weather_data weather_data;

/* Time data holder */
static struct time time;


/* Helper functions declarations */
static void modify_buffer_1_char(char *buf, uint8_t position, char c);
static void write_temp_to_buf(char *buf, temp_Cdeg_t t);
static void write_humidity_to_buf(char *buf, humidity_percent_t h);
static void write_pressure_to_buf(char *buf, pressure_hPa_t p);
static void write_rain_state_to_buf(char *buf, enum rain_state r);
static void check_alarm(char *buf);
static void get_time(void);
static void write_time_to_buf(char *buf, struct time time);
static void prepare_buf(struct weather_data data, struct time time);
static void blink_signs(char *buf);
static void start_measuring_weather(void);
static void stop_measuring_weather(void);
static void read_weather_data(void);


/* State functions */
static void weather_in_init(struct display_interface *funs)
{
    /* Check if display functions pointers are not NULL pointers */
    if ((funs -> init == NULL) || (funs -> print == NULL) || (funs -> clear == NULL)) safe_state();

    /* Save display functions to holder */
    display.init = funs -> init;
    display.print = funs -> print;
    display.clear = funs -> clear;
}


static void weather_in_on_entry(enum state_status last_state)
{
    (void)last_state; /* Unused*/
    start_measuring_weather();
    read_weather_data();
    get_time();
    strncpy(buf, weather_out_window_template, strlen(weather_out_window_template));
    prepare_buf(weather_data, time);
    display.print(buf);
}


static void weather_in_on_exit(void)
{
    stop_measuring_weather();
    display.clear();
}


static enum state_status weather_in_on_ok_mode_button_pressed(void)
{
    return MODE_SELECTION;
}


static enum state_status weather_in_on_right_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status weather_in_on_left_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status weather_in_on_up_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status weather_in_on_down_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status weather_in_on_refresh(void)
{
    blink_signs(buf);
    read_weather_data();
    get_time();
    prepare_buf(weather_data, time);
    display.print(buf);

    return STATE_UNCHANGED;
}


static enum state_status weather_in_on_alarm(void)
{
    return ALARM;
}


static const struct main_state_interface state_weather_in = 
{
    weather_in_init,
    weather_in_on_entry,
    weather_in_on_exit,
    weather_in_on_ok_mode_button_pressed,
    weather_in_on_right_button_pressed,
    weather_in_on_left_button_pressed,
    weather_in_on_up_button_pressed,
    weather_in_on_down_button_pressed,
    weather_in_on_refresh,
    weather_in_on_alarm,
};


const struct main_state_interface *main_state_weather_in_get(void)
{
    return &state_weather_in;
}


/* Helper functions definitions */
static void modify_buffer_1_char(char *buf, uint8_t position, char c)
{
    if (buf != NULL)
    {
        buf[position] = c;
    }
}


static void write_temp_to_buf(char *buf, temp_Cdeg_t t)
{   
    /* That function assumes that absolute value of temperature is lower than 100 *C */
    if (abs(t) >= 100) return;

    /* Minus sign */
    if (t < 0) modify_buffer_1_char(buf, BUF_POSITION_TEMP_MINUS, '-');
    /* Get absolute value */
    uint32_t tmp = abs(t);
    /* Write mumber of tens */
    modify_buffer_1_char(buf, BUF_POSITION_TEMP_TENS, (tmp / 10) + '0');
    /* Write number of units */
    tmp = tmp - (tmp / 10) * 10;
    modify_buffer_1_char(buf, BUF_POSITION_TEMP_UNITS, tmp + '0');
}


static void write_humidity_to_buf(char *buf, humidity_percent_t h)
{
    /* Humidity is equal to 100% */
    if (h >= 100)
    {
        modify_buffer_1_char(buf, BUF_POSITION_HUM_HUNDREDS, '1');
        modify_buffer_1_char(buf, BUF_POSITION_HUM_TENS, '0');
        modify_buffer_1_char(buf, BUF_POSITION_HUM_UNITS, '0');
    }
    
    /* Humidity is lower than 100% */
    else
    {
        /* Write blank sign in place of number of hundreds */
        modify_buffer_1_char(buf, BUF_POSITION_HUM_HUNDREDS, BLANK_SIGN);
        /* Write mumber of tens */
        uint32_t tmp = h;
        modify_buffer_1_char(buf, BUF_POSITION_HUM_TENS, (tmp / 10) + '0');
        /* Write number of units */
        tmp = tmp - (tmp / 10) * 10;
        modify_buffer_1_char(buf, BUF_POSITION_HUM_UNITS, tmp + '0');
    }
}


static void write_pressure_to_buf(char *buf, pressure_hPa_t p)
{
    /* That function assumes that pressure vakue is lower than 10000 hPa */
    if (abs(p) >= 10000) return;
    /* Write number of thounsands if necessary */
    uint32_t p1000 = p / 1000;
    if (p1000 > 0) modify_buffer_1_char(buf, BUF_POSITION_PRES_THOUSANDS, p1000 + '0');
    else modify_buffer_1_char(buf, BUF_POSITION_PRES_THOUSANDS, BLANK_SIGN);
    /* Write number of hundreds */
    uint32_t p100 = (p - (p1000 * 1000)) / 100;
    modify_buffer_1_char(buf, BUF_POSITION_PRES_HUNDREDS, p100 + '0');
    /* Write number of tens */
    uint32_t p10 = (p - (p1000 * 1000) - (p100 * 100)) / 10;
    modify_buffer_1_char(buf, BUF_POSITION_PRES_TENS, p10 + '0');
    /* Write number of units */
    uint32_t p1 = (p - (p1000 * 1000) - (p100 * 100) - (p10 * 10));
    modify_buffer_1_char(buf, BUF_POSITION_PRES_UNITS, p1 + '0');
}


static void write_rain_state_to_buf(char *buf, enum rain_state r)
{
    if (r == RAINING)
    {
        /* "   RAIN" */
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_NO_N, BLANK_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_NO_O, BLANK_SIGN);
    }

    else if (r == SENSOR_NOT_APPLIED)
    {
        /* "NO DATA" */
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_NO_N, BLANK_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_NO_O, BLANK_SIGN);

        modify_buffer_1_char(buf, BUF_POSITION_RAIN_R, 'D');
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_A, 'A');
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_I, 'T');
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_N, 'A');
    }

    else if (r == NOT_RAINING)
    {
        /* "NO RAIN" */
        /* already written in template, do nothing */
    }

    else /* Not possible to enter here under normal conditions */
    {
        /* "   ----" */
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_NO_N, BLANK_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_NO_O, BLANK_SIGN);

        modify_buffer_1_char(buf, BUF_POSITION_RAIN_R, ERROR_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_A, ERROR_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_I, ERROR_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_RAIN_N, ERROR_SIGN);
    }
}


static void check_alarm(char *buf)
{
    char tmp;
    if (is_alarm_set() == true) tmp = ALARM_SIGN;
    else tmp = BLANK_SIGN;
    modify_buffer_1_char(buf, BUF_POSITION_ALARM_SIGN, tmp);
}


static void get_time(void)
{
    time = time_get();
}


static void write_time_to_buf(char *buf, struct time time)
{
    /* Check if time is correct */
    if (time.hours < 24 && time.minutes < 60 && time.seconds < 60)
    {
        /* Hour tens */
        uint8_t tmp = time.hours / 10;
        modify_buffer_1_char(buf, BUF_POSITION_HOUR_TENS, tmp + '0');
        /* Hour units */
        tmp = time.hours % 10;
        modify_buffer_1_char(buf, BUF_POSITION_HOUR_UNITS, tmp + '0');
        /* Minute tens */
        tmp = time.minutes / 10;
        modify_buffer_1_char(buf, BUF_POSITION_HOUR_UNITS, tmp + '0');
        /* Minute units */
        tmp = time.minutes % 10;
        modify_buffer_1_char(buf, BUF_POSITION_HOUR_UNITS, tmp + '0');
        /* Second tens */
        tmp = time.seconds / 10;
        modify_buffer_1_char(buf, BUF_POSITION_HOUR_UNITS, tmp + '0');
        /* Second units */
        tmp = time.seconds % 10;
        modify_buffer_1_char(buf, BUF_POSITION_HOUR_UNITS, tmp + '0');
    }

    else
    {
        modify_buffer_1_char(buf, BUF_POSITION_HOUR_TENS, ERROR_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_HOUR_UNITS, ERROR_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_MINUTE_TENS, ERROR_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_MINUTE_UNITS, ERROR_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_SECOND_TENS, ERROR_SIGN);
        modify_buffer_1_char(buf, BUF_POSITION_SECOND_UNITS, ERROR_SIGN);
    }
    
}


static void prepare_buf(struct weather_data data, struct time time)
{   
    write_temp_to_buf(buf, data.temp);
    write_humidity_to_buf(buf, data.humidity);
    write_pressure_to_buf(buf, data.pressure);
    write_rain_state_to_buf(buf, data.is_raining);
    check_alarm(buf);
    write_time_to_buf(buf, time);
}


static void blink_signs(char *buf)
{
    uint8_t i;
    for (i = 0; i < BLINKING_SIGN_QUANTITY; i++)
    {
        char tmp;
        uint8_t position = blinking_sign_buf_position[i];
        if (buf[position] == BLINKING_SIGN) tmp = BLANK_SIGN;
        else if (buf[position] == BLANK_SIGN) tmp = BLINKING_SIGN;
        else {}; /* If everything is alright, this case will never execute */
        modify_buffer_1_char(buf, position, tmp);
    }
}


static void start_measuring_weather(void)
{
    weather_functions() -> run();
    weather_functions() -> force_meas();
}


static void stop_measuring_weather(void)
{
    weather_functions() -> sleep();
}


static void read_weather_data(void)
{
    weather_data = weather_functions() -> get_data();
}