#include "uart.h"
#include "../gpio/gpio.h"
#include "stm32g4xx.h"
#include "../../../safe_state/safe_state.h"
#include <stdlib.h>


/********************************************************************************************/
/*                                          UART4                                           */
/********************************************************************************************/

/* Callback function pointer */
static cb_t uart4_callback;

/* Buffer pointer */
static uint8_t *uart4_buf_ptr;

/* Buffer start address, used to come back to start adddress after receving all bytes (before next message) */
static uint8_t *uart4_buf_start;

/* Clock frequency, set in core_clock submodule */
extern uint32_t clock_freq;


void uart4_init(void)
{
    /* GPIO clock enable and set pins
	 * PC10 - TX, PC11 - RX */
    if (gpio_config(PORT_C, 10, ALTERNATE_FUN, PUSH_PULL, NO_PULL, SPEED_LOW) == ERR) safe_state();
    if (gpio_set_af(PORT_C, 10, 5) == ERR) safe_state();
    if (gpio_config(PORT_C, 11, ALTERNATE_FUN, PUSH_PULL, NO_PULL, SPEED_LOW) == ERR) safe_state();
    if (gpio_set_af(PORT_C, 11, 5) == ERR) safe_state();


	/* Clock enable on UART4
	 * 9600 baudrate, 8n1, 16x oversampling, interrupt on received data, disable overrun detection */
	RCC -> APB1ENR1 |= RCC_APB1ENR1_UART4EN;
	UART4 -> BRR = clock_freq / 9600;
	UART4 -> CR1 |= USART_CR1_RXNEIE_RXFNEIE;
	UART4 -> CR3 |= USART_CR3_OVRDIS;
	NVIC_EnableIRQ(UART4_IRQn);

	/* DMA settings */
	RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	/* Transmit
	* Periph addr, priority, memory and periph sizes, transfer direction, mem inc mode, transfer complete and error IRQ, DMA request source */
	DMA1_Channel4 -> CPAR = (uint32_t)&(UART4 -> TDR);
	DMA1_Channel4 -> CCR |= DMA_CCR_PL_0 | DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_TEIE | DMA_CCR_TCIE;
	DMAMUX1_Channel3 -> CCR |= 31;

    /* UART reception timer settings (measure time between received bytes, if timer (3ms) turns then this is assumed all bytes are received) */
    /* Prescaler and auto-reload register values dependent on clock freq and measurement delay */
	uint32_t ms_presc = clock_freq / 1000;
	uint32_t ms_arr = 3;
	/* Enable TIM16 clock, set prescaler, counted value and preload */
	RCC -> APB2ENR |= RCC_APB2ENR_TIM16EN;
	TIM16 -> CR1 &= ~(TIM_CR1_DIR);
	TIM16 -> PSC = ms_presc - 1;
	TIM16 -> ARR = ms_arr - 1;
	TIM16 -> CR1 |= TIM_CR1_ARPE;

	/* Update registers and clear irq flag */
	TIM16 -> EGR = TIM_EGR_UG;
	TIM16 -> SR &= ~(TIM_SR_UIF);

	/* Enable counter overflow interrupt */
	TIM16-> DIER |= TIM_DIER_UIE;
	NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);
	NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 2);
	NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

	/* Enable UART */
	UART4 -> CR1 |= (USART_CR1_UE);
}


void uart4_send(uint8_t *buf, uint8_t len)
{
    /* Set transmitted data size and buffer */
	UART4 -> CR3 |= USART_CR3_DMAT;
	DMA1_Channel4 -> CNDTR = len;
	DMA1_Channel4 -> CMAR = (uint32_t)buf;

	/* Start transmission */
	DMA1_Channel4 -> CCR |= DMA_CCR_EN;
	UART4 -> CR1 |= USART_CR1_TE;
}


void uart4_recv(uint8_t *buf, uint8_t len, cb_t cb)
{
    /* Set global variables */
	uart4_buf_start = buf;
    uart4_buf_ptr = uart4_buf_start;
    uart4_callback = cb;
    (void)len;

    /* Enable receiving */
    UART4 -> CR1 |= USART_CR1_RE;
}


static void tim_uart_reset(void)
{
	/* Reset timer and start counting */
	TIM16 -> CNT = 0;
	TIM16 -> CR1 |= TIM_CR1_CEN;
}


/* Interrupts */
/* Transmission */
void DMA1_CH4_IRQHandler(void)
{
	/* UART4 TX DMA transmission end */
	if (DMA1 -> ISR & DMA_ISR_TCIF4)
	{
		/* Delete half and full complete transfer flags */
		DMA1 -> IFCR = DMA_IFCR_CTCIF4 | DMA_IFCR_CHTIF4;

		/* Disable DMA CH4 */
		DMA1_Channel4 -> CCR &= ~DMA_CCR_EN;

		/* Disable UART4 TX DMA mode */
		UART4 -> CR3 &= USART_CR3_DMAT;
	}
}


/* Reception */
void UART4_IRQHandler(void)
{
	/* Received data */
	if (UART4 -> ISR & USART_ISR_RXNE_RXFNE)
	{
		tim_uart_reset();
		*(uart4_buf_ptr++) = UART4 -> RDR;
	}
}


/* Reception timer */
void TIM1_UP_TIM16_IRQHandler(void)
{
	/* All message received */
	if (TIM16 -> SR & TIM_SR_UIF)
	{
        /* Stop timer */
		TIM16 -> SR &= ~(TIM_SR_UIF);
		TIM16 -> CR1 &= ~TIM_CR1_CEN;

		/* Reset actual buffer index */
		uart4_buf_ptr = uart4_buf_start;

	/* All message received callback */
    if (uart4_callback != NULL)
    {
        uart4_callback();
    }
	}
}