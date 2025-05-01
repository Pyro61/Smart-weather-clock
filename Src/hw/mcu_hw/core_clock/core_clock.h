#pragma once

#include "stdint.h"


/* Current clock frequency */
extern uint32_t clock_freq;

/* Core clock config - PPL (HSI16) 48Mhz */
void core_clock_config(void);