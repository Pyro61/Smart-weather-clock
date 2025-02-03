#include "external_hw_init.h"
#include "LPS25HB/LPS25HB.h"
#include "aht15/aht15.h"
#include "HC05/HC05.h"

void external_hw_init(void)
{
    LPS25HB_init();
    aht15_init();
    HC05_init();
}