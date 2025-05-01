#pragma once
#include "../../Src/display/display_interface.h"

struct display_interface *get_display_mock_funs(void);

void display_mock_reset_buf(void);
void display_mock_read_buf(char *dest_buf);