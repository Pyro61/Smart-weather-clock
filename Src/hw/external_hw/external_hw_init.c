#include "external_hw_init.h"
#include "LPS25HB/LPS25HB.h"

void external_hw_init(void)
{
    LPS25HB_init();
}