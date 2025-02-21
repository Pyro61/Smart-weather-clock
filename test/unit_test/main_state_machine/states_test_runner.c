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
   RUN_TEST_CASE(mode_selection, WhenSelectedModeIs1ClickDownButtonAfterUpButtonThenSelectedModeIsStill1);
   RUN_TEST_CASE(mode_selection, WhenSelectedModeIs4ClickDownButtonThenSelectedModeIsStill4);
   RUN_TEST_CASE(mode_selection, SelectedMode1MessageAfterRefreshDontHaveSelectedModeSign);
   RUN_TEST_CASE(mode_selection, AfterDoubleRefreshThereIsSelectedSign);
   RUN_TEST_CASE(mode_selection, AfterRefreshChangedSelectedModeThenThereIsSelectedModeSign);
   RUN_TEST_CASE(mode_selection, AfterRefreshChangedSelectedModeAfterThatRefreshThenThereIsNoSelectedSign);
}

TEST_GROUP_RUNNER(set_time)
{
   RUN_TEST_CASE(set_time, OnEntryDisplayOutput);
   RUN_TEST_CASE(set_time, OnEntryDisplayOutput);
   RUN_TEST_CASE(set_time, PressedOkButtonReturnedStateIsTheSameAsGivenToStateEntry);
   RUN_TEST_CASE(set_time, WhenEditingHourTensPressLeftButtonAfterThatRefreshThenEditingTimeIsStillHourTens);
   RUN_TEST_CASE(set_time, WhenEditingHourTensPressRightButtonAfterThatRefreshThenEditingTimeIsHourUnits);
   RUN_TEST_CASE(set_time, WhenEditingSecondTensPressLeftButtonAfterThatRefreshThenEditingTimeIsMinuteUnits);
   RUN_TEST_CASE(set_time, WhenEditingSecondUnitsPressRightButtonAfterThatRefreshThenEditingTimeIsStillSecondUnits);
   RUN_TEST_CASE(set_time, WhenEditingHourTensPressDownButtonThenHourTensIsStill0);
   RUN_TEST_CASE(set_time, WhenEditingHourTensPressUpButtonThenHourTensIs1);
   RUN_TEST_CASE(set_time, WhenEditingHourTensPressUpButton3TimesThenHourTensIs2InsteadOf3);
   RUN_TEST_CASE(set_time, WhenEditingHourUnitsPressUpButton10TimesThenHourIs9InsteadOf10);
   RUN_TEST_CASE(set_time, WhenTimeIsSetTo00_50_00AndEditingPartIsMinuteUnitsPressDownButtonTimeDoNotChange);
   RUN_TEST_CASE(set_time, WhenTimeIsSetTo23_00_00AndEditingPartIsHourUnitsPressUpButtonThenTimeDoNotChange);
   RUN_TEST_CASE(set_time, WhenTimeIsSetTo00_00_19AndEditingPartIsSecondUnitsRefresh4TimesAndTimeIsTheSame);
   RUN_TEST_CASE(set_time, WhenEditingHourTensPressRightButton2TimesAfterThatLeftButton1TimeThenEditingTimeIsHourUnits);
   RUN_TEST_CASE(set_time, WhenTimeIsSetTo20_00_00RefreshAfterThatPressRightButtonThenDisplayShows20_00_00);
   RUN_TEST_CASE(set_time, WhenTimeIsSetTo01_00_00RefreshAfterThatPressDownButtonThenDisplayShows00_00_00);
}

TEST_GROUP_RUNNER(set_alarm)
{
   RUN_TEST_CASE(set_alarm, OnEntryDisplayOutput);
   RUN_TEST_CASE(set_alarm, OnEntryDisplayOutput);
   RUN_TEST_CASE(set_alarm, PressedOkButtonReturnedStateIsTheSameAsGivenToStateEntry);
   RUN_TEST_CASE(set_alarm, WhenEditingHourTensPressLeftButtonAfterThatRefreshThenEditingTimeIsStillHourTens);
   RUN_TEST_CASE(set_alarm, WhenEditingHourTensPressRightButtonAfterThatRefreshThenEditingTimeIsHourUnits);
   RUN_TEST_CASE(set_alarm, WhenEditingSecondTensPressLeftButtonAfterThatRefreshThenEditingTimeIsMinuteUnits);
   RUN_TEST_CASE(set_alarm, WhenEditingSecondUnitsPressRightButtonAfterThatRefreshThenEditingTimeIsStillSecondUnits);
   RUN_TEST_CASE(set_alarm, WhenEditingHourTensPressDownButtonThenHourTensIsStill0);
   RUN_TEST_CASE(set_alarm, WhenEditingHourTensPressUpButtonThenHourTensIs1);
   RUN_TEST_CASE(set_alarm, WhenEditingHourTensPressUpButton3TimesThenHourTensIs2InsteadOf3);
   RUN_TEST_CASE(set_alarm, WhenEditingHourUnitsPressUpButton10TimesThenHourIs9InsteadOf10);
   RUN_TEST_CASE(set_alarm, WhenTimeIsSetTo00_50_00AndEditingPartIsMinuteUnitsPressDownButtonTimeDoNotChange);
   RUN_TEST_CASE(set_alarm, WhenTimeIsSetTo23_00_00AndEditingPartIsHourUnitsPressUpButtonThenTimeDoNotChange);
   RUN_TEST_CASE(set_alarm, WhenTimeIsSetTo00_00_19AndEditingPartIsSecondUnitsRefresh4TimesAndTimeIsTheSame);
   RUN_TEST_CASE(set_alarm, WhenEditingHourTensPressRightButton2TimesAfterThatLeftButton1TimeThenEditingTimeIsHourUnits);
   RUN_TEST_CASE(set_alarm, WhenTimeIsSetTo20_00_00RefreshAfterThatPressRightButtonThenDisplayShows20_00_00);
   RUN_TEST_CASE(set_alarm, WhenTimeIsSetTo01_00_00RefreshAfterThatPressDownButtonThenDisplayShows00_00_00);
}

TEST_GROUP_RUNNER(alarm)
{
   RUN_TEST_CASE(alarm, OnEntryDisplayOutput);
   RUN_TEST_CASE(alarm, OnExitDisplayOutput);
   RUN_TEST_CASE(alarm, PressedOkButtonReturnedStateIsTheSameAsGivenToStateEntry);
   RUN_TEST_CASE(alarm, WriteTimeThenEntryAlarmStateTimeShownOnDisplayIsTheSameAsWritten);
   RUN_TEST_CASE(alarm, AfterRefreshAlarmStringIsGone);
}