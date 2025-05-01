#pragma once

#include <stdbool.h>
#include <stdint.h>


typedef void (*cb_t)(void);

/********************************************************************************************/
/*                                           I2C1                                           */
/********************************************************************************************/

void i2c1_init(void);
void i2c1_write_polling(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t size);
void i2c1_read_polling(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t size);
void i2c1_read_dma(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t size, cb_t cb);
bool is_i2c1_bus_free(void);


/********************************************************************************************/
/*                                           I2C2                                           */
/********************************************************************************************/

void i2c2_init(void);
void i2c2_write_polling(const uint8_t addr, const uint8_t *data, const uint8_t size);
void i2c2_read_polling(const uint8_t addr, uint8_t *buf, const uint8_t size);
void i2c2_read_dma(const uint8_t addr, uint8_t *buf, const uint8_t size, const cb_t cb);
bool is_i2c2_bus_free(void);