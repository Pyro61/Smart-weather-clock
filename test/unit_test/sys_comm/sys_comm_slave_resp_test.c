#include "unity/fixture/unity_fixture.h"

#include "assert.h"
#include "sys_comm/sys_comm.h"
#include "../../Src/types.h"
#include "mock_get_data.h"
#include <string.h>

#define TEST_SLAVE_ID   0xAA

/* Add to avoid unused variable warnings */
#define UNUSED(x)   (void)(x)

static uint8_t send_buf[SYS_COMM_BUF_SIZE];
static struct weather_data test_data;

static void read_data_from_buf(const uint8_t *const buf, struct weather_data *const data_buf)
{
    struct weather_data *tmp = (struct weather_data*)buf;
    data_buf->temp = tmp->temp;
    data_buf->pressure = tmp->pressure;
    data_buf->humidity = tmp->humidity;
    data_buf->is_raining = tmp->is_raining;
}

/*
static uint8_t read_buf[SYS_COMM_BUF_SIZE];

static void read_request(req_resp_lenght_t len)
{
    memcpy(read_buf, send_buf, len);
}

static req_resp_lenght_t send_request(enum sys_comm_req_cmd req)
{
    switch (req)
    {
        case REQ_START_MEASURING:
        {
            sys_comm_req_start_meas(send_buf, TEST_SLAVE_ID);
            break;
        }

        case REQ_STOP_MEASURING:
        {
            sys_comm_req_stop_meas(send_buf, TEST_SLAVE_ID);
            break;
        }

        case REQ_GET_DATA:
        {
            sys_comm_req_get_data(send_buf, TEST_SLAVE_ID);
            break;
        }
    }
    return SYS_COMM_REQ_RESP_SIZE;
} */

TEST_GROUP(sys_comm_slave_resp);

TEST_SETUP(sys_comm_slave_resp)
{
    /* Init before every test */
    memset(send_buf, 0, SYS_COMM_BUF_SIZE);
}

TEST_TEAR_DOWN(sys_comm_slave_resp)
{
    /* Cleanup after every test */
}

//Move to lower submodule tests
/* TEST(sys_comm_slave_resp, SlaveReadReq)
{
    enum sys_comm_req_cmd req = REQ_START_MEASURING;
    req_resp_lenght_t len = send_request(req);
    read_request(len);

    TEST_ASSERT_EQUAL(req, read_buf[SYS_COMM_CMD_IDX]);
}
*/

TEST(sys_comm_slave_resp, SlaveSendRespGetData)
{
    req_resp_length_t len = sys_comm_resp_get_data(send_buf, (uint8_t*)&test_data);
    UNUSED(len);

    TEST_ASSERT_EQUAL(RESP_GET_DATA, send_buf[SYS_COMM_CMD_IDX]);
}

TEST(sys_comm_slave_resp, SlaveSendRespCheckLength)
{
    req_resp_length_t len = sys_comm_resp_get_data(send_buf, (uint8_t*)&test_data);

    TEST_ASSERT_EQUAL(SYS_COMM_DATA_SIZE + SYS_COMM_REQ_RESP_SIZE, len);
}

TEST(sys_comm_slave_resp, SlaveSendRespCheckData)
{
    /* Write test data to buffer */
    temp_Cdeg_t t = 50;
    pressure_hPa_t p = 1001;
    humidity_percent_t h = 48;
    enum rain_state r = SENSOR_NOT_APPLIED;
    mock_get_data(&test_data, t, p, h, r);

    /* Write response to buffer */
    req_resp_length_t len = sys_comm_resp_get_data(send_buf, (uint8_t*)&test_data);
    UNUSED(len);

    /* Read data from buffer */
    struct weather_data tmp = {0};
    read_data_from_buf(send_buf + SYS_COMM_DATA_START_IDX, &tmp);

    /* Test */
    TEST_ASSERT_EQUAL(t, tmp.temp);
    TEST_ASSERT_EQUAL(p, tmp.pressure);
    TEST_ASSERT_EQUAL(h, tmp.humidity);
    TEST_ASSERT_EQUAL(r, tmp.is_raining);
}


