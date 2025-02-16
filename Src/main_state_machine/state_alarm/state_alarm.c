#include "state_alarm.h"
#include "../../display/display_interface.h"
#include "../../safe_state/safe_state.h"
#include "../../time/time.h"
#include "../../alarm/alarm.h"
#include "../../types.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/* Buffer and signs defines */
#define BUFFER_MAX_SIZE            80
#define BLINK_ELEMENTS             5
#define BLANK_SIGN                 ' '
#define ERROR_SIGN                 '-'

/* Alarm time parts */
#define BUF_POSITION_HOUR_TENS          22
#define BUF_POSITION_HOUR_UNITS         23
#define BUF_POSITION_MINUTE_TENS        25
#define BUF_POSITION_MINUTE_UNITS       26
#define BUF_POSITION_SECOND_TENS        28
#define BUF_POSITION_SECOND_UNITS       29

/* Display window template */
static const char alarm_window_template[BUFFER_MAX_SIZE] = 
"/n\
        ALARM\n\
      hh:mm:ss";


/* Alarm blink buffer positions and chars */
static const uint8_t alarm_blink_buf_positions[BLINK_ELEMENTS] = {10, 11, 12, 13, 14};
static const char alarm_string_chars[BLINK_ELEMENTS] = {'A', 'L', 'A', 'R', 'M'};

/* Display buffer */
static char buf[BUFFER_MAX_SIZE];

/* Display functions holder */
static struct display_interface display;

/* Alarm time holder */
static struct time alarm_time;

/* Last state holder */
static enum state_status return_state;


/* Helper functions declarations */
static void modify_buffer_1_char(char *buf, uint8_t position, char c);
static void prepare_buf(struct time alarm_time);
static void write_time_to_buf(char *buf, struct time time);
static void blink_alarm_string(void);


/* State functions */
static void alarm_init(struct display_interface *funs)
{
    /* Check if display functions pointers are not NULL pointers */
    if ((funs -> init == NULL) || (funs -> print == NULL) || (funs -> clear == NULL)) safe_state();

    /* Save display functions to holder */
    display.init = funs -> init;
    display.print = funs -> print;
    display.clear = funs -> clear;
}


static void alarm_on_entry(enum state_status last_state)
{
    return_state = last_state;
    alarm_time = time_get();
    alarm_start();
    prepare_buf(alarm_time);
    display.print(buf);
}


static void alarm_on_exit(void)
{
    alarm_stop();
    display.clear();
}


static enum state_status alarm_on_ok_mode_button_pressed(void)
{
    return return_state;
}


static enum state_status alarm_on_right_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status alarm_on_left_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status alarm_on_up_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status alarm_on_down_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status alarm_on_refresh(void)
{
    blink_alarm_string();
    display.print(buf);

    return STATE_UNCHANGED;
}


static enum state_status alarm_on_alarm(void)
{
    return STATE_UNCHANGED;
}


static const struct main_state_interface state_alarm = 
{
    alarm_init,
    alarm_on_entry,
    alarm_on_exit,
    alarm_on_ok_mode_button_pressed,
    alarm_on_right_button_pressed,
    alarm_on_left_button_pressed,
    alarm_on_up_button_pressed,
    alarm_on_down_button_pressed,
    alarm_on_refresh,
    alarm_on_alarm,
};


const struct main_state_interface *main_state_alarm_get(void)
{
    return &state_alarm;
}


/* Helper functions definitions */
static void modify_buffer_1_char(char *buf, uint8_t position, char c)
{
    if (buf != NULL)
    {
        buf[position] = c;
    }
}


static void prepare_buf(struct time alarm_time)
{
    strncpy(buf, alarm_window_template, BUFFER_MAX_SIZE);
    write_time_to_buf(buf, alarm_time);
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


static void blink_alarm_string(void)
{
    uint8_t i;
    for (i = 0; i < BLINK_ELEMENTS; i++)
    {
        if (buf[alarm_blink_buf_positions[i]] == BLANK_SIGN)
        {
            buf[alarm_blink_buf_positions[i]] = alarm_string_chars[i];
        }

        else if (buf[alarm_blink_buf_positions[i]] == alarm_string_chars[i])
        {
            buf[alarm_blink_buf_positions[i]] = BLANK_SIGN;
        } 
    }
}
