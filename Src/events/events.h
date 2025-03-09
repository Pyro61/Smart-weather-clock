#pragma once

#include "stdint.h"


#define EVENTS_MAX_SUBSCRIBERS  25


enum event
{
    NO_EVENT,
    EVENT_ID_START,
    EVENT_RUN = EVENT_ID_START,
    EVENT_SLEEP,
    EVENT_GET,
    EVENT_BME280_DATA_READY,
    EVENT_BUTTON_OK_MODE_PRESSED,
    EVENT_BUTTON_RIGHT_PRESSED,
    EVENT_BUTTON_LEFT_PRESSED,
    EVENT_BUTTON_UP_PRESSED,
    EVENT_BUTTON_DOWN_PRESSED,
    EVENT_ALARM,
    EVENT_1S_ELAPSED,
    EVENT_MEAS_DELAY_ELAPSED,
    EVENT_SEND = 0x43,
    EVENT_ID_END = EVENT_SEND
};

enum subscribe_state
{
    SUBSCRIBE_SUCCESS,
    SUBSCRIBE_NO_SPACE,
    SUBSCRIBE_ALREADY_TAKEN,
    SUBSCRIBE_WRONG_EVENT_ID,
    SUBSCRIBE_NULLPTR
};

typedef void (*callback_t)(void);


enum subscribe_state events_subscribe(callback_t cb, enum event event);
void events_notify_subscribers(enum event event);

#ifdef UNIT_TEST
/* Needed only in unit tests */
void events_clear(void);
#endif /* UNIT_TEST */