#include "fpu.h"
#include "stm32g4xx.h"


void fpu_init(void)
{
    /* Enable Coprocessors CP10 an CP11 in privileged and unprivileged mode (full access) */
    SCB -> CPACR = 0x0F << 20;

    /* Make sure that changes got applied */
    __DSB();
    __ISB();
}