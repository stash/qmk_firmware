#include QMK_KEYBOARD_H
#include "blinky.h"
#include "raw_hid.h"

inline static int16_t blinky_read_int16BE(const uint8_t *data, uint8_t *index) {
  int16_t result = ((int16_t)data[*index] << 8) + data[*index+1];
  *index += 2;
  return result;
}

inline static void blinky_read_RGBf(const uint8_t *data, uint8_t *index, RGB *rgb, float f) {
  rgb->r = f * data[*index];
  rgb->g = f * data[*index+1];
  rgb->b = f * data[*index+2];
  *index += 3;
}

inline static void blinky_write_int16BE(int16_t value, uint8_t *data, uint8_t index) {
  data[index]   = (value & 0xff00) >> 8;
  data[index+1] = (value & 0x00ff);
}

static void blinky_set_rgb_matrix_chunk(const uint8_t *data, uint8_t *index, uint8_t limit) {
  RGB rgb;
  float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
  int16_t led_index = blinky_read_int16BE(data, index);
  while ((*index)+3 <= limit && led_index < DRIVER_LED_TOTAL) {
    blinky_read_RGBf(data, index, &rgb, f);
    rgb_matrix_set_color(led_index++, rgb.r, rgb.g, rgb.b);
  }
}

static void blinky_set_whole_rgb_matrix(const uint8_t *data, uint8_t *index) {
  RGB rgb;
  float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
  blinky_read_RGBf(data, index, &rgb, f);
  rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
}

static void blinky_reply_init(void) {
  uint8_t buffer[RAW_EPSIZE];
  memset(buffer, 0, RAW_EPSIZE);
  buffer[0] = BLINKY_EVT_INIT;
  buffer[1] = 2; // protocol version
  buffer[2] = RAW_EPSIZE;
  blinky_write_int16BE(DRIVER_LED_TOTAL, buffer, 3);
  buffer[5] = get_highest_layer(layer_state);
  raw_hid_send(buffer, RAW_EPSIZE);
}

void blinky_send_layer(uint8_t layer) {
  uint8_t buffer[RAW_EPSIZE];
  memset(buffer, 0, RAW_EPSIZE);
  buffer[0] = BLINKY_EVT_LAYER;
  buffer[1] = get_highest_layer(layer_state);
  raw_hid_send(buffer, RAW_EPSIZE);
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
  if (length == 0) return;

  uint8_t i = 0, limit;
  uint8_t cmd = data[i++];
  switch (cmd) {
    case BLINKY_CMD_INIT:
      blinky_reply_init();
      break;

    case BLINKY_CMD_SET_ALL:
      if (length - i < 3) break;
      blinky_set_whole_rgb_matrix(data, &i);
      break;

    case BLINKY_CMD_SET_ONE:
    case BLINKY_CMD_SET_MANY:
      if (length - i < 5) break;

      if (cmd == BLINKY_CMD_SET_ONE) limit = 5 + i;
      else limit = (length < RAW_EPSIZE) ? length : RAW_EPSIZE;

      blinky_set_rgb_matrix_chunk(data, &i, limit);
      break;

    default:
      break;
  }
}
