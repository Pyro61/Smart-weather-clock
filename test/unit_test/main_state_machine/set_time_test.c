#include "unity/fixture/unity_fixture.h"
#include "assert.h"
#include "../Src/main_state_machine/state.h"
#include "../Src/main_state_machine/state_set_time/state_set_time.h"
#include "display_mock.h"
#include <string.h>


/* Messages showed by display */
#define CLEARED_MESSAGE             "\0"
#define ENTRY_MESSAGE               "      SET TIME      \n      HH:MM:SS      \n      00:00:00      "
#define BLANK_HOUR_TENS             "      SET TIME      \n      HH:MM:SS      \n       0:00:00      "
#define BLANK_HOUR_UNITS            "      SET TIME      \n      HH:MM:SS      \n      0 :00:00      "
#define BLANK_MINUTE_TENS           "      SET TIME      \n      HH:MM:SS      \n      00: 0:00      "
#define BLANK_MINUTE_UNITS          "      SET TIME      \n      HH:MM:SS      \n      00:0 :00      "
#define BLANK_SECOND_TENS           "      SET TIME      \n      HH:MM:SS      \n      00:00: 0      "
#define BLANK_SECOND_UNITS          "      SET TIME      \n      HH:MM:SS      \n      00:00:0       "


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
static void press_left_button(void);
static void press_right_button(void);
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


TEST(set_time, OnEntryDisplayOutput)
{
    entry_state();
    TEST_ASSERT_EQUAL_STRING(ENTRY_MESSAGE, test_buf);
}


TEST(set_time, OnExitDisplayOutput)
{
    entry_state();
    exit_state();
    TEST_ASSERT_EQUAL_STRING(CLEARED_MESSAGE, test_buf);
}


TEST(set_time, PressedOkButtonReturnedStateIsTheSameAsGivenToStateEntry)
{
    last_state = WEATHER_OUT;
    entry_state();
    enum state_status returned_state = press_ok_mode_button();
    TEST_ASSERT_EQUAL(last_state, returned_state);
}


TEST(set_time, WhenEditingHourTensPressLeftButtonAfterThatRefreshThenEditingTimeIsStillHourTens)
{
    entry_state();
    press_left_button();
    refresh();
    TEST_ASSERT_EQUAL_STRING(BLANK_HOUR_TENS, test_buf);
}


TEST(set_time, WhenEditingHourTensPressRightButtonAfterThatRefreshThenEditingTimeIsHourUnits)
{
    entry_state();
    press_right_button();
    refresh();
    TEST_ASSERT_EQUAL_STRING(BLANK_HOUR_UNITS, test_buf);
}


TEST(set_time, WhenEditingSecondTensPressLeftButtonAfterThatRefreshThenEditingTimeIsMinuteUnits)
{
    uint8_t i;
    entry_state();
    /* Press right button 4 times to start editing second tens */
    for (i = 0; i < 4; i++)
    {
        press_right_button();
    }
    press_left_button();
    refresh();
    TEST_ASSERT_EQUAL_STRING(BLANK_MINUTE_UNITS, test_buf);
}


TEST(set_time, WhenEditingSecondUnitsPressRightButtonAfterThatRefreshThenEditingTimeIsStillSecondUnits)
{
    uint8_t i;
    entry_state();
    /* Press right button 5 times to start editing second units */
    for (i = 0; i < 5; i++)
    {
        press_right_button();
    }
    /* Press right button one more time */
    press_right_button();
    refresh();
    TEST_ASSERT_EQUAL_STRING(BLANK_SECOND_UNITS, test_buf);
}


TEST(set_time, WhenEditingHourTensPressDownButtonThenHourTensIsStill0)
{
    entry_state();
    press_down_button();
    TEST_ASSERT_EQUAL_STRING(ENTRY_MESSAGE, test_buf);
}


TEST(set_time, WhenEditingHourTensPressUpButtonThenHourTensIs1)
{
    entry_state();
    press_up_button();
    TEST_ASSERT_EQUAL_STRING("      SET TIME      \n      HH:MM:SS      \n      10:00:00      ", test_buf);
}


TEST(set_time, WhenEditingHourTensPressUpButton3TimesThenHourTensIs2InsteadOf3)
{
    uint8_t i;
    entry_state();
    for(i = 0; i < 3; i++)
    {
        press_up_button();
    }
    TEST_ASSERT_EQUAL_STRING("      SET TIME      \n      HH:MM:SS      \n      20:00:00      ", test_buf);
}


TEST(set_time, WhenEditingHourUnitsPressUpButton10TimesThenHourIs9InsteadOf10)
{
    uint8_t i;
    entry_state();
    press_right_button();
    for(i = 0; i < 10; i++)
    {
        press_up_button();
    }
    TEST_ASSERT_EQUAL_STRING("      SET TIME      \n      HH:MM:SS      \n      09:00:00      ", test_buf);
}


TEST(set_time, WhenTimeIsSetTo00_50_00AndEditingPartIsMinuteUnitsPressDownButtonTimeDoNotChange)
{
    uint8_t i;
    entry_state();
    /* Enter editing minute tens */
    press_right_button();
    press_right_button();
    /* Set minute tens to 5 */
    for (i = 0; i < 5; i++)
    {
        press_up_button();
    }
    /* Enter editing minute units */
    press_right_button();
    /* Minute units 0 -> 0 */
    press_down_button();
    TEST_ASSERT_EQUAL_STRING("      SET TIME      \n      HH:MM:SS      \n      00:50:00      ", test_buf);
}


TEST(set_time, WhenTimeIsSetTo23_00_00AndEditingPartIsHourUnitsPressUpButtonThenTimeDoNotChange)
{
    uint8_t i;
    entry_state();
    /* Hour tens 0 -> 2 */
    for (i = 0; i < 2; i++)
    {
        press_up_button();
    }
    /* Enter editing hour units */
    press_right_button();
    /* Hour units 0 -> 3 */
    for (i = 0; i < 3; i++)
    {
        press_up_button();
    }
    /* Hour units 3 -> 3 */
    TEST_ASSERT_EQUAL_STRING("      SET TIME      \n      HH:MM:SS      \n      23:00:00      ", test_buf);
}


TEST(set_time, WhenTimeIsSetTo00_00_19AndEditingPartIsSecondUnitsRefresh4TimesAndTimeIsTheSame)
{
    uint8_t i;
    entry_state();
    /* Enter editing second tens */
    for (i = 0; i < 4; i++)
    {
        press_right_button();
    }
    /* Second tens 0 -> 1 */
    press_up_button();
    /* Enter editing second units */
    press_right_button();
    /* Second units 0 -> 9 */
    for (i = 0; i < 9; i++)
    {
        press_up_button();
    }
    /* Refresh 2 times */
    for (i = 0; i < 4; i++)
    {
        refresh();
    }
    TEST_ASSERT_EQUAL_STRING("      SET TIME      \n      HH:MM:SS      \n      00:00:19      ", test_buf);
}


TEST(set_time, WhenEditingHourTensPressRightButton2TimesAfterThatLeftButton1TimeThenEditingTimeIsHourUnits)
{
    entry_state();
    /* Enter editing minute tens */
    press_right_button();
    press_right_button();
    /* Enter editing hour units */
    press_left_button();
    /* Refresh */
    refresh();
    TEST_ASSERT_EQUAL_STRING(BLANK_HOUR_UNITS, test_buf);
}


TEST(set_time, WhenTimeIsSetTo20_00_00RefreshAfterThatPressRightButtonThenDisplayShows20_00_00)
{
    uint8_t i;
    entry_state();
    /* Hour tens 0 -> 2 */
    for (i = 0; i < 2; i++)
    {
        press_up_button();
    }
    /* Refresh */
    refresh();
    /* Press right button */
    press_right_button();
    TEST_ASSERT_EQUAL_STRING("      SET TIME      \n      HH:MM:SS      \n      20:00:00      ", test_buf);
}


TEST(set_time, WhenTimeIsSetTo01_00_00RefreshAfterThatPressDownButtonThenDisplayShows00_00_00)
{
    entry_state();
    /* Enter editing hour units */
    press_right_button();
    /* Hour units 0 -> 1 */
    press_up_button();
    /* Refresh */
    refresh();
    /* Hour units 1 -> 0 */
    press_down_button();
    TEST_ASSERT_EQUAL_STRING("      SET TIME      \n      HH:MM:SS      \n      00:00:00      ", test_buf);
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


static void press_left_button(void)
{
    state_set_time_funs() -> on_left_button_pressed();
    display_mock_read_buf(test_buf);
}


static void press_right_button(void)
{
    state_set_time_funs() -> on_right_button_pressed();
    display_mock_read_buf(test_buf);
}


static void refresh(void)
{
    state_set_time_funs() -> on_refresh();
    display_mock_read_buf(test_buf);
}