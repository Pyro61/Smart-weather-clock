#pragma once

#include <stdint.h>


/* Typedefs */
typedef uint32_t ms_t;
typedef void (*cb_t)(void);


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


/********************************************************************************************/
/*                         TIM15 - time measurement non-blocking                            */
/********************************************************************************************/
void tim_meas_no_block_init(void);
void tim_meas_no_block_start(ms_t time, cb_t cb);
