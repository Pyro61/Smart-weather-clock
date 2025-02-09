#include "unity/fixture/unity_fixture.h"
#include "assert.h"
#include "../Src/main_state_machine/state.h"
#include "../Src/main_state_machine/state_mode_selection/state_mode_selection.h"
#include "display_mock.h"
#include <string.h>


/* Messages showed by display */
#define CLEARED_MESSAGE             "\0"
#define SELECTED_MODE_1_MESSAGE     "> 1. weather indoor\n  2. weather outdoor\n  3. set time\n  4. set alarm"
#define SELECTED_MODE_2_MESSAGE     "  1. weather indoor\n> 2. weather outdoor\n  3. set time\n  4. set alarm"
#define SELECTED_MODE_3_MESSAGE     "  1. weather indoor\n  2. weather outdoor\n> 3. set time\n  4. set alarm"
#define SELECTED_MODE_4_MESSAGE     "  1. weather indoor\n  2. weather outdoor\n  3. set time\n> 4. set alarm"
#define MESSAGE_AFTER_REFRESH       "  1. weather indoor\n  2. weather outdoor\n  3. set time\n  4. set alarm" /* No selected mode sign */

/* Modes */
#define MODE_1                      WEATHER_IN
#define MODE_2                      WEATHER_OUT
#define MODE_3                      SET_TIME
#define MODE_4                      SET_ALARM

typedef const struct main_state_interface * (*main_state_get_t)(void);
static const main_state_get_t (mode_selection_funs) = main_state_mode_selection_get;


char test_buf[80];

/* Helper function definitions */
static void reset_test_buf(void);
static void init_state(void);
static void entry_state(void);
static void exit_state(void);
static enum state_status press_ok_mode_button(void);
static void press_up_button(void);
static void press_down_button(void);
static void refresh(void);


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
    press_up_button(); /* Mode should be still 1 */
    enum state_status state = press_ok_mode_button();

    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_1_MESSAGE, test_buf);
    TEST_ASSERT_EQUAL(state, MODE_1);
}


TEST(mode_selection, WhenSelectedModeIs1ClickDownButtonThenSelectedModeIs2)
{
    entry_state();
    press_down_button(); /* Change mode 1 -> 2 */
    enum state_status state = press_ok_mode_button();

    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_2_MESSAGE, test_buf);
    TEST_ASSERT_EQUAL(state, MODE_2);
}


TEST(mode_selection, WhenSelectedModeIs2ClickDownButtonThenSelectedModeIs3)
{
    entry_state();
    press_down_button(); /* Change mode 1 -> 2 */
    press_down_button(); /* Change mode 2 -> 3 */
    enum state_status state = press_ok_mode_button();

    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_3_MESSAGE, test_buf);
    TEST_ASSERT_EQUAL(state, MODE_3);
}


TEST(mode_selection, WhenSelectedModeIs3ClickDownButtonThenSelectedModeIs4)
{
    entry_state();
    press_down_button(); /* Change mode 1 -> 2 */
    press_down_button(); /* Change mode 2 -> 3 */
    press_down_button(); /* Change mode 3 -> 4 */
    enum state_status state = press_ok_mode_button();

    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_4_MESSAGE, test_buf);
    TEST_ASSERT_EQUAL(state, MODE_4);
}


TEST(mode_selection, WhenSelectedModeIs1ClickDownButtonAfterUpButtonThenSelectedModeIsStill1)
{
    entry_state();
    press_down_button(); /* Change mode 1 -> 2 */
    press_up_button(); /* Change mode 2 -> 1 */
    enum state_status state = press_ok_mode_button();

    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_1_MESSAGE, test_buf);
    TEST_ASSERT_EQUAL(state, MODE_1);
}


TEST(mode_selection, WhenSelectedModeIs4ClickDownButtonThenSelectedModeIsStill4)
{
    entry_state();
    press_down_button(); /* Change mode 1 -> 2 */
    press_down_button(); /* Change mode 2 -> 3 */
    press_down_button(); /* Change mode 3 -> 4 */
    press_down_button(); /* Mode should be still 4 */
    enum state_status state = press_ok_mode_button();

    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_4_MESSAGE, test_buf);
    TEST_ASSERT_EQUAL(state, MODE_4);
}


TEST(mode_selection, SelectedMode1MessageAfterRefreshDontHaveSelectedModeSign)
{
    entry_state();
    refresh();
    TEST_ASSERT_EQUAL_STRING(MESSAGE_AFTER_REFRESH, test_buf);
}


TEST(mode_selection, AfterDoubleRefreshThereIsSelectedSign)
{
    entry_state();
    refresh();
    refresh();
    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_1_MESSAGE, test_buf);
}


TEST(mode_selection, AfterRefreshChangedSelectedModeThenThereIsSelectedModeSign)
{
    entry_state();
    refresh();
    press_down_button();
    TEST_ASSERT_EQUAL_STRING(SELECTED_MODE_2_MESSAGE, test_buf);
}


TEST(mode_selection, AfterRefreshChangedSelectedModeAfterThatRefreshThenThereIsNoSelectedSign)
{
    entry_state();
    refresh();
    press_down_button();
    refresh();
    TEST_ASSERT_EQUAL_STRING(MESSAGE_AFTER_REFRESH, test_buf);
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


static void press_down_button(void)
{
    mode_selection_funs() -> on_down_button_pressed();
    display_mock_read_buf(test_buf);
}


static void refresh(void)
{
    mode_selection_funs() -> on_refresh();
    display_mock_read_buf(test_buf);
}


/* For correct compilation */
void safe_state(void)
{
    while (1);
}