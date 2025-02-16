#pragma once

#include "../types.h"
#include <stdbool.h>

bool is_alarm_set(void);
void alarm_set(struct time time);

