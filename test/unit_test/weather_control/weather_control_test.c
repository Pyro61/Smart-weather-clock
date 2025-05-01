#include "unity/fixture/unity_fixture.h"

#include "assert.h"

#include "weather_control/weather_sensors_control.h"
#include "mock_sensors.h"

TEST_GROUP(weather_control);

TEST_SETUP(weather_control)
{
    /* Init before every test */
    mock_init_sensors();
    weather_sensors_get_data_init(temp_get, pressure_get, humidity_get, rain_state_get);
}

TEST_TEAR_DOWN(weather_control)
{
    /* Cleanup after every test */
}

TEST(weather_control, GetTemp)
{
    struct weather_sensors_get_data *get_funs = weather_sensors_get_data_get();
    mock_set_temp(20);
    temp_Cdeg_t temp = get_funs -> get_temp();
    TEST_ASSERT_EQUAL(20, temp);
}

TEST(weather_control, GetPressure)
{
    struct weather_sensors_get_data *get_funs = weather_sensors_get_data_get();
    mock_set_pressure(1010);
    temp_Cdeg_t pressure = get_funs -> get_pressure();
    TEST_ASSERT_EQUAL(1010, pressure);
}

TEST(weather_control, GetHumidity)
{
    struct weather_sensors_get_data *get_funs = weather_sensors_get_data_get();
    mock_set_humidity(50);
    temp_Cdeg_t humidity = get_funs -> get_humidity();
    TEST_ASSERT_EQUAL(50, humidity);
}

TEST(weather_control, GetRainState)
{
    struct weather_sensors_get_data *get_funs = weather_sensors_get_data_get();
    mock_set_rain_state(RAINING);
    enum rain_state state = get_funs -> is_raining();
    TEST_ASSERT_EQUAL(RAINING, state);
}