#pragma once

#include <stdint.h>


/********************************************************************************************/
/*                                      TIM6 - delay                                        */
/********************************************************************************************/
void tim_delay_init(void);
void delay_ms(uint32_t delay);


/********************************************************************************************/
/*                                    TIM7 - periodic                                       */
/********************************************************************************************/
void tim_periodic_init(void);
void tim_periodic_start(void);
void tim_periodic_stop(void);
void tim_periodic_generate_update(void);
