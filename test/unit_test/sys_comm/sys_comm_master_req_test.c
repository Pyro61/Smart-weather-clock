#include "unity/fixture/unity_fixture.h"

#include "assert.h"
#include "sys_comm/sys_comm.h"
#include <string.h>

#define TEST_SLAVE_ID   0xAA

/* Add to avoid unused variable warnings */
#define UNUSED(x)   (void)(x)

static uint8_t send_buf[SYS_COMM_BUF_SIZE];

TEST_GROUP(sys_comm_master_req);

TEST_SETUP(sys_comm_master_req)
{
    /* Init before every test */
    memset(send_buf, 0, SYS_COMM_BUF_SIZE);
}

TEST_TEAR_DOWN(sys_comm_master_req)
{
    /* Cleanup after every test */
}

TEST(sys_comm_master_req, MasterSendReqStartMeasuring)
{

    req_resp_length_t len = sys_comm_req_start_meas(send_buf);
    UNUSED(len);
    TEST_ASSERT_EQUAL(REQ_START_MEASURING, send_buf[SYS_COMM_CMD_IDX]);
}

TEST(sys_comm_master_req, MasterSendReqStopMeasuring)
{
    req_resp_length_t len = sys_comm_req_stop_meas(send_buf);
    UNUSED(len);
    TEST_ASSERT_EQUAL(REQ_STOP_MEASURING, send_buf[SYS_COMM_CMD_IDX]);
}

TEST(sys_comm_master_req, MasterSendReqGetData)
{
    req_resp_length_t len = sys_comm_req_get_data(send_buf);
    UNUSED(len);
    TEST_ASSERT_EQUAL(REQ_GET_DATA, send_buf[SYS_COMM_CMD_IDX]);
}

TEST(sys_comm_master_req, MasterSendReqCheckLength)
{
    /* All require functions returns the same length, so it's no need to test all req functions */
    req_resp_length_t len = sys_comm_req_get_data(send_buf);

    TEST_ASSERT_EQUAL(SYS_COMM_REQ_RESP_SIZE, len);
}