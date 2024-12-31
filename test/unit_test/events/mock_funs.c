#include "mock_funs.h"

/* Empty functions to fill events array, can't generate with preprocessor so must be hardcoded */
void mock_fun0(void) {}
void mock_fun1(void) {}
void mock_fun2(void) {}
void mock_fun3(void) {}
void mock_fun4(void) {}
void mock_fun5(void) {}
void mock_fun6(void) {}
void mock_fun7(void) {}

static callback_t mock_funs_array[EVENTS_MAX_SUBSCRIBERS] = 
{
    mock_fun0,
    mock_fun1,
    mock_fun2,
    mock_fun3,
    mock_fun4,
    mock_fun5,
    mock_fun6,
    mock_fun7,
};

callback_t get_mock_fun_addr(uint8_t n)
{
    return mock_funs_array[n];
}