#include "buzzer.h"
#include "../../mcu_hw/gpio/gpio.h"

/* Buzzer port defines */
#define BUZZER_PORT     PORT_B
#define BUZZER_PIN      7


/* API functions */
void buzzer_init(void)
{
    gpio_config(BUZZER_PORT, BUZZER_PIN, OUTPUT, OPEN_DRAIN, PULL_UP, SPEED_LOW);
}


void buzzer_start(void)
{
    gpio_output_write(BUZZER_PORT, BUZZER_PIN, LOW);
}


void buzzer_stop(void)
{
    gpio_output_write(BUZZER_PORT, BUZZER_PIN, HIGH);
}