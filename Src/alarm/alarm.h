#pragma once

#include "../types.h"
#include <stdbool.h>

bool is_alarm_set(void);
void set_alarm(struct time time);

