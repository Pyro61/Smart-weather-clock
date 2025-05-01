#pragma once

#include "../display/display_interface.h"

enum state_status
{
    /* No need to change state */
    STATE_UNCHANGED = -1,
    /* Must change currect state */
    STATE_CHANGE = 0,
    /* States list */
    MODE_SELECTION = 0,
    WEATHER_IN,
    WEATHER_OUT,
    SET_TIME,
    SET_ALARM,
    ALARM,
    STATE_MAX
};

/* Init function pointer typedef */
typedef void (*main_state_init_t)(const struct display_interface *display_funs);

/* Entry to and exit from state function pointer typedefs */
typedef void (*main_state_entry_t)(enum state_status last_state);
typedef void (*main_state_exit_t)(void);

/* Typedefs for state reactions to events */
typedef enum state_status (*main_state_button_ok_mode_t)(void);
typedef enum state_status (*main_state_button_right_t)(void);
typedef enum state_status (*main_state_button_left_t)(void);
typedef enum state_status (*main_state_button_up_t)(void);
typedef enum state_status (*main_state_button_down_t)(void);
typedef enum state_status (*main_state_refresh_t)(void);
typedef enum state_status (*main_state_alarm_t)(void);


/* State reactions list */
struct main_state_interface
{
    main_state_init_t init;
    main_state_entry_t on_entry;
    main_state_exit_t on_exit;
    main_state_button_ok_mode_t on_ok_mode_button_pressed;
    main_state_button_right_t on_right_button_pressed;
    main_state_button_left_t on_left_button_pressed;
    main_state_button_up_t on_up_button_pressed;
    main_state_button_down_t on_down_button_pressed;
    main_state_refresh_t on_refresh;
    main_state_alarm_t on_alarm;
};