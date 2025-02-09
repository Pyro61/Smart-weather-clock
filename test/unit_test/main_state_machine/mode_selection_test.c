#include "unity/fixture/unity_fixture.h"

#include "assert.h"
#include "../Src/main_state_machine/state.h"
#include "../Src/main_state_machine/state_mode_selection/state_mode_selection.h"
#include "display_mock.h"
#include <string.h>


#define CLEARED_MESSAGE             "\0"
#define SELECTED_MODE_1_MESSAGE     "> 1. weather indoor\n  2. weather outdoor\n  3. set time\n  4. set alarm"

#define MODE_1                      WEATHER_IN

typedef const struct main_state_interface * (*main_state_get_t)(void);
static const main_state_get_t (mode_selection_funs) = main_state_mode_selection_get;

char test_buf[80];

static void reset_test_buf(void);
static void init_state(void);
static void entry_state(void);
static void exit_state(void);
static enum state_status press_ok_mode_button(void);
static void press_up_button(void);

TEST_GROUP(mode_selection);

TEST_SETUP(mode_selection)
{
    /* Init before every test */
    /* Call state init and reset buffers */
    init_state();
    display_mock_reset_buf();
    reset_test_buf();
}


TEST_TEAR_DOWN(mode_selection)
{
    /* Cleanup after every test */
}


TEST(mode_selection, OnEntryDisplayOutput)
{
    entry_state();
    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_1_MESSAGE, test_buf);
}


TEST(mode_selection, OnExitDisplayOutput)
{
    entry_state();
    exit_state();
    TEST_ASSERT_EQUAL_STRING(CLEARED_MESSAGE, test_buf);
}


TEST(mode_selection, AfterEntrySelectedModeIs1)
{
    entry_state();
    enum state_status state = press_ok_mode_button();
    TEST_ASSERT_EQUAL(state, MODE_1);
}


TEST(mode_selection, WhenSelectedModeIs1ClickUpButtonThenSelectedModeIsStill1)
{
    entry_state();
    press_up_button();
    enum state_status state = press_ok_mode_button();

    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_1_MESSAGE, test_buf);
    TEST_ASSERT_EQUAL(state, MODE_1);
}


/* Helper functions */
static void reset_test_buf(void)
{
    memset(test_buf, 0, 80);
}


static void init_state(void)
{
    mode_selection_funs() -> init(get_display_mock_funs());
}


static void entry_state(void)
{
    mode_selection_funs() -> on_entry();
    display_mock_read_buf(test_buf);
}


static void exit_state(void)
{
    mode_selection_funs() -> on_exit();
    display_mock_read_buf(test_buf);
}


static enum state_status press_ok_mode_button(void)
{
    return mode_selection_funs() -> on_ok_mode_button_pressed();
}


static void press_up_button(void)
{
    mode_selection_funs() -> on_up_button_pressed();
    display_mock_read_buf(test_buf);
}


/* For correct compilation */
void safe_state(void)
{
    while (1);
}