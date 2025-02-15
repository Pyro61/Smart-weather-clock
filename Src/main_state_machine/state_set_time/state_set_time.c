#include "state_set_time.h"
#include "../../display/display_interface.h"
#include "../../safe_state/safe_state.h"
#include "../../time/time.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/* Buffer and sign defines */
#define BLANK_SIGN                      ' '
#define BUFFER_MAX_SIZE                 80

/* Time parts */
#define SET_TIME_HOUR_TENS              1
#define SET_TIME_HOUR_UNITS             2
#define SET_TIME_MINUTE_TENS            3
#define SET_TIME_MINUTE_UNITS           4
#define SET_TIME_SECOND_TENS            5
#define SET_TIME_SECOND_UNITS           6
#define SET_TIME_PARTS                  6


/* Display window template */
static const char set_time_window_template[BUFFER_MAX_SIZE] = 
"      HH:MM:SS      \n\
      00:00:00      ";

/* Display buffer */
static char buf[BUFFER_MAX_SIZE];

/* Display functions holder */
static struct display_interface display;

/* Time parts buffer positions */
static const uint8_t set_time_parts_buf_positions[SET_TIME_PARTS] = {27, 28, 30, 31, 33, 34};

/* Currently edited time part */
static uint8_t edited_time_part = SET_TIME_HOUR_TENS;

/* Last state holder */
static enum main_state return_state;

/* Time data holder */
static struct time time;


/* Helper functions declarations */
static void modify_buffer_1_char(char *buf, uint8_t position, char c);
static void prepare_buf(char *buf, struct time time);
static void save_time_part_value(uint8_t time_part, uint32_t value);
static uint32_t get_time_part_value(uint8_t time_part);
static bool check_time_set_position(uint8_t next_position);
static void blink_edited_time_part(char *buf, uint8_t edited_time_part);


/* State functions */
static void set_time_init(struct display_interface *funs)
{
    /* Check if display functions pointers are not NULL pointers */
    if ((funs -> init == NULL) || (funs -> print == NULL) || (funs -> clear == NULL)) safe_state();

    /* Save display functions to holder */
    display.init = funs -> init;
    display.print = funs -> print;
    display.clear = funs -> clear;
}


static void set_time_on_entry(enum state_status last_state)
{
    return_state = last_state;
    strncpy(buf, set_time_window_template, BUFFER_MAX_SIZE);
    display.print(buf);
}


static void set_time_on_exit(void)
{
    time_set(time);
    display.clear();
}


static enum state_status set_time_on_ok_mode_button_pressed(void)
{
    return return_state;
}


static enum state_status set_time_on_right_button_pressed(void)
{
    if (check_time_set_position(edited_time_part + 1) == true)
    {
        edited_time_part++;
    }

    return STATE_UNCHANGED;
}


static enum state_status set_time_on_left_button_pressed(void)
{
    if (check_time_set_position(edited_time_part - 1) == true)
    {
        edited_time_part--;
    }

    return STATE_UNCHANGED;
}


static enum state_status set_time_on_up_button_pressed(void)
{
    /* Preventing against writing wrong time part value is inside save_time_part_value function */
    uint8_t tmp;
    if (edited_time_part % 2 == 1) tmp = 10; /* Tens */
    else tmp = 0; /* Units */
    save_time_part_value(edited_time_part, get_time_part_value(edited_time_part) + tmp);
    prepare_buf(buf, time);
    display.print(buf);

    return STATE_UNCHANGED;
}


static enum state_status set_time_on_down_button_pressed(void)
{
    /* Preventing against writing wrong time part value is inside save_time_part_value function */
    uint8_t tmp;
    if (edited_time_part % 2 == 1) tmp = 10; /* Tens */
    else tmp = 0; /* Units */
    save_time_part_value(edited_time_part, get_time_part_value(edited_time_part) - tmp);
    prepare_buf(buf, time);
    display.print(buf);

    return STATE_UNCHANGED;
}


static enum state_status set_time_on_refresh(void)
{
    blink_edited_time_part(buf, edited_time_part);
    display.print(buf);

    return STATE_UNCHANGED;
}


static enum state_status set_time_on_alarm(void)
{
    return ALARM;
}


static const struct main_state_interface state_set_time = 
{
    set_time_init,
    set_time_on_entry,
    set_time_on_exit,
    set_time_on_ok_mode_button_pressed,
    set_time_on_right_button_pressed,
    set_time_on_left_button_pressed,
    set_time_on_up_button_pressed,
    set_time_on_down_button_pressed,
    set_time_on_refresh,
    set_time_on_alarm,
};


const struct main_state_interface *main_state_set_time_get(void)
{
    return &state_set_time;
}


/* Helper functions definitions */
static void modify_buffer_1_char(char *buf, uint8_t position, char c)
{
    if (buf != NULL)
    {
        buf[position] = c;
    }
}


static void prepare_buf(char *buf, struct time time)
{
    strncpy(buf, set_time_window_template, BUFFER_MAX_SIZE);
    /* Hours */
    uint8_t tmp = get_time_part_value(SET_TIME_HOUR_TENS);
    modify_buffer_1_char(buf, SET_TIME_HOUR_TENS, tmp / 10);
    modify_buffer_1_char(buf, SET_TIME_HOUR_UNITS, tmp % 10);
    /* Minutes */
    tmp = get_time_part_value(SET_TIME_MINUTE_TENS);
    modify_buffer_1_char(buf, SET_TIME_MINUTE_TENS, tmp / 10);
    modify_buffer_1_char(buf, SET_TIME_MINUTE_UNITS, tmp % 10);
    /* Seconds */
    tmp = get_time_part_value(SET_TIME_SECOND_TENS);
    modify_buffer_1_char(buf, SET_TIME_SECOND_TENS, tmp / 10);
    modify_buffer_1_char(buf, SET_TIME_SECOND_UNITS, tmp % 10);
}


static void save_time_part_value(uint8_t time_part, uint32_t value)
{
    /* Hours */
    if ((time_part == SET_TIME_HOUR_TENS) || (time_part == SET_TIME_HOUR_UNITS))
    {
        /* Check if new hours value if lesser than 24 (23:59:59 is maximum time value) */
        if (value < 24) time.hours = (hour_t)value;
    }

    /* Minutes */
    else if ((time_part == SET_TIME_MINUTE_TENS) || (time_part == SET_TIME_MINUTE_UNITS))
    {
        /* Check if new minutes value if lesser than 60 (23:59:59 is maximum time value) */
        if (value < 60) time.minutes = (minute_t)value;
    }

    /* Seconds */
    else if ((time_part == SET_TIME_SECOND_TENS) || (time_part == SET_TIME_SECOND_UNITS))
    {
        /* Check if new seconds value if lesser than 60 (23:59:59 is maximum time value) */
        if (value < 60) time.seconds = (second_t)value;
    }

    /* Wrong time part, do nothing */
    else {}
}


static uint32_t get_time_part_value(uint8_t time_part)
{
    /* Hours */
    if ((time_part == SET_TIME_HOUR_TENS) || (time_part == SET_TIME_HOUR_UNITS))
    {
        return (uint32_t)time.hours;
    }

    /* Minutes */
    else if ((time_part == SET_TIME_MINUTE_TENS) || (time_part == SET_TIME_MINUTE_UNITS))
    {
        return (uint32_t)time.minutes;
    }

    /* Seconds */
    else if ((time_part == SET_TIME_SECOND_TENS) || (time_part == SET_TIME_SECOND_UNITS))
    {
        return (uint32_t)time.seconds;
    }

    /* Wrong time part, return wrong value */
    else
    {
        return (uint32_t)-1;
    }
}


static bool check_time_set_position(uint8_t next_position)
{
    if ((next_position > 0) && (next_position <= SET_TIME_PARTS))
    {
        return true;
    }

    else
    {
        return false;
    }
}


static void blink_edited_time_part(char *buf, uint8_t edited_time_part)
{
    char tmp;
    uint8_t position = set_time_parts_buf_positions[edited_time_part];

    /* Showing number, change to blank sign */
    if (buf[position] >= '0' && buf[position] <= '9')
    {
        tmp = BLANK_SIGN;
    }

    /* Showing blank sign, change to number */
    else if (buf[position] == BLANK_SIGN)
    {
        uint32_t value = get_time_part_value(edited_time_part);
        if (value % 2 == 1) tmp = (value / 10) + '0'; /* Tens */
        else tmp = (value % 10) + '0'; /* Units */
    }

    else {}; /* If everything is alright, this case will never execute */
    modify_buffer_1_char(buf, position, tmp);
}
