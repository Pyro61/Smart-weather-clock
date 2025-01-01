#include "mcu_hw_init.h"

#include "core_clock/core_clock.h"
#include "fpu/fpu.h"


void mcu_hw_init(void)
{
    core_clock_config();
    fpu_init();
}