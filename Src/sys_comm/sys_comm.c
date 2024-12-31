#include "sys_comm.h"
#include <string.h>


/* Request functions */
req_resp_length_t sys_comm_req_start_meas(buf_t buf)
{
    buf[SYS_COMM_CMD_IDX] = REQ_START_MEASURING;

    return SYS_COMM_REQ_RESP_SIZE;
}


req_resp_length_t sys_comm_req_stop_meas(buf_t buf)
{
    buf[SYS_COMM_CMD_IDX] = REQ_STOP_MEASURING;

    return SYS_COMM_REQ_RESP_SIZE;
}


req_resp_length_t sys_comm_req_get_data(buf_t buf)
{
    buf[SYS_COMM_CMD_IDX] = REQ_GET_DATA;

    return SYS_COMM_REQ_RESP_SIZE;
}


/* Response functions */
req_resp_length_t sys_comm_resp_get_data(buf_t buf, buf_t data)
{
    buf[SYS_COMM_CMD_IDX] = RESP_GET_DATA;
    memcpy(buf + SYS_COMM_DATA_START_IDX, data, SYS_COMM_DATA_SIZE);

    return SYS_COMM_DATA_SIZE + SYS_COMM_REQ_RESP_SIZE;
}