#pragma once

#include "../../../display/display_interface.h"




void HD44780_init(void);
const struct display_interface *HD44780_get_funs(void);