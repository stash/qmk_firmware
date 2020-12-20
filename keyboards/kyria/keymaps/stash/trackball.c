#include "trackball.h"

#define TRACKBALL_WRITE ((TRACKBALL_ADDRESS << 1) | I2C_WRITE)
#define TRACKBALL_READ  ((TRACKBALL_ADDRESS << 1) | I2C_READ)
#define REG_RED 0x00
#define REG_GREEN 0x01
#define REG_BLUE 0x02
#define REG_WHITE 0x03

#define REG_CTRL 0xFE
#define MSK_CTRL_SLEEP 0x01
#define MSK_CTRL_RESET 0x02

#define REG_LEFT 0x04
#define REG_RIGHT 0x05
#define REG_UP 0x06
#define REG_DOWN 0x07
#define REG_SWITCH 0x08
// switch button is in high bit:
#define MSK_SWITCH_STATE 0x80

void trackball_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    uint8_t data[] = {REG_RED, r, g, b, w};
    i2c_transmit(TRACKBALL_WRITE, data, sizeof(data), I2C_TIMEOUT);
}

void trackball_reset() {
    uint8_t data[] = {REG_CTRL, MSK_CTRL_RESET};
    i2c_transmit(TRACKBALL_WRITE, data, sizeof(data), I2C_TIMEOUT);
}

bool trackball_read(uint16_t *x_axis, uint16_t *y_axis, uint8_t *button) {
    return false;
}
