#include "buttons.h"
#include "../../mcu_hw/gpio/gpio.h"
#include "../../../safe_state/safe_state.h"
#include "../../../events/events.h"


/* GPIOs */
#define BUTTON_UP_GPIO          PORT_A, 0
#define BUTTON_DOWN_GPIO        PORT_A, 1
#define BUTTON_RIGHT_GPIO       PORT_A, 4
#define BUTTON_LEFT_GPIO        PORT_C, 2
#define BUTTON_OK_MODE_GPIO     PORT_C, 3


/* Helper function declarations */
static void button_up_pressed_handler(void);
static void button_down_pressed_handler(void);
static void button_right_pressed_handler(void);
static void button_left_pressed_handler(void);
static void button_ok_mode_pressed_handler(void);


/* API functions */
void buttons_init(void)
{
    /* Configure GPIO and EXTI */
    /* Up button */
    if (gpio_config(BUTTON_UP_GPIO, INPUT, PUSH_PULL, PULL_UP, SPEED_LOW) == ERR) safe_state();
    if (gpio_set_exti(BUTTON_UP_GPIO, FALLING_EDGE, button_up_pressed_handler) == ERR) safe_state();

    /* Down button */
    if (gpio_config(BUTTON_DOWN_GPIO, INPUT, PUSH_PULL, PULL_UP, SPEED_LOW) == ERR) safe_state();
    if (gpio_set_exti(BUTTON_DOWN_GPIO, FALLING_EDGE, button_down_pressed_handler) == ERR) safe_state();

    /* Right button */
    if (gpio_config(BUTTON_RIGHT_GPIO, INPUT, PUSH_PULL, PULL_UP, SPEED_LOW) == ERR) safe_state();
    if (gpio_set_exti(BUTTON_RIGHT_GPIO, FALLING_EDGE, button_right_pressed_handler) == ERR) safe_state();

    /* Left button */
    if (gpio_config(BUTTON_LEFT_GPIO, INPUT, PUSH_PULL, PULL_UP, SPEED_LOW) == ERR) safe_state();
    if (gpio_set_exti(BUTTON_LEFT_GPIO, FALLING_EDGE, button_left_pressed_handler) == ERR) safe_state();

    /* OK / Mode button */
    if (gpio_config(BUTTON_OK_MODE_GPIO, INPUT, PUSH_PULL, PULL_UP, SPEED_LOW) == ERR) safe_state();
    if (gpio_set_exti(BUTTON_OK_MODE_GPIO, FALLING_EDGE, button_ok_mode_pressed_handler) == ERR) safe_state();
}


/* Helper functions */
static void button_up_pressed_handler(void)
{
    events_notify_subscribers(EVENT_BUTTON_UP_PRESSED);
}


static void button_down_pressed_handler(void)
{
    events_notify_subscribers(EVENT_BUTTON_DOWN_PRESSED);
}


static void button_right_pressed_handler(void)
{
    events_notify_subscribers(EVENT_BUTTON_RIGHT_PRESSED);
}


static void button_left_pressed_handler(void)
{
    events_notify_subscribers(EVENT_BUTTON_LEFT_PRESSED);
}


static void button_ok_mode_pressed_handler(void)
{
    events_notify_subscribers(EVENT_BUTTON_OK_MODE_PRESSED);
}