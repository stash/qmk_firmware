#CONSOLE_ENABLE = yes
#DEBUG_ENABLE = yes
BOOTMAGIC_ENABLE = no

OLED_DRIVER_ENABLE = yes   # Enables the use of OLED displays
ENCODER_ENABLE = yes       # Enables the use of one or more encoders
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no
AUTO_SHIFT_ENABLE = no
LEADER_ENABLE = yes
DYNAMIC_MARCO_ENABLE = no
NKRO_ENABLE = yes

SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
POINTING_DEVICE_ENABLE = yes

DEBOUNCE_TYPE = eager_pk
SRC += superalttab.c
SRC += trackball.c
COMMON_VPATH += "../source/repos/qmk-private"
COMMON_VPATH += "../src/qmk-private"
