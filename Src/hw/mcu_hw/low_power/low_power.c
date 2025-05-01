#include "low_power.h"
#include "stm32g4xx.h"


/* Entering sleep mode to keep peripherals working normally */
void sleep(void)
{
    __WFI();
}