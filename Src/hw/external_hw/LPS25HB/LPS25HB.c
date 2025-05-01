#include "LPS25HB.h"
#include "LPS25HB_defines.h"
#include "../../mcu_hw/gpio/gpio.h"
#include "../../../safe_state/safe_state.h"
#include "../../mcu_hw/i2c/i2c.h"
#include "../../mcu_hw/tim/tim.h"
#include "../../../events/events.h"
#include <stdlib.h>
#include <stdbool.h>

/* Auto increment of register addresses */
#define REG_AUTO_INC_BIT        7   /* Auto increment of register address in multiple read/write operations if MSB (7th) bit of register is 1, 
                                     * otherwise operation is done on one register */

/* Macro to change register address to do multi read/write operation */
#define MULTI_READ_WRITE(x)     (x | (1 << REG_AUTO_INC_BIT))   /* Sets multiple read/write operation, 
                                                                 * default state - 1 register operation 
                                                                 * (all registers have 7th bit set to 0 by default) */

/* Data size */
#define P_DATA_SIZE             3
#define T_DATA_SIZE             2
#define DATA_SIZE               (P_DATA_SIZE + T_DATA_SIZE)


/* Data buffer */
static uint8_t data_buf[DATA_SIZE];


/* Helper function declarations */
static void LPS25HB_write_1B(enum LPS25HB_REG_ADDR reg, uint8_t value);
static void LPS25HB_read(enum LPS25HB_REG_ADDR reg, uint8_t *buf, uint8_t size);
static void LPS25HB_is_communication_working(void);
static void LPS25HB_write_config(void);
static void LPS25HB_read_data(void);


/* Module functions */
void LPS25HB_start_measuring(void)
{
    LPS25HB_write_1B(LPS25HB_REG_ADDR_CTRL_2, ONE_SHOT_EN);
}


void LPS25HB_init(void)
{
    /* Configure GPIO Pins */
    /* PA12 - IRQ */
    if (gpio_config(PORT_A, 12, INPUT, PUSH_PULL, NO_PULL, SPEED_LOW) == ERR) safe_state();
    if (gpio_set_exti(PORT_A, 12, RISING_EDGE, LPS25HB_read_data) == ERR) safe_state();

    /* Wait for device power up */
    delay_ms(10);

    /* Check if device communication is working */
    LPS25HB_is_communication_working();

    /* Configuration */
    LPS25HB_write_config();

    /* Subsribe measure function to measurement delay event */
    if (events_subscribe(LPS25HB_start_measuring, EVENT_MEAS_DELAY_ELAPSED) != SUBSCRIBE_SUCCESS) safe_state();
}


int32_t LPS25HB_get_temp(void)
{
    /* View datasheet */
    int16_t tmp = (data_buf[3] | (data_buf[4] << 8));

    float temp = 42.5f + (float)tmp / 480.0f;

    return (uint32_t)temp;
}

int32_t LPS25HB_get_pressure(void)
{
    /* View datasheet */
    int32_t tmp = (data_buf[0] | (data_buf[1] << 8) | data_buf[2] << 16);

    return tmp / 4096;
}


/* Helper functions */
static void LPS25HB_write_1B(enum LPS25HB_REG_ADDR reg, uint8_t value)
{
    /* 1 bit write operation */
    while (!(is_i2c1_bus_free()));
    i2c1_write_polling(LPS25HB_ADDRESS, reg, &value, 1);
}


static void LPS25HB_read(enum LPS25HB_REG_ADDR reg, uint8_t *buf, uint8_t size)
{
    /* Check if read will be done on more than 1 register */
    if (size > 1)
    {
        /* Set multiple read/write bit */
        reg = MULTI_READ_WRITE(reg);
    }

    /* Read operation */
    while (!(is_i2c1_bus_free()));
    i2c1_read_dma(LPS25HB_ADDRESS, reg, buf, size, NULL);
}


static void LPS25HB_is_communication_working(void)
{
    uint8_t tmp;
    LPS25HB_read(LPS25HB_REG_ADDR_WHO_AM_I, &tmp, 1);
    while (tmp != LPS25HB_DEVICE_ID);
}


static void LPS25HB_write_config(void)
{
    LPS25HB_write_1B(LPS25HB_REG_ADDR_CTRL_4, DATA_READY_EN);
    LPS25HB_write_1B(LPS25HB_REG_ADDR_CTRL_1, POWER_UP_DEVICE | BLOCK_DATA_UPDATE | IRQ_EN);
}


static void LPS25HB_read_data(void)
{
    LPS25HB_read(LPS25HB_REG_ADDR_OUT_P_XL, data_buf, DATA_SIZE);
}