#include "unity/fixture/unity_fixture.h"
#include "assert.h"
#include "../Src/main_state_machine/state.h"
#include "../Src/main_state_machine/state_set_time/state_set_time.h"
#include "display_mock.h"
#include <string.h>


/* Messages showed by display */
#define CLEARED_MESSAGE             "\0"


typedef const struct main_state_interface * (*main_state_get_t)(void);
static const main_state_get_t state_set_time_funs = main_state_set_time_get;
static enum state_status last_state;

static char test_buf[80];

/* Helper function definitions */
static void reset_test_buf(void);
static void init_state(void);
static void entry_state(void);
static void exit_state(void);
static enum state_status press_ok_mode_button(void);
static void press_up_button(void);
static void press_down_button(void);
static void refresh(void);


TEST_GROUP(set_time);

TEST_SETUP(set_time)
{
    /* Init before every test */
    /* Call state init and reset buffers */
    init_state();
    display_mock_reset_buf();
    reset_test_buf();
}


TEST_TEAR_DOWN(set_time)
{
    /* Cleanup after every test */
}


TEST(set_time, FirstTest)
{
    TEST_FAIL();
}


/* Helper functions */
static void reset_test_buf(void)
{
    memset(test_buf, 0, 80);
}


static void init_state(void)
{
    state_set_time_funs() -> init(get_display_mock_funs());
}


static void entry_state(void)
{
    state_set_time_funs() -> on_entry(last_state);
    display_mock_read_buf(test_buf);
}


static void exit_state(void)
{
    state_set_time_funs() -> on_exit();
    display_mock_read_buf(test_buf);
}


static enum state_status press_ok_mode_button(void)
{
    return state_set_time_funs() -> on_ok_mode_button_pressed();
}


static void press_up_button(void)
{
    state_set_time_funs() -> on_up_button_pressed();
    display_mock_read_buf(test_buf);
}


static void press_down_button(void)
{
    state_set_time_funs() -> on_down_button_pressed();
    display_mock_read_buf(test_buf);
}


static void refresh(void)
{
    state_set_time_funs() -> on_refresh();
    display_mock_read_buf(test_buf);
}