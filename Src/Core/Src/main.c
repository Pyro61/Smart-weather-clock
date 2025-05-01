#include "../Inc/main.h"
#include "../../hw/mcu_hw/mcu_hw_init.h"
#include "../../hw/external_hw/external_hw_init.h"
#include "../../logic_init.h"
#include "../../hw/mcu_hw/iwdg/iwdg.h"


int main(void)
{
    /* Hw initialisation */
    mcu_hw_init();
    external_hw_init();

    /* Application logic initialisation */
    logic_init();
    
    /* Loop forever */
	while (1)
    {
        iwdg_feed_counter();
    }
}
