#include "HD44780.h"
#include "../../mcu_hw/gpio/gpio.h"
#include "../../../safe_state/safe_state.h"
#include "../../mcu_hw/tim/tim.h"
#include <stdint.h>
#include <stdbool.h>


/* GPIO port and pins */
#define HD44780_PORT    PORT_B
#define HD44780_PIN_RS  12
#define HD44780_PIN_RW  11
#define HD44780_PIN_E   2
#define HD44780_PIN_D4  1
#define HD44780_PIN_D5  15
#define HD44780_PIN_D6  14
#define HD44780_PIN_D7  13

/* Display size */
#define DISPLAY_ROWS    4
#define DISPLAY_COLS    20

/* Control pins enable/disable */
#define RS_HIGH()       gpio_output_write(HD44780_PORT, HD44780_PIN_RS, HIGH)
#define RS_LOW()        gpio_output_write(HD44780_PORT, HD44780_PIN_RS, LOW)
#define RW_HIGH()       gpio_output_write(HD44780_PORT, HD44780_PIN_RW, HIGH)
#define RW_LOW()        gpio_output_write(HD44780_PORT, HD44780_PIN_RW, LOW)
#define E_HIGH()        gpio_output_write(HD44780_PORT, HD44780_PIN_E, HIGH)
#define E_LOW()         gpio_output_write(HD44780_PORT, HD44780_PIN_E, LOW)

/* Commands */
#define CMD_CLEAR       0x01


/* Rows addresses */
static uint8_t rows_addr[DISPLAY_ROWS] = {0x00, 0x40, 0x14, 0x54};
static uint8_t row, col;

/* Helper function declarations */
static void set_data_gpio_as_input(void);
static void set_data_gpio_as_output(void);
static void enable(void);
static void send_4_bits(uint8_t data);
static void send_char(uint8_t c);
static void send_cmd(uint8_t cmd);
static bool is_current_position_correct(uint8_t c, uint8_t r);
static void change_position(const uint8_t c, const uint8_t r);


/* HD44780 functions */
void HD44780_init(void)
{
    if (gpio_config(HD44780_PORT, HD44780_PIN_RS, OUTPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    if (gpio_config(HD44780_PORT, HD44780_PIN_RW, OUTPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    if (gpio_config(HD44780_PORT, HD44780_PIN_E, OUTPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    set_data_gpio_as_output();

    RS_LOW();
    RW_LOW();
    E_LOW();
    delay_ms(50); /* Wait for power up */

    send_4_bits(0x03);
	delay_ms(5);
	send_4_bits(0x03);
	delay_ms(1);
	send_4_bits(0x03);
	delay_ms(1);
	send_4_bits(0x02);
	delay_ms(1);
    send_cmd(0x28); /* 4-bit mode, 2 lines, 5x8 font */
    send_cmd(0x0C); /* Display on, cursor and blink off */
    send_cmd(0x06); /* Increment cursor position, no display shift */
    send_cmd(0x01); /* Clear the display */

    delay_ms(2); /* Wait for stabilisation */
}


static void HD44780_print(const char *buf)
{
    while (*buf)
    {
        /* Check writing position and eventually change it */
        if (is_current_position_correct(col, row) != true)
        {
            change_position(col, row);
        }

        /* New line */
        if (*buf == '\n')
        {
            row++;
            col = 0;
            buf++;
            change_position(col, row);
            continue;
        }

        /* Char send */
        send_char(*buf++);
        col++;
    }
}


static void HD44780_clear(void)
{
    send_cmd(0x01);
    row = 0;
    col = 0;
    delay_ms(2);
}




/* Return API functions */
const struct display_interface HD44780_funs = 
{
    HD44780_init,
    HD44780_print,
    HD44780_clear,
};


const struct display_interface *HD44780_get_funs(void)
{
    return &HD44780_funs;
}


/* Helper function definitions */
static void set_data_gpio_as_input(void)
{
    if (gpio_config(HD44780_PORT, HD44780_PIN_D4, INPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    if (gpio_config(HD44780_PORT, HD44780_PIN_D5, INPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    if (gpio_config(HD44780_PORT, HD44780_PIN_D6, INPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    if (gpio_config(HD44780_PORT, HD44780_PIN_D7, INPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
}


static void set_data_gpio_as_output(void)
{
    if (gpio_config(HD44780_PORT, HD44780_PIN_D4, OUTPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    if (gpio_config(HD44780_PORT, HD44780_PIN_D5, OUTPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    if (gpio_config(HD44780_PORT, HD44780_PIN_D6, OUTPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
    if (gpio_config(HD44780_PORT, HD44780_PIN_D7, OUTPUT, PUSH_PULL, NO_PULL, SPEED_MEDIUM) == ERR) safe_state();
}


static void enable(void)
{
    gpio_output_write(HD44780_PORT, HD44780_PIN_E, HIGH);
    delay_ms(1);
    gpio_output_write(HD44780_PORT, HD44780_PIN_E, LOW);
    delay_ms(1);
}


static void send_4_bits(uint8_t data)
{
    gpio_output_write(HD44780_PORT, HD44780_PIN_D4, (data & 0x01) ? 1 : 0);
    gpio_output_write(HD44780_PORT, HD44780_PIN_D5, (data & 0x02) ? 1 : 0);
    gpio_output_write(HD44780_PORT, HD44780_PIN_D6, (data & 0x04) ? 1 : 0);
    gpio_output_write(HD44780_PORT, HD44780_PIN_D7, (data & 0x08) ? 1 : 0);
}


static void send_char(uint8_t c)
{
    gpio_output_write(HD44780_PORT, HD44780_PIN_RS, HIGH);
    gpio_output_write(HD44780_PORT, HD44780_PIN_RW, LOW);

    gpio_output_write(HD44780_PORT, HD44780_PIN_E, HIGH);
    send_4_bits(c >> 4);
    gpio_output_write(HD44780_PORT, HD44780_PIN_E, LOW);

    gpio_output_write(HD44780_PORT, HD44780_PIN_E, HIGH);
    send_4_bits(c);
    gpio_output_write(HD44780_PORT, HD44780_PIN_E, LOW);

    delay_ms(1);
}


static void send_cmd(uint8_t cmd)
{
    gpio_output_write(HD44780_PORT, HD44780_PIN_RS, LOW);
    gpio_output_write(HD44780_PORT, HD44780_PIN_RW, LOW);

    gpio_output_write(HD44780_PORT, HD44780_PIN_E, HIGH);
    send_4_bits(cmd >> 4);
    gpio_output_write(HD44780_PORT, HD44780_PIN_E, LOW);

    gpio_output_write(HD44780_PORT, HD44780_PIN_E, HIGH);
    send_4_bits(cmd);
    gpio_output_write(HD44780_PORT, HD44780_PIN_E, LOW);

    delay_ms(1);
}


static bool is_current_position_correct(uint8_t c, uint8_t r)
{
    bool tmp = true;

    if (c >= DISPLAY_COLS)
    {
        col = 0;
        row++;
        tmp = false;
    }

    if (r >= DISPLAY_ROWS)
    {
        col = 0;
        row = 0;
        tmp = false;
    }

    return tmp;
}


static void change_position(const uint8_t c, const uint8_t r)
{
    if ((c >= DISPLAY_COLS) || (r >= DISPLAY_ROWS)) return; /* Wrong col / row */

    send_cmd(0x80 | (rows_addr[r] + c));
}