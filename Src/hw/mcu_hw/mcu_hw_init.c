#include "mcu_hw_init.h"

#include "core_clock/core_clock.h"
#include "iwdg/iwdg.h"
#include "fpu/fpu.h"
#include "spi/spi.h"
#include "tim/tim.h"
#include "i2c/i2c.h"
#include "uart/uart.h"
#include "rtc/rtc.h"

void mcu_hw_init(void)
{
    core_clock_config();
    iwdg_init();
    iwdg_enable_irq_generator();
    fpu_init();
    spi1_init();
    tim_delay_init();
    tim_periodic_init();
    tim_meas_no_block_init();
    i2c1_init();
    i2c2_init();
    uart4_init();
    rtc_init();
}