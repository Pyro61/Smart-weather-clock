#pragma once

#include "../types.h"


/* Protocol data indexes */
#define SYS_COMM_SLAVE_ID_IDX       0
#define SYS_COMM_MSG_LENGTH_IDX     1
#define SYS_COMM_APP_LAYER_OFFSET   2
#define SYS_COMM_CMD_IDX            SYS_COMM_APP_LAYER_OFFSET
#define SYS_COMM_DATA_START_IDX     (SYS_COMM_APP_LAYER_OFFSET + 1)

/* Buffer size defines */
#define SYS_COMM_DATA_SIZE          (sizeof(struct weather_data))
#define SYS_COMM_REQ_RESP_SIZE      (SYS_COMM_APP_LAYER_OFFSET + 1) /* Includes 1 byte for req/resp cmd and 2 bytes for slave and msg length */
#define SYS_COMM_BUF_SIZE           (SYS_COMM_DATA_SIZE + SYS_COMM_REQ_RESP_SIZE)

/* Response command value addition */
#define SYS_COMM_RESP_CMD_ADD       0x40

/* Quantity of requests/responses */
#define SYS_COMM_REQ_NUM            3
#define SYS_COMM_RESP_NUM           1

/* Typedefs */
typedef uint8_t *buf_t;
typedef uint8_t slave_id_t;
typedef uint8_t req_resp_length_t;

/* Request commands (master only) */
enum sys_comm_req_cmd
{
    REQ_ID_START = 1,
    REQ_START_MEASURING = REQ_ID_START,
    REQ_STOP_MEASURING = 2,
    REQ_ID_END,
    REQ_GET_DATA = REQ_ID_END,
};

/* Response commands (slave only) */
enum sys_comm_resp_cmd
{
    RESP_ID_START = SYS_COMM_RESP_CMD_ADD + REQ_GET_DATA,
    RESP_GET_DATA = RESP_ID_START,
    RESP_ID_END = RESP_ID_START,
};

/* Receive error codes */
enum sys_comm_recv_err
{
    ERR_NONE = 0,
    ERR_SLAVE_ID = -1,
    ERR_REQ = -2,
    ERR_RESP = -3,
};