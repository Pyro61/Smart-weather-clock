#include "unity/fixture/unity_fixture.h"

#include "assert.h"
#include "sys_comm/sys_comm_send_recv.h"
#include "sys_comm/sys_comm.h"
#include "sys_comm/sys_comm_types.h"
#include "types.h"
#include <string.h>
#include <stdbool.h>

#define TEST_SLAVE_ID   0xAA
#define WRONG_RESP      0x87

static uint8_t send_buf[SYS_COMM_BUF_SIZE];
static uint8_t recv_buf[SYS_COMM_BUF_SIZE];

static struct weather_data data;

/* Helper function definitions */
static void send_wrong_slave_id(void);
static void send_req(uint8_t req);
static void send_resp(uint8_t req);
static void receive(void);


TEST_GROUP(sys_comm_send_recv);

TEST_SETUP(sys_comm_send_recv)
{
    /* Init before every test */
    memset(send_buf, 0, SYS_COMM_BUF_SIZE);
    memset(recv_buf, 0, SYS_COMM_BUF_SIZE);
}

TEST_TEAR_DOWN(sys_comm_send_recv)
{
    /* Cleanup after every test */
}

TEST(sys_comm_send_recv, SendWriteMsgLength)
{
    req_resp_length_t len = sys_comm_req_get_data(send_buf);
    sys_comm_send_write_length(send_buf, len);

    TEST_ASSERT_EQUAL(len, send_buf[SYS_COMM_MSG_LENGTH_IDX]);
}

TEST(sys_comm_send_recv, SendWriteSlaveId)
{
    sys_comm_send_write_slave_id(send_buf, TEST_SLAVE_ID);
    TEST_ASSERT_EQUAL(TEST_SLAVE_ID, send_buf[SYS_COMM_SLAVE_ID_IDX]);
}

TEST(sys_comm_send_recv, RecvWrongSlaveId)
{
    send_wrong_slave_id();
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(ERR_SLAVE_ID, err);
}

TEST(sys_comm_send_recv, RecvWrongReqEqual1BelowLowerLimit)
{
    send_req(REQ_ID_START - 1);
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(ERR_REQ, err);
}

TEST(sys_comm_send_recv, RecvWrongReqEqual1AboveUpperLimit)
{
    send_req(REQ_ID_END + 1);
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(ERR_REQ, err);
}

TEST(sys_comm_send_recv, RecvCorrectReqEqualLowerLimit)
{
    send_req(REQ_ID_START);
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(REQ_ID_START, err);
}

TEST(sys_comm_send_recv, RecvCorrectReqEqualUpperLimit)
{
    send_req(REQ_ID_END);
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(REQ_ID_END, err);
}

TEST(sys_comm_send_recv, RecvWrongRespEqual1BelowLowerLimit)
{
    send_resp(RESP_ID_START - 1);
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(ERR_RESP, err);
}

TEST(sys_comm_send_recv, RecvWrongRespEqual1AboveUpperLimit)
{
    send_resp(RESP_ID_END + 1);
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(ERR_RESP, err);
}

TEST(sys_comm_send_recv, RecvCorrectRespEqualLowerLimit)
{
    send_resp(RESP_ID_START);
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(RESP_ID_START, err);
}

TEST(sys_comm_send_recv, RecvCorrectRespEqualUpperLimit)
{
    send_resp(RESP_ID_END);
    receive();
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);
    TEST_ASSERT_EQUAL(RESP_ID_END, err);
}


/* Helper functions */
static void send_wrong_slave_id(void)
{
    /* Only for simulating real work of module */
    req_resp_length_t len = sys_comm_req_get_data(send_buf);
    sys_comm_send_write_length(send_buf, len);

    /* Write wrong slave id */
    sys_comm_send_write_slave_id(send_buf, TEST_SLAVE_ID + 1);
}

static void send_req(uint8_t req)
{
    /* Only for getting length and simulating real work of mudule */
    req_resp_length_t len = sys_comm_req_get_data(send_buf);
    sys_comm_send_write_length(send_buf, len);
    sys_comm_send_write_slave_id(send_buf, TEST_SLAVE_ID);

    /* Write req */
    send_buf[SYS_COMM_CMD_IDX] = req;
}

static void send_resp(uint8_t resp)
{
    /* Only for getting length and simulating real work of mudule */
    req_resp_length_t len = sys_comm_resp_get_data(send_buf, (uint8_t*)&data);
    sys_comm_send_write_length(send_buf, len);
    sys_comm_send_write_slave_id(send_buf, TEST_SLAVE_ID);

    /* Write resp */
    send_buf[SYS_COMM_CMD_IDX] = resp;
}

static void receive(void)
{
    memcpy(recv_buf, send_buf, SYS_COMM_BUF_SIZE);
}