#include "spi.h"
#include "stm32g4xx.h"
#include "../gpio/gpio.h"
#include "../../../safe_state/safe_state.h"



void spi1_init(void)
{
    /* Enable SPI clock */
	RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN;

    /* Spi configuration 
     * 8 bit data size, Motorola format */
	SPI1 -> CR1 |= 0x04 << 3; /* Baudrate - freq / 32 = 1,5 MHz */
	SPI1 -> CR1 |= SPI_CR1_MSTR; /* Master mode */
	SPI1 -> CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI); /* Software SS */

	/* GPIO config - PA5 - SCK, PA6 - MISO, PA7 - MOSI */
	if (gpio_config(PORT_A, 5, ALTERNATE_FUN, PUSH_PULL, NO_PULL, SPEED_VERY_HIGH) == ERR) safe_state();
    if (gpio_set_af(PORT_A, 5, 5) == ERR) safe_state();
    if (gpio_config(PORT_A, 6, ALTERNATE_FUN, PUSH_PULL, NO_PULL, SPEED_VERY_HIGH) == ERR) safe_state();
    if (gpio_set_af(PORT_A, 6, 5) == ERR) safe_state();
    if (gpio_config(PORT_A, 7, ALTERNATE_FUN, PUSH_PULL, NO_PULL, SPEED_VERY_HIGH) == ERR) safe_state();
    if (gpio_set_af(PORT_A, 7, 5) == ERR) safe_state();

    /* DMA config */
    /* Enable DMA1 and DMAMUX clocks */
    RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
    /* Receive
	 * Periph addr, priority, memory and periph sizes, mem inc mode, transfer complete and error IRQ, DMA request source */
	DMA1_Channel1 -> CPAR = (uint32_t)&(SPI1 -> DR);
	DMA1_Channel1 -> CCR |= DMA_CCR_PL_0 | DMA_CCR_MINC | DMA_CCR_TEIE | DMA_CCR_TCIE;
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    DMAMUX1_Channel0 -> CCR |= 0x0A << 0;

	/* Enable SPI */
	SPI1 -> CR1 |= SPI_CR1_SPE;
}


enum spi_state spi1_write_polling(uint8_t reg, uint8_t *data, uint8_t bytes_num)
{
    /* Check if line is busy */
    if (SPI1 -> SR & SPI_SR_BSY)
    {
        return SPI_BUSY;
    }

    uint8_t i;
    /* Send register address to slave */
    *(volatile uint8_t *)&SPI1 -> DR = reg;
    
    /* Send data */
    for (i = 0; i < bytes_num; i++)
    {
        while (!(SPI1 -> SR & SPI_SR_TXE)); /* Wait for transmit buffer to be not full */
        *(volatile uint8_t *)&SPI1 -> DR = data[i];
    }
    return SPI_OK;
}


