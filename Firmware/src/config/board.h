#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

// Keyboard matrix
#define K_ROWS 8
#define K_COLS 14

#define K_ROW0_PIN 2
#define K_ROW1_PIN 3
#define K_ROW2_PIN 4
#define K_ROW3_PIN 5
#define K_ROW4_PIN 6
#define K_ROW5_PIN 7
#define K_ROW6_PIN 8
#define K_ROW7_PIN 9

const uint8_t K_ROW_PINS[K_ROWS] = {
    K_ROW0_PIN,
    K_ROW1_PIN,
    K_ROW2_PIN,
    K_ROW3_PIN,
    K_ROW4_PIN,
    K_ROW5_PIN,
    K_ROW6_PIN,
    K_ROW7_PIN
};

#define K_COL0_PIN 10
#define K_COL1_PIN 11
#define K_COL2_PIN 12
#define K_COL3_PIN 13
#define K_COL4_PIN 14
#define K_COL5_PIN 15
#define K_COL6_PIN 16
#define K_COL7_PIN 17
#define K_COL8_PIN 18
#define K_COL9_PIN 19
#define K_COL10_PIN 20
#define K_COL11_PIN 21
#define K_COL12_PIN 22
#define K_COL13_PIN 26

const uint8_t K_COL_PINS[K_COLS] = {
    K_COL0_PIN,
    K_COL1_PIN,
    K_COL2_PIN,
    K_COL3_PIN,
    K_COL4_PIN,
    K_COL5_PIN,
    K_COL6_PIN,
    K_COL7_PIN,
    K_COL8_PIN,
    K_COL9_PIN,
    K_COL10_PIN,
    K_COL11_PIN,
    K_COL12_PIN,
    K_COL13_PIN
};

// Oled Display
#define OLED_I2C_PORT i2c0
#define OLED_SDA_PIN 0
#define OLED_SCL_PIN 1

// LED SK6812 array
#define LED_PIN 27

#endif