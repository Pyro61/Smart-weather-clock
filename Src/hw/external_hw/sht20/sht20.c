#include "sht20.h"
#include "../../mcu_hw/i2c/i2c.h"
#include "../../mcu_hw/tim/tim.h"
#include "../../../events/events.h"
#include "../../../safe_state/safe_state.h"
#include <stdlib.h>

/* Device address (i2c address) */
#define SHT20_ADDRESS                   0x80

/* Data (up to 14 bits) + Status 2 bits + CRC 8 bits */
#define DATA_SIZE                       3

/* Status bits */
#define STATUS_REG_CURRENT_MEAS_MASK    0x02
#define STATUS_REG_CURRENT_MEAS_TEMP    0
#define STATUS_REG_CURRENT_MEAS_HUM     1

/* Power up delay */
#define SHT20_POWER_UP_DELAY_MS         15

enum SHT20_COMMAND
{
    SHT20_CMD_MEAS_T_HOLD = 0xE3,
    SHT20_CMD_MEAS_T_NO_HOLD = 0xF3,
    SHT20_CMD_MEAS_H_HOLD = 0xE5,
    SHT20_CMD_MEAS_H_NO_HOLD = 0xF5,
    SHT20_CMD_READ_USER_REG = 0xE6,
    SHT20_CMD_WRITE_USER_REG = 0xE7,
    SHT20_CMD_SOFT_RESET = 0xFE,
};

enum SHT20_DATA_LSB_BIT_MASK
{
    RESOLUTION_8_BIT_MASK = 0x00,
    RESOLUTION_10_BIT_MASK = 0xC0,
    RESOLUTION_11_BIT_MASK = 0xE0,
    RESOLUTION_12_BIT_MASK = 0xF0,
    RESOLUTION_13_BIT_MASK = 0xF8,
    RESOLUTION_14_BIT_MASK = 0xFC,
};

static enum SHT20_DATA_LSB_BIT_MASK temp_lsb_bit_mask = RESOLUTION_14_BIT_MASK; /* Default opction */
static enum SHT20_DATA_LSB_BIT_MASK humidity_lsb_bit_mask = RESOLUTION_12_BIT_MASK; /* Default option */

/* Data buffers */
static uint8_t humidity_data_buf[DATA_SIZE];
static uint8_t temp_data_buf[DATA_SIZE];


/* Support functions declarations */
static void sht20_send_cmd(enum SHT20_COMMAND cmd);
static void sht20_write(const uint8_t *data, const uint8_t size);
static void sht20_read_hum(void);
static void sht20_read_temp(void);
static void sht20_measure_2nd_part(void);


/* API functions */
void sht20_init(void)
{
    if (events_subscribe(sht20_start_measuring, EVENT_MEAS_DELAY_ELAPSED) != SUBSCRIBE_SUCCESS) safe_state();
    delay_ms(SHT20_POWER_UP_DELAY_MS);
}


int32_t sht20_get_temp(void)
{
    uint32_t raw_value = (temp_data_buf[0] << 8) | (temp_data_buf[1] & (uint32_t)temp_lsb_bit_mask);
    return (int32_t)(-46.85f + ((175.72f * raw_value) / 65536.0f));
}


int32_t sht20_get_humidity(void)
{
    uint32_t raw_value = (humidity_data_buf[0]  << 8) | (humidity_data_buf[1] & (uint32_t)humidity_lsb_bit_mask);
    return -6 + ((125 * (int32_t)raw_value) >> 16);
}


void sht20_start_measuring(void)
{
    sht20_send_cmd(SHT20_CMD_MEAS_H_HOLD);
    sht20_read_hum();
}


/* Support functions */
static void sht20_send_cmd(enum SHT20_COMMAND cmd)
{
    uint8_t tmp = (uint8_t)cmd;
    sht20_write(&tmp, 1); /* SHT20 don't have registers, so only write commands instead of registers and data */
}


static void sht20_write(const uint8_t *data, const uint8_t size)
{
    while (!is_i2c2_bus_free());
    i2c2_write_polling(SHT20_ADDRESS, data, size);
}


static void sht20_read_hum(void)
{
    while (!is_i2c2_bus_free());
    i2c2_read_dma(SHT20_ADDRESS, humidity_data_buf, DATA_SIZE, sht20_measure_2nd_part);
}


static void sht20_read_temp(void)
{
    while (!is_i2c2_bus_free());
    i2c2_read_dma(SHT20_ADDRESS, temp_data_buf, DATA_SIZE, NULL);
}


static void sht20_measure_2nd_part(void)
{
    sht20_send_cmd(SHT20_CMD_MEAS_T_HOLD);
    sht20_read_temp();
}


