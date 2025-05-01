#include "time_mock.h"

static struct time mock_time_holder;

void time_set(struct time time)
{
    mock_time_holder.hours = time.hours;
    mock_time_holder.minutes = time.minutes;
    mock_time_holder.seconds = time.seconds;
}

struct time time_get(void)
{
    return mock_time_holder;
}