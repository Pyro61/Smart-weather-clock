#include "iwdg.h"
#include "stm32g4xx.h"


/* Key register commands */
#define IWDG_ENABLE             0xCCCC
#define IWDG_REG_ACCESS         0x5555
#define IWDG_FEED_CNT           0xAAAA

/* Prescaler values */
#define IWDG_PRESCALER_X4       0
#define IWDG_PRESCALER_X8       1
#define IWDG_PRESCALER_X16      2
#define IWDG_PRESCALER_X32      3
#define IWDG_PRESCALER_X64      4
#define IWDG_PRESCALER_X128     5
#define IWDG_PRESCALER_X256     6

/* Reload register max value (12 bits) */
#define IWDG_MAX_RELOAD         0xFFF

/* Calculate seconds to miliseconds */
#define S_TO_MS(ms)		(ms * 1000)

/* Clock frequency, set in core_clock submodule */
extern uint32_t clock_freq;


void iwdg_init(void)
{
    /* Enable watchdog */
    IWDG -> KR = IWDG_ENABLE;
    /* Get access to registers */
    IWDG -> KR = IWDG_REG_ACCESS;
    /* Set prescaler */
    IWDG -> PR = IWDG_PRESCALER_X256;
    /* Set reload value */
    IWDG -> RLR = IWDG_MAX_RELOAD;

    /* Wait for registers update */
    while (IWDG -> SR != 0);
}


void iwdg_feed_counter(void)
{
    IWDG -> KR = IWDG_FEED_CNT;
}


/* Generates irq every 10s */
void iwdg_enable_irq_generator(void)
{
    /* Prescaler value dependent on clock frequency */
	uint32_t ms_presc = clock_freq / 1000;
    uint32_t ms_arr = S_TO_MS(10);

	/* Enable TIM20 clock, set prescaler, counted value and preload */
	RCC -> APB2ENR |= RCC_APB2ENR_TIM20EN;
	TIM20 -> PSC = ms_presc - 1;
	TIM20 -> ARR = ms_arr - 1;
	TIM20 -> CR1 |= TIM_CR1_ARPE;

	/* Update registers and clear irq flag */
	TIM20 -> EGR = TIM_EGR_UG;
	TIM20 -> SR &= ~(TIM_SR_UIF);
	
	/* Enable counter overflow interrupt */
	TIM20 -> DIER |= TIM_DIER_UIE;
	NVIC_ClearPendingIRQ(TIM20_UP_IRQn);
	NVIC_SetPriority(TIM20_UP_IRQn, 1);
	NVIC_EnableIRQ(TIM20_UP_IRQn);
    
    TIM20 -> CR1 |= TIM_CR1_CEN;
}


/* Irq generator interrupt */
void TIM20_UP_IRQHandler(void)
{
	if (TIM20 -> SR & TIM_SR_UIF)
	{
        /* Clear irq flag */
		TIM20 -> SR &= ~(TIM_SR_UIF);
	}
}