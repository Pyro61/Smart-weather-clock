#include "sys_comm_send_recv.h"
#include "../events/events.h"
#include <stdbool.h>


void sys_comm_send_write_length(buf_t buf, req_resp_length_t len)
{
    buf[SYS_COMM_MSG_LENGTH_IDX] = len;
}


void sys_comm_send_write_slave_id(buf_t buf, slave_id_t id)
{
    buf[SYS_COMM_SLAVE_ID_IDX] = id;
}


static bool is_slave_id_correct(slave_id_t id)
{
    return id == DEVICE_ID;
}


static int8_t read_cmd(buf_t buf)
{
    int8_t tmp = buf[SYS_COMM_CMD_IDX];
    /* Request */
    if (tmp >= REQ_ID_START && tmp <= REQ_ID_END)
    {
        return tmp;
    }

    /* Response */
    else if (tmp >= RESP_ID_START && tmp <= RESP_ID_END)
    {
        return tmp;
    }

    /* Wrong request */
    else if (tmp < SYS_COMM_RESP_CMD_ADD)
    {
        return ERR_REQ;
    }

    /* Wrong response */
    else
    {
        return ERR_RESP;
    }
}


enum sys_comm_recv_err sys_comm_recv(buf_t buf)
{
    if (!is_slave_id_correct(buf[SYS_COMM_SLAVE_ID_IDX]))
    {
        /* Wrong slave id */
        return ERR_SLAVE_ID;
    }

    /* Read cmd from buffer */
    /* Wrong cmd err code handling is included inside read_cmd function */
    enum sys_comm_recv_err recv = read_cmd(buf);

    return recv;
}