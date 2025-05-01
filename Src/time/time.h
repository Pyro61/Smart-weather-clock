#pragma once

#include "time_interface.h"


void time_init(const struct time_interface *time_interface);
void time_set(const struct time time);
struct time time_get(void);
