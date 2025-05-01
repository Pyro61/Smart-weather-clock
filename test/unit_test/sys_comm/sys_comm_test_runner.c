#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(sys_comm_master_req)
{
   /* Test cases to run */
   RUN_TEST_CASE(sys_comm_master_req, MasterSendReqStartMeasuring);
   RUN_TEST_CASE(sys_comm_master_req, MasterSendReqStopMeasuring);
   RUN_TEST_CASE(sys_comm_master_req, MasterSendReqGetData);
   RUN_TEST_CASE(sys_comm_master_req, MasterSendReqCheckLength);
}

TEST_GROUP_RUNNER(sys_comm_slave_resp)
{
   /* Test cases to run */
   RUN_TEST_CASE(sys_comm_slave_resp, SlaveSendRespGetData);
   RUN_TEST_CASE(sys_comm_slave_resp, SlaveSendRespCheckLength)
   RUN_TEST_CASE(sys_comm_slave_resp, SlaveSendRespCheckData);
}

TEST_GROUP_RUNNER(sys_comm_send_recv)
{
   /* Test cases to run */
   RUN_TEST_CASE(sys_comm_send_recv, SendWriteMsgLength);
   RUN_TEST_CASE(sys_comm_send_recv, SendWriteSlaveId);
   RUN_TEST_CASE(sys_comm_send_recv, RecvWrongSlaveId);
   RUN_TEST_CASE(sys_comm_send_recv, RecvWrongReqEqual1BelowLowerLimit);
   RUN_TEST_CASE(sys_comm_send_recv, RecvWrongReqEqual1AboveUpperLimit);
   RUN_TEST_CASE(sys_comm_send_recv, RecvCorrectReqEqualLowerLimit);
   RUN_TEST_CASE(sys_comm_send_recv, RecvCorrectReqEqualUpperLimit);
   RUN_TEST_CASE(sys_comm_send_recv, RecvWrongRespEqual1BelowLowerLimit);
   RUN_TEST_CASE(sys_comm_send_recv, RecvWrongRespEqual1AboveUpperLimit);
   RUN_TEST_CASE(sys_comm_send_recv, RecvCorrectRespEqualLowerLimit);
   RUN_TEST_CASE(sys_comm_send_recv, RecvCorrectRespEqualUpperLimit);
}