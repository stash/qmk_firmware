#pragma once

enum blinky_cmds {
    BLINKY_CMD_INIT = 1,
    BLINKY_CMD_SET_ALL,
    BLINKY_CMD_SET_ONE,
    BLINKY_CMD_SET_MANY,

    BLINKY_EVT_BASE = 0x7f,
    BLINKY_EVT_INIT,
    BLINKY_EVT_LAYER,
};

extern void blinky_send_layer(uint8_t layer);
