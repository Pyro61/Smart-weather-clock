#include "HC05.h"
#include "../../mcu_hw/uart/uart.h"


void HC05_init(void)
{
    /* Send anything to slave to prevent receiving zeros by slave device until first reception */
    uint8_t tmp = 1;
    uart4_send(&tmp, sizeof(tmp));
}


void HC05_send(uint8_t *buf, uint8_t len)
{
    uart4_send(buf, len);
}


void HC05_recv(uint8_t *buf, uint8_t len, cb_t cb)
{
    uart4_recv(buf, len, cb);
}


struct sys_comm_hw_funs HC05_funs = {HC05_init, HC05_send, HC05_recv};


struct sys_comm_hw_funs *HC05_get_funs(void)
{
    return &HC05_funs;
}

