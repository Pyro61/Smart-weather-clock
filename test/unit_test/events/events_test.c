#include "unity/fixture/unity_fixture.h"

#include "assert.h"

#include "events/events.h"
#include "mock_funs.h"

#define RUN_EVENT_RET_VALUE 3
#define SLEEP_EVENT_RET_VALUE 1
#define GET_EVENT_RET_VALUE 5

static int cb_value;

/* Callback functions */
static void cb_run_event(void)
{
    cb_value += RUN_EVENT_RET_VALUE;
}

static void cb_sleep_event(void)
{
    cb_value += SLEEP_EVENT_RET_VALUE;
}

static void cb_get_event(void)
{
    cb_value += GET_EVENT_RET_VALUE;
}

/* Support functions */
static int call_run_event(void)
{
    events_notify_subscribers(EVENT_RUN);
    return RUN_EVENT_RET_VALUE;
}

static int call_sleep_event(void)
{
    events_notify_subscribers(EVENT_SLEEP);
    return SLEEP_EVENT_RET_VALUE;
}

static int call_get_event(void)
{
    events_notify_subscribers(EVENT_GET);
    return GET_EVENT_RET_VALUE;
}

static void fill_whole_subs_memory(void)
{
    for (int i = 0; i < EVENTS_MAX_SUBSCRIBERS; i++)
    {
        /* Filling subscribe array with mock funs, event doesn't matter */
        events_subscribe(get_mock_fun_addr(i), EVENT_SLEEP);
    }
}


TEST_GROUP(events);

TEST_SETUP(events)
{
    /* Init before every test */
    cb_value = 0;
    events_clear();
}

TEST_TEAR_DOWN(events)
{
    /* Cleanup after every test */
}

TEST(events, Subscribe1EventThenCallThatEvent)
{
    int sub_ret, expected = 0;

    sub_ret = events_subscribe(cb_run_event, EVENT_RUN);
    expected += call_run_event();

    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret);
    TEST_ASSERT_EQUAL(expected, cb_value);
}

TEST(events, Subscribe1EventThenCallOtherEvents)
{
    int sub_ret;

    sub_ret = events_subscribe(cb_run_event, EVENT_RUN);
    /* Must be done directly from production code because we don't care for returned value (cb function is not called) */
    events_notify_subscribers(EVENT_SLEEP);
    events_notify_subscribers(EVENT_GET);

    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret);
    TEST_ASSERT_EQUAL(0, cb_value);
}

TEST(events, Subscribe3DifferentEventsThenCallThatEvents)
{
    int sub_ret[3], expected = 0;

    sub_ret[0] = events_subscribe(cb_run_event, EVENT_RUN);
    sub_ret[1] = events_subscribe(cb_sleep_event, EVENT_SLEEP);
    sub_ret[2] = events_subscribe(cb_get_event, EVENT_GET);

    expected += call_run_event();
    expected += call_sleep_event();
    expected += call_get_event();

    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret[0]);
    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret[1]);
    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret[2]);
    TEST_ASSERT_EQUAL(expected, cb_value);
}

TEST(events, Subscribe1EventThenCallThatEvent2Times)
{
    int sub_ret, expected = 0;

    sub_ret = events_subscribe(cb_get_event, EVENT_GET);

    expected += call_get_event();
    expected += call_get_event();

    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret);
    TEST_ASSERT_EQUAL(expected, cb_value);
}

TEST(events, Subscribe1Event2TimesThen2ndSubscribeIsRejected)
{
    int sub_ret[2], expected = 0;

    sub_ret[0] = events_subscribe(cb_sleep_event, EVENT_SLEEP);
    sub_ret[1] = events_subscribe(cb_sleep_event, EVENT_SLEEP);

    expected += call_sleep_event();

    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret[0]);
    TEST_ASSERT_EQUAL(SUBSCRIBE_ALREADY_TAKEN, sub_ret[1]);
    TEST_ASSERT_EQUAL(expected, cb_value);
}

TEST(events, Subscribe1FunctionTo2DifferentEventsThenCall2Events)
{
    int sub_ret[2], expected = 0;

    sub_ret[0] = events_subscribe(cb_run_event, EVENT_RUN);
    sub_ret[1] = events_subscribe(cb_run_event, EVENT_SLEEP);

    expected += call_run_event();

    /* Must be done directly from production code because call_sleep_event() returns value for cb_sleep_event() */
    events_notify_subscribers(EVENT_SLEEP);
    expected += RUN_EVENT_RET_VALUE;

    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret[0]);
    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret[1]);
    TEST_ASSERT_EQUAL(expected, cb_value);
}

TEST(events, ReachMaxSubscribersThenSubscribe1MoreEventAndCallEvent)
{
    int sub_ret;

    fill_whole_subs_memory();
    /* There's no more space for subscribers, so that subscribe should be rejected */
    sub_ret = events_subscribe(cb_get_event, EVENT_GET);

    events_notify_subscribers(EVENT_GET);

    TEST_ASSERT_EQUAL(SUBSCRIBE_NO_SPACE, sub_ret);
    TEST_ASSERT_EQUAL(0, cb_value);
}

TEST(events, Subscribe2FunctionsTo1EventThenCallThatEvent)
{
    int sub_ret[2], expected = 0;

    sub_ret[0] = events_subscribe(cb_get_event, EVENT_GET);
    sub_ret[1] = events_subscribe(cb_run_event, EVENT_GET);

    /* 2 functions are called by one event, so it can't be done with call_get_event() fun */
    events_notify_subscribers(EVENT_GET);
    expected += RUN_EVENT_RET_VALUE + GET_EVENT_RET_VALUE;

    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret[0]);
    TEST_ASSERT_EQUAL(SUBSCRIBE_SUCCESS, sub_ret[1]);
    TEST_ASSERT_EQUAL(expected, cb_value);
}

TEST(events, SubscribeToWrongEventShouldBeRejected)
{
    int sub_ret;

    sub_ret = events_subscribe(cb_get_event, NO_EVENT);
    events_notify_subscribers(NO_EVENT);

    TEST_ASSERT_EQUAL(SUBSCRIBE_WRONG_EVENT_ID, sub_ret);
    TEST_ASSERT_EQUAL(0, cb_value);
}

TEST(events, SubscribeNULLPtrReturnsFailCode)
{
    int sub_ret;

    sub_ret = events_subscribe(NULL, EVENT_RUN);

    TEST_ASSERT_EQUAL(SUBSCRIBE_NULLPTR, sub_ret);
}