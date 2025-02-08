#pragma once


typedef void (*display_init_t)(void);
typedef void (*display_print_t)(const char *buf);
typedef void (*display_clear_t)(void);


struct display_interface
{
    display_init_t init;
    display_print_t print;
    display_clear_t clear;
};