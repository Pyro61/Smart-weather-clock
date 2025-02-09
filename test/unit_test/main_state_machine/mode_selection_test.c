#include "unity/fixture/unity_fixture.h"

#include "assert.h"
#include "../Src/main_state_machine/state.h"
#include "../Src/main_state_machine/state_mode_selection/state_mode_selection.h"
#include "display_mock.h"
#include <string.h>

typedef const struct main_state_interface * (*main_state_get_t)(void);
static const main_state_get_t (mode_selection_funs) = main_state_mode_selection_get;

char test_buf[80];

static void reset_test_buf(void)
{
    memset(test_buf, 0, 80);
}

TEST_GROUP(mode_selection);

TEST_SETUP(mode_selection)
{
    /* Init before every test */
    mode_selection_funs() -> init(get_display_mock_funs());
    display_mock_reset_buf();
    reset_test_buf();
}

TEST_TEAR_DOWN(mode_selection)
{
    /* Cleanup after every test */
}

TEST(mode_selection, OnEntryDisplayOutput)
{
    mode_selection_funs() -> on_entry();
    display_mock_read_buf(test_buf);
    TEST_ASSERT_EQUAL_STRING("> 1. weather outdoor\n  2. weather indoor\n  3. set time\n  4. set alarm", test_buf);
}


void safe_state(void)
{
    while (1);
}