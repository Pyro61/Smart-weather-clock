#pragma once

#include "events/events.h"

/* Empty functions to fill events array, can't generate with preprocessor so must be hardcoded */
void mock_fun0(void);
void mock_fun1(void);
void mock_fun2(void);
void mock_fun3(void);
void mock_fun4(void);
void mock_fun5(void);
void mock_fun6(void);
void mock_fun7(void);

/* Returns address of n mock_fun */
callback_t get_mock_fun_addr(uint8_t n);