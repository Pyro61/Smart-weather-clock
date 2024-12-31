#pragma once

enum state
{
    SLEEP,
    RUN,
    STATE_MAX
};

typedef void (*state_entry_t)(void);
typedef void (*state_get_data_t)(void);
typedef void (*state_send_data_t)(void);
typedef void (*state_exit_t)(void);

struct state_interface
{
    state_entry_t on_entry;
    state_get_data_t on_get;
    state_send_data_t on_send;
    state_exit_t on_exit;
};
