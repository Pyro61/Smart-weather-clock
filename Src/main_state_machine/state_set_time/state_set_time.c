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
#define SET_TIME_HOUR_TENS              0
#define SET_TIME_HOUR_UNITS             1
#define SET_TIME_MINUTE_TENS            2
#define SET_TIME_MINUTE_UNITS           3
#define SET_TIME_SECOND_TENS            4
#define SET_TIME_SECOND_UNITS           5
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
static uint8_t edited_time_part;

/* Last state holder */
static enum state_status return_state;

/* Time data holder */
static struct time time;


/* Helper functions declarations */
static void modify_buffer_1_char(char *buf, uint8_t position, char c);
static void prepare_buf(char *buf, struct time time);
static bool is_time_part_value_correct(uint8_t edited_time_part, uint32_t value);
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
    /* Save last state */
    return_state = last_state;
    /* Reset edited time part */
    edited_time_part = SET_TIME_HOUR_TENS;
    /* Reset time */
    time.hours = 0;
    time.minutes = 0;
    time.seconds = 0;
    /* Copy initial display output to buffer */
    strncpy(buf, set_time_window_template, BUFFER_MAX_SIZE);
    /* Print display output */
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
        prepare_buf(buf, time);
        display.print(buf);
    }

    return STATE_UNCHANGED;
}


static enum state_status set_time_on_left_button_pressed(void)
{
    if (check_time_set_position(edited_time_part - 1) == true)
    {
        edited_time_part--;
        prepare_buf(buf, time);
        display.print(buf);
    }

    return STATE_UNCHANGED;
}


static enum state_status set_time_on_up_button_pressed(void)
{
    uint8_t tmp = get_time_part_value(edited_time_part);
    if (is_time_part_value_correct(edited_time_part, tmp + 1) == true)
    {
        save_time_part_value(edited_time_part, tmp + 1);
        prepare_buf(buf, time);
        display.print(buf);
    }
    return STATE_UNCHANGED;
}


static enum state_status set_time_on_down_button_pressed(void)
{
    uint8_t tmp = get_time_part_value(edited_time_part);
    if (is_time_part_value_correct(edited_time_part, tmp - 1) == true)
    {
        save_time_part_value(edited_time_part, tmp - 1);
        prepare_buf(buf, time);
        display.print(buf);
    }
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
    modify_buffer_1_char(buf, set_time_parts_buf_positions[SET_TIME_HOUR_TENS], (time.hours / 10) + '0');
    modify_buffer_1_char(buf, set_time_parts_buf_positions[SET_TIME_HOUR_UNITS], (time.hours % 10) + '0');
    /* Minutes */
    modify_buffer_1_char(buf, set_time_parts_buf_positions[SET_TIME_MINUTE_TENS], (time.minutes / 10) + '0');
    modify_buffer_1_char(buf, set_time_parts_buf_positions[SET_TIME_MINUTE_UNITS], (time.minutes % 10) + '0');
    /* Seconds */
    modify_buffer_1_char(buf, set_time_parts_buf_positions[SET_TIME_SECOND_TENS], (time.seconds / 10) + '0');
    modify_buffer_1_char(buf, set_time_parts_buf_positions[SET_TIME_SECOND_UNITS], (time.seconds % 10) + '0');
}


static bool is_time_part_value_correct(uint8_t time_part, uint32_t value)
{
    switch (time_part)
    {
        case SET_TIME_HOUR_TENS:
        {
            /* 2-:--:-- */
            if (value > 2) return false;
            else return true;
        }
        case SET_TIME_HOUR_UNITS:
        {
            /* 23:--:-- */
            if (get_time_part_value(SET_TIME_HOUR_TENS) == 2)
            {
                if (value > 3) return false;
                else return true;
            }

            /* -9:--:-- */
            else
            {
                if (value > 9) return false;
                else return true;
            }
        }
        case SET_TIME_MINUTE_TENS:
        case SET_TIME_SECOND_TENS:
        {
            /* --:5-:5- */
            if (value > 5) return false;
            else return true;
        }
        case SET_TIME_MINUTE_UNITS:
        case SET_TIME_SECOND_UNITS:
        {
            /* --:-9:-9 */
            if (value > 9) return false;
            else return true;
        }
        default:
        {
            return false;
        }   
    }
}


static void save_time_part_value(uint8_t time_part, uint32_t value)
{
    switch (time_part)
    {
        case SET_TIME_HOUR_TENS:
        {
            time.hours = get_time_part_value(SET_TIME_HOUR_UNITS) + value * 10;
            break;
        }
        case SET_TIME_HOUR_UNITS:
        {
            time.hours = get_time_part_value(SET_TIME_HOUR_TENS) * 10 + value;
            break;
        }
        case SET_TIME_MINUTE_TENS:
        {
            time.minutes = get_time_part_value(SET_TIME_MINUTE_UNITS) + value * 10;
            break;
        }
        case SET_TIME_MINUTE_UNITS:
        {
            time.minutes = get_time_part_value(SET_TIME_MINUTE_TENS) * 10 + value;
            break;
        }
        case SET_TIME_SECOND_TENS:
        {
            time.seconds = get_time_part_value(SET_TIME_SECOND_UNITS) + value * 10;
            break;
        }
        case SET_TIME_SECOND_UNITS:
        {
            time.seconds = get_time_part_value(SET_TIME_SECOND_TENS) * 10 + value;
            break;
        }
    }
}


static uint32_t get_time_part_value(uint8_t time_part)
{
    switch (time_part)
    {
        case SET_TIME_HOUR_TENS: return (uint32_t)(time.hours / 10);
        case SET_TIME_HOUR_UNITS: return (uint32_t)(time.hours % 10);
        case SET_TIME_MINUTE_TENS: return (uint32_t)(time.minutes / 10);
        case SET_TIME_MINUTE_UNITS: return (uint32_t)(time.minutes % 10);
        case SET_TIME_SECOND_TENS: return (uint32_t)(time.seconds / 10);
        case SET_TIME_SECOND_UNITS: return (uint32_t)(time.seconds % 10);
        default: return (uint32_t)-1;
    }
}


static bool check_time_set_position(uint8_t next_position)
{
    if (next_position < SET_TIME_PARTS)
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
        if (value % 2 == 0) tmp = (value / 10) + '0'; /* Tens */
        else tmp = (value % 10) + '0'; /* Units */
    }

    else {}; /* If everything is alright, this case will never execute */
    modify_buffer_1_char(buf, position, tmp);
}
