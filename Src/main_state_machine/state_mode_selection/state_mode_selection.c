#include "state_mode_selection.h"
#include "../../display/display_interface.h"
#include "../../safe_state/safe_state.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define SELECTABLE_MODES_QUANTITY  4
#define SELECTED_SIGN              '>'
#define BUFFER_MAX_SIZE            80

/* Display window template */
static const char *selectable_modes_list = 
"  1. weather indoor\n\
  2. weather outdoor\n\
  3. set time\n\
  4. set alarm";


/* Display buffer */
static char buf[BUFFER_MAX_SIZE];

/* Selected mode holder */
static uint8_t selected_mode = 1;

/* Positions where to put selected sign in buffer */
static const uint8_t selected_mode_sign_buf_position[SELECTABLE_MODES_QUANTITY] = {0, 20, 41, 55};

/* Display functions holder */
struct display_interface display;


/* Helper functions declarations */
static void modify_buffer_1_char(char *buf, uint8_t position, char c);
static void prepare_buf(uint8_t selected_mode);
static bool check_selected_mode_position(uint8_t position);


/* State functions */
static void mode_selection_init(struct display_interface *funs)
{
    /* Check if display functions pointers are not NULL pointers */
    if ((funs -> init == NULL) || (funs -> print == NULL) || (funs -> clear == NULL)) safe_state();

    /* Save display functions to holder */
    display.init = funs -> init;
    display.print = funs -> print;
    display.clear = funs -> clear;
}


static void mode_selection_on_entry(enum state_status last_state)
{
    (void)last_state; /* Unused */
    selected_mode = 1;
    prepare_buf(selected_mode);
    display.print(buf);
}


static void mode_selection_on_exit(void)
{
    display.clear();
}


static enum state_status mode_selection_on_ok_mode_button_pressed(void)
{
    return selected_mode;
}


static enum state_status mode_selection_on_right_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status mode_selection_on_left_button_pressed(void)
{
    return STATE_UNCHANGED;
}


static enum state_status mode_selection_on_up_button_pressed(void)
{
    if (check_selected_mode_position(selected_mode - 1) == true)
    {
        selected_mode--;
        prepare_buf(selected_mode);
        display.print(buf);
    }

    return STATE_UNCHANGED;
}


static enum state_status mode_selection_on_down_button_pressed(void)
{
    if (check_selected_mode_position(selected_mode + 1) == true)
    {
        selected_mode++;
        prepare_buf(selected_mode);
        display.print(buf);
    }

    return STATE_UNCHANGED;
}


static enum state_status mode_selection_on_refresh(void)
{
    char tmp;
    uint8_t position = selected_mode_sign_buf_position[selected_mode - 1];
    if (buf[position] == '>') tmp = ' ';
    else if (buf[position] == ' ') tmp = '>';
    else {}; /* If everything is alright, this case will never execute */
    modify_buffer_1_char(buf, position, tmp);
    display.print(buf);

    return STATE_UNCHANGED;
}


static enum state_status mode_selection_on_alarm(void)
{
    return ALARM;
}


static const struct main_state_interface state_mode_selection = 
{
    mode_selection_init,
    mode_selection_on_entry,
    mode_selection_on_exit,
    mode_selection_on_ok_mode_button_pressed,
    mode_selection_on_right_button_pressed,
    mode_selection_on_left_button_pressed,
    mode_selection_on_up_button_pressed,
    mode_selection_on_down_button_pressed,
    mode_selection_on_refresh,
    mode_selection_on_alarm,
};


const struct main_state_interface *main_state_mode_selection_get(void)
{
    return &state_mode_selection;
}


/* Helper functions definitions */
static void modify_buffer_1_char(char *buf, uint8_t position, char c)
{
    if (buf != NULL)
    {
        buf[position] = c;
    }
}


static void prepare_buf(uint8_t position)
{
    strncpy(buf, selectable_modes_list, strlen(selectable_modes_list));
    modify_buffer_1_char(buf, selected_mode_sign_buf_position[position - 1], SELECTED_SIGN);
}


static bool check_selected_mode_position(uint8_t position)
{
    if ((position > 0) && (position <= SELECTABLE_MODES_QUANTITY))
    {
        return true;
    }

    else
    {
        return false;
    }
}