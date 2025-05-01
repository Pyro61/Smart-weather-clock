#include "core_clock.h"
#include "stm32g4xx.h"


/* Current clock freq */
uint32_t clock_freq = 16000000;


/* HSI16 - 48MHz */
void core_clock_config(void)
{
    /* Flash access delay */
	FLASH->ACR &= ~FLASH_ACR_LATENCY;    /* Reset current latency setting */
    FLASH->ACR |= FLASH_ACR_LATENCY_1WS; /* Set required wait states - 1WS */
	while ((FLASH -> ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_1WS); /* Wait for register update */

	/* Disable HSI16 in low power modes */
	RCC -> CR &= ~RCC_CR_HSIKERON;

	/* Configure PLL 
	 * Output frequency = entry clock * (PLLN/PLLM) / PLLR */
	RCC -> PLLCFGR = RCC_PLLCFGR_PLLSRC_1; /* Select HSI16 as entry clock source */
	RCC -> PLLCFGR |= 12 << 8; /* Main PLL multiplicator factor - PLLN = 12 */
	RCC -> PLLCFGR |= 0 << 4; /* Main PLL division factor - PLLM = 1 */
	RCC -> PLLCFGR |= 1 << 25; /* PLL "R" clock division factor - PLLR = 4 */
	RCC -> PLLCFGR |= RCC_PLLCFGR_PLLREN; /* Enable PLL "R" clock */
	RCC -> CR |= RCC_CR_PLLON; /* Enable PLL */
	while (!(RCC->CR & RCC_CR_PLLRDY)); /* Wait for PLL to be ready */

	/* Select system clock - PLL */
	RCC -> CFGR |= RCC_CFGR_SW;
	/* Wait for switch to PLL */
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	clock_freq = 48000000;
}