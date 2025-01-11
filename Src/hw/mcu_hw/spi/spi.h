#pragma once

#include <stdint.h>
#include <stdbool.h>


typedef void (*cb_t)(void);

void spi1_init(void);
void spi1_write_polling(uint8_t reg, uint8_t *data, uint8_t bytes_num);
void spi1_read_dma(uint8_t reg, uint8_t *buf, uint8_t bytes_num, cb_t cb);
bool is_spi1_bus_free(void);