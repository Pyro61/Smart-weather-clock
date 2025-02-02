#include "aht15.h"
#include "../../mcu_hw/i2c/i2c.h"
#include "../../mcu_hw/tim/tim.h"
#include "stdlib.h"

/* Device address */
#define AHT15_ADDR                  0x70

/* Commands */
#define AHT15_SOFT_RESET            0xBA
#define AHT15_INIT                  0xE1
#define AHT15_MEASURE_BYTE_1        0xAC
#define AHT15_MEASURE_BYTE_2        0x33
#define AHT15_MEASURE_BYTE_3        0x00
#define AHT15_MEASURE_CMD_SIZE      3
#define AHT15_CMD_SIZE              1

/* Delays */
#define AHT15_POWER_UP_DELAY_MS     20
#define AHT15_SOFT_RESET_DELAY_MS   20
#define AHT15_MEASURE_TIME_MS       80

/* Data size */
#define DATA_SIZE                   6 /* Temp 2.5 bytes + Press 2.5 bytes + Status 1 byte */

/* Calculate humidity and temperature constants */
#define AHT15_CALC_DIVIDER          (1 << 20)
#define AHT15_H_CALC_MULTIPLIER     100
#define AHT15_T_CALC_MULTIPLIER     200
#define AHT15_T_CALC_SUBTRACTOR     50 


/* Data buffer */
static uint8_t data_buf[DATA_SIZE];

/* Trigger measurement command */
static const uint8_t trigger_measurement_cmd[AHT15_MEASURE_CMD_SIZE] = {AHT15_MEASURE_BYTE_1, AHT15_MEASURE_BYTE_2, AHT15_MEASURE_BYTE_3};


/* Helper function definitions */
static void aht15_write(const uint8_t *cmd, uint8_t size);


void aht15_read_data(void)
{
    while (!is_i2c2_bus_free());
    i2c2_read_dma(AHT15_ADDR, data_buf, DATA_SIZE, NULL);
}


static void aht15_soft_reset(void)
{
    uint8_t tmp = AHT15_SOFT_RESET;
    aht15_write(&tmp, AHT15_CMD_SIZE);
}


void aht15_start_measuring(void)
{
    aht15_write(&trigger_measurement_cmd[0], AHT15_MEASURE_CMD_SIZE);
    //timer 80ms and read data
}


void aht15_init(void)
{
    delay_ms(AHT15_POWER_UP_DELAY_MS);
    aht15_soft_reset();
    delay_ms(AHT15_SOFT_RESET_DELAY_MS);
}


int32_t aht15_get_humidity(void)
{
    /* View datasheet */
    int32_t tmp = (data_buf[1] << 12) | (data_buf[2] << 4) | (data_buf[3] >> 4);

    return (tmp * AHT15_H_CALC_MULTIPLIER) / AHT15_CALC_DIVIDER;
}

int32_t aht15_get_temp(void)
{
    /* View datasheet */
    int32_t tmp = ((data_buf[3] & 0x0F) << 16) | (data_buf[4] << 8) | (data_buf[5]);

    return (tmp * AHT15_T_CALC_MULTIPLIER) / AHT15_CALC_DIVIDER - AHT15_T_CALC_SUBTRACTOR;
}


/* Helper functions */
static void aht15_write(const uint8_t *cmd, uint8_t size)
{
    while (!is_i2c2_bus_free());
    i2c2_write_polling(AHT15_ADDR, cmd, size); /* AHT15 don't have registers, so only write commands instead of registers and data */
}