#include "adapter_sys_comm.h"
#include "sys_comm.h"
#include "../events/events.h"
#include "sys_comm_send_recv.h"
#include <stdint.h>


/* Data Buffers */
uint8_t send_buf[SYS_COMM_BUF_SIZE];
uint8_t recv_buf[SYS_COMM_BUF_SIZE];

/* Hw functions */
struct sys_comm_hw_funs hw;


static void adapter_sys_comm_recv(void)
{
    /* Get req/resp/err */
    enum sys_comm_recv_err err = sys_comm_recv(recv_buf);

    /* Correct req/resp received - save event as pending */
    /* Depends on req/resp index in events array */
    if (err > ERR_NONE)
    {
        events_notify_subscribers((enum event)err);
    }

    /* Wrong req/resp received - ignore msg */
    else{}
}


void adapter_sys_comm_init(struct sys_comm_hw_funs *funs)
{
    /* Get hw functions to send/recv data */
    hw.init = funs->init;
    hw.send = funs->send;
    hw.recv = funs->recv;
    
    /* Start hearing for receiving data */
    hw.recv(recv_buf, SYS_COMM_REQ_RESP_SIZE, adapter_sys_comm_recv);
}


void adapter_sys_comm_send_resp_data(struct weather_data *data)
{
    /* Write req data to buffer */
    req_resp_length_t len = sys_comm_resp_get_data(send_buf, (buf_t)data);
    sys_comm_send_write_length(send_buf, len);
    sys_comm_send_write_slave_id(send_buf, DEVICE_ID);

    /* Send req */
    hw.send(send_buf, len);
}


void adapter_sys_comm_send_req_start_meas(void)
{
    /* Write req data to buffer */
    req_resp_length_t len = sys_comm_req_start_meas(send_buf);
    sys_comm_send_write_length(send_buf, len);
    sys_comm_send_write_slave_id(send_buf, DEVICE_ID);

    /* Send req */
    hw.send(send_buf, len);
}


void adapter_sys_comm_send_req_stop_meas(void)
{
    /* Write req data to buffer */
    req_resp_length_t len = sys_comm_req_stop_meas(send_buf);
    sys_comm_send_write_length(send_buf, len);
    sys_comm_send_write_slave_id(send_buf, DEVICE_ID);

    /* Send req */
    hw.send(send_buf, len); 
}


void adapter_sys_comm_send_req_get_data(void)
{
    /* Write req data to buffer */
    req_resp_length_t len = sys_comm_req_get_data(send_buf);
    sys_comm_send_write_length(send_buf, len);
    sys_comm_send_write_slave_id(send_buf, DEVICE_ID);

    /* Send req */
    hw.send(send_buf, len); 
}