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
void mock_fun8(void) {}
void mock_fun9(void) {}
void mock_fun10(void) {}
void mock_fun11(void) {}
void mock_fun12(void) {}
void mock_fun13(void) {}
void mock_fun14(void) {}
void mock_fun15(void) {}
void mock_fun16(void) {}
void mock_fun17(void) {}
void mock_fun18(void) {}
void mock_fun19(void) {}
void mock_fun20(void) {}
void mock_fun21(void) {}
void mock_fun22(void) {}
void mock_fun23(void) {}
void mock_fun24(void) {}

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
    mock_fun8,
    mock_fun9,
    mock_fun10,
    mock_fun11,
    mock_fun12,
    mock_fun13,
    mock_fun14,
    mock_fun15,
    mock_fun16,
    mock_fun17,
    mock_fun18,
    mock_fun19,
    mock_fun20,
    mock_fun21,
    mock_fun22,
    mock_fun23,
    mock_fun24,
};

callback_t get_mock_fun_addr(uint8_t n)
{
    return mock_funs_array[n];
}