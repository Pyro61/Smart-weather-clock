#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(events)
{
   /* Test cases to run */
   RUN_TEST_CASE(events, Subscribe1EventThenCallThatEvent);
   RUN_TEST_CASE(events, Subscribe1EventThenCallOtherEvents);
   RUN_TEST_CASE(events, Subscribe3DifferentEventsThenCallThatEvents);
   RUN_TEST_CASE(events, Subscribe1EventThenCallThatEvent2Times);
   RUN_TEST_CASE(events, Subscribe1Event2TimesThen2ndSubscribeIsRejected);
   RUN_TEST_CASE(events, Subscribe1FunctionTo2DifferentEventsThenCall2Events);
   RUN_TEST_CASE(events, ReachMaxSubscribersThenSubscribe1MoreEventAndCallEvent);
   RUN_TEST_CASE(events, Subscribe2FunctionsTo1EventThenCallThatEvent);
   RUN_TEST_CASE(events, SubscribeToWrongEventShouldBeRejected);
   RUN_TEST_CASE(events, SubscribeNULLPtrReturnsFailCode);

}