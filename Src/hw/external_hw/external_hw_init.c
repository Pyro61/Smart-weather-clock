#include "external_hw_init.h"
#include "LPS25HB/LPS25HB.h"
#include "sht20/sht20.h"
#include "HC05/HC05.h"
#include "buzzer/buzzer.h"
#include "HD44780/HD44780.h"
#include "buttons/buttons.h"

void external_hw_init(void)
{
    LPS25HB_init();
    sht20_init();
    HC05_init();
    buzzer_init();
    HD44780_init();
    buttons_init();
}