#include "display_mock.h"
#include <string.h>

#define MOCK_BUF_MAX_SIZE   80

static char mock_buf[MOCK_BUF_MAX_SIZE];

static void display_mock_init(void)
{

}

static void display_mock_print(const char* buf)
{
    strncpy(mock_buf, buf, MOCK_BUF_MAX_SIZE);
}

static void display_mock_clear(void)
{
    memset((void*)mock_buf, 0, MOCK_BUF_MAX_SIZE * sizeof(char));
}

struct display_interface display_mock = 
{
    display_mock_init,
    display_mock_print,
    display_mock_clear
};


struct display_interface *get_display_mock_funs(void)
{
    return &display_mock;
}

void display_mock_reset_buf(void)
{
    memset((void*)mock_buf, 0, MOCK_BUF_MAX_SIZE * sizeof(char));
}

void display_mock_read_buf(char *dest_buf)
{
    strncpy(dest_buf, (const char*)mock_buf, MOCK_BUF_MAX_SIZE);
}