#include "events.h"
#include "stddef.h"
#include "stdbool.h"


struct subscribers_data
{
    callback_t fun;
    enum event ev;
};

static struct subscribers_data subscribers_array[EVENTS_MAX_SUBSCRIBERS];


static bool is_cb_already_subscribed(callback_t cb, enum event event)
{
    uint8_t i;
    for (i = 0; i < EVENTS_MAX_SUBSCRIBERS; i++)
    {
        if ((subscribers_array[i].fun == cb) && (subscribers_array[i].ev == event))
        {
            return true;
        }
    }
    return false;
}


static bool is_event_id_correct(enum event event)
{
    return ((event >= EVENT_ID_START) && (event <= EVENT_ID_END));
}


enum subscribe_state events_subscribe(callback_t cb, enum event event)
{
    uint8_t i;

    if (cb == NULL)
    {
        return SUBSCRIBE_NULLPTR;
    }

    if (is_cb_already_subscribed(cb, event))
    {
        return SUBSCRIBE_ALREADY_TAKEN;
    }

    if (!(is_event_id_correct(event)))
    {
        return SUBSCRIBE_WRONG_EVENT_ID;
    }

    for (i = 0; i < EVENTS_MAX_SUBSCRIBERS; i++)
    {
        if (subscribers_array[i].fun == NULL)
        {
            subscribers_array[i].fun = cb;
            subscribers_array[i].ev = event;
            
            return SUBSCRIBE_SUCCESS;
        }
    }
    return SUBSCRIBE_NO_SPACE;
}


void events_notify_subscribers(enum event event)
{
    uint8_t i;

    if (!(is_event_id_correct(event)))
    {
        return;
    }
    
    for (i = 0; i < EVENTS_MAX_SUBSCRIBERS; i++)
    {
        if ((subscribers_array[i].ev == event) && (subscribers_array[i].fun != NULL))
        {
            subscribers_array[i].fun();
        }
    }
}


#ifdef UNIT_TEST
/* Clears subscribers_array before every unit test */
void events_clear(void)
{
    uint8_t i;
    for (i = 0; i < EVENTS_MAX_SUBSCRIBERS; i++)
    {
        subscribers_array[i].fun = NULL;
        subscribers_array[i].ev = NO_EVENT;
    }
}
#endif /* UNIT_TEST */