#pragma once

#include "stdint.h"


#define EVENTS_MAX_SUBSCRIBERS  8


enum event
{
    NO_EVENT,
    EVENT_ID_START,
    EVENT_RUN = EVENT_ID_START,
    EVENT_SLEEP,
    EVENT_GET,
    EVENT_SEND,
    EVENT_BME280_DATA_READY,
    EVENT_ID_END,
    EVENT_MEAS_DELAY_ELAPSED = EVENT_ID_END
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