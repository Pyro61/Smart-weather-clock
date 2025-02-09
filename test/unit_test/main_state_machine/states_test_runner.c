#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(mode_selection)
{
   /* Test cases to run */
   RUN_TEST_CASE(mode_selection, OnEntryDisplayOutput);
   RUN_TEST_CASE(mode_selection, OnExitDisplayOutput);
   RUN_TEST_CASE(mode_selection, AfterEntrySelectedModeIs1);
   RUN_TEST_CASE(mode_selection, WhenSelectedModeIs1ClickUpButtonThenSelectedModeIsStill1);
   RUN_TEST_CASE(mode_selection, WhenSelectedModeIs1ClickDownButtonThenSelectedModeIs2);
   RUN_TEST_CASE(mode_selection, WhenSelectedModeIs2ClickDownButtonThenSelectedModeIs3);
   RUN_TEST_CASE(mode_selection, WhenSelectedModeIs3ClickDownButtonThenSelectedModeIs4);
}