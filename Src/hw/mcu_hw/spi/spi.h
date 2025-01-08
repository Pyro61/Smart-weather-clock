#pragma once

#include <stdint.h>

enum spi_state
{
    SPI_ERR = -1,
    SPI_OK = 0,
    SPI_BUSY = 1
};

void spi1_init(void);
enum spi_state spi1_write_polling(uint8_t reg, uint8_t *data, uint8_t bytes_num);