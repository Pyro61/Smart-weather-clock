#include "core_clock.h"
#include "stm32g4xx.h"


/* Current clock freq */
uint32_t clock_freq = 12000000;


/* HSE - 48MHz */
void core_clock_config(void)
{
    /* Flash access delay */
	FLASH -> ACR |= FLASH_ACR_LATENCY_0;
	while (!(FLASH -> ACR & FLASH_ACR_LATENCY));

	/* Enable HSE clock */
	RCC -> CR |= RCC_CR_HSEON;
	while (!(RCC -> CR & RCC_CR_HSERDY));

	/* Select SYSCLK clock - HSE */
	RCC -> CFGR |= RCC_CFGR_SW_0;

	/* Disable HSI */
	RCC -> CR &= ~RCC_CR_HSION;
	clock_freq = 48000000;
}