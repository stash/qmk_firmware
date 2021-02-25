# Set any rules.mk overrides for your specific keymap here.
# See rules at https://docs.qmk.fm/#/config_options?id=the-rulesmk-file
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
AUTO_SHIFT_ENABLE = no
DYNAMIC_MACRO_ENABLE = no
WEBUSB_ENABLE = no
ORYX_ENABLE = no
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = yes
MOUSEKEY_ENABLE = yes
LEADER_ENABLE = yes
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no

RAW_ENABLE = yes
BLINKY_ENABLE = yes

DEBOUNCE_TYPE = eager_pk
DEBOUNCE = 45
SWAP_HANDS_ENABLE = no
SRC = matrix.c
SRC += cvvv.c
SRC += superalttab.c

COMMON_VPATH += "../source/repos/qmk-private"
COMMON_VPATH += "../src/qmk-private"
