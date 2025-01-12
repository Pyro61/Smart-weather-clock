#include "tim.h"
#include "stm32g4xx.h"
#include "../../../events/events.h"


/* Calculate seconds to miliseconds */
#define S_TO_MS(ms)		(ms * 1000)

/* Clock frequency, set in core_clock submodule */
extern uint32_t clock_freq;

/* Used in delay_ms function */
static uint32_t ms_elapsed = 0;


void tim_delay_init(void)
{
	/* Prescaler value dependent on clock frequency */
	uint32_t ms_presc = clock_freq / 1000;

	/* Enable TIM6 clock, set prescaler, counted value and preload */
	RCC -> APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
	TIM6 -> PSC = ms_presc - 1;
	TIM6 -> ARR = 1;
	TIM6 -> CR1 |= TIM_CR1_ARPE;

	/* Update registers and clear irq flag */
	TIM6 -> EGR = TIM_EGR_UG;
	TIM6 -> SR &= ~(TIM_SR_UIF);
	
	/* Enable counter overflow interrupt */
	TIM6-> DIER |= TIM_DIER_UIE;
	NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
	NVIC_SetPriority(TIM6_DAC_IRQn, 9);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
}


void tim_periodic_init(void)
{
	/* Prescaler and auto-reload register values dependent on clock freq and measurement delay */
	uint32_t ms_presc = clock_freq / 1000;
	uint32_t ms_arr = S_TO_MS(MEASUREMENT_S_DELAY);

	/* Enable TIM7 clock, set prescaler, counted value and preload */
	RCC -> APB1ENR1 |= RCC_APB1ENR1_TIM7EN;
	TIM7 -> CR1 &= ~(TIM_CR1_DIR);
	TIM7 -> PSC = ms_presc - 1;
	TIM7 -> ARR = ms_arr - 1;
	TIM7 -> CR1 |= TIM_CR1_ARPE;
	
	/* Update registers and clear irq flag */
	TIM7 -> EGR = TIM_EGR_UG;
	TIM7 -> SR &= ~(TIM_SR_UIF);
	
	/* Enable counter overflow interrupt */
	TIM7-> DIER |= TIM_DIER_UIE;
	NVIC_ClearPendingIRQ(TIM7_DAC_IRQn);
	NVIC_SetPriority(TIM7_DAC_IRQn, 8);
	NVIC_EnableIRQ(TIM7_DAC_IRQn);
}


void delay_ms(uint32_t delay)
{
	ms_elapsed = 0;

	/* Enable counter */
	TIM6 -> CR1 |= TIM_CR1_CEN;

	while(delay > ms_elapsed);

	/* Disable counter */
	TIM6 -> CR1 &= ~TIM_CR1_CEN;
}


void tim_periodic_start(void)
{
	/* Reset counter and start counting */
	TIM7 -> CNT = 0;
	TIM7 -> CR1 |= TIM_CR1_CEN;
}


void tim_periodic_stop(void)
{
	/* Stop counting */
	TIM7 -> CR1 &= ~TIM_CR1_CEN;
}


void tim_periodic_generate_update(void)
{
	/* Generate update */
	TIM7 -> EGR = TIM_EGR_UG;
}


/* Interrupts */
/* Delay - 1ms elapsed */
void TIM6_DACUNDER_IRQHandler(void)
{
	if (TIM6 -> SR & TIM_SR_UIF)
	{
		TIM6 -> SR &= ~(TIM_SR_UIF);
		ms_elapsed++;
	}
}


/* Periodic - set time elapsed */
void TIM7_IRQHandler(void)
{
	if (TIM7 -> SR & TIM_SR_UIF)
	{
		TIM7 -> SR &= ~(TIM_SR_UIF);
		events_notify_subscribers(EVENT_MEAS_DELAY_ELAPSED);
	}
}