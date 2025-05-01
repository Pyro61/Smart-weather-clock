#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(weather_control)
{
   /* Test cases to run */
   RUN_TEST_CASE(weather_control, GetTemp);
   RUN_TEST_CASE(weather_control, GetPressure);
   RUN_TEST_CASE(weather_control, GetHumidity);
   RUN_TEST_CASE(weather_control, GetRainState);
   

}