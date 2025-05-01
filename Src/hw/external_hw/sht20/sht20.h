#pragma once

#include <stdint.h>


enum SHT20_USER_REG
{
    /* Measurement resolution */
    SHT20_RES_H12_T14 = 0x00, /* Default option */
    SHT20_RES_H8_T12 = 0x01,
    SHT20_RES_H10_T13 = 0x80,
    SHT20_RES_H11_T11 = 0x81,

    /* Low battery status (VDD < 2.25 V) */
    SHT20_LOW_BATTERY_BIT_MASK = 0x40,
    SHT20_LOW_BATTERY_TRUE = 0x01,
    SHT20_LOW_BATTERY_FALSE = 0x00,

    /* On-chip heater */
    SHT20_ON_CHIP_HEATER_ENABLE = 0x04, /* Disabled by default */

    /* Disable OTP reload */
    SHT20_OTP_RELOAD_DISABLE = 0x02, /* Disabled by default */
};


/* API functions */
void sht20_init(void);
int32_t sht20_get_temp(void);
int32_t sht20_get_humidity(void);
void sht20_start_measuring(void);