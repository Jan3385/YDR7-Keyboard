/**
 * BOARD CONFIG FILE
 * this file contains all hardware specific settings
 * you should only modify this file for direct hardware changes to your keyboard
 * DO NOT modify this file if you
 *  1. dont know what you are doing
 *  2. have not changed the hardware of your keyboard (such as changing the keyboard pinout or flashing a different keyboard model)
 * 
 * Modifying this file may result in permanent damage to your hardware!
 */

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
#define OLED_I2C_ADDRESS 0x3C
#define OLED_SDA_PIN 0
#define OLED_SCL_PIN 1

// LED SK6812 array
#define NUM_OF_LEDS 103
#define LED_PIN 27

const uint8_t KEYBOARD_KEY_TO_LED_INDEX[K_COLS * K_ROWS] = {
    102,101,100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89,
     67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
     66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53,
     33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 0 ,
     32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 0 , 0 ,
     4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 20, 3 , 2 , 1 , 0 , // <- real index 0
     13, 19, 18, 17, 46, 47, 48, 52, 51, 50, 81, 82, 83, 0 ,
     14, 15, 16, 49, 84, 88, 87, 86, 85, 0 , 0 , 0 , 0 , 0
};

// Unused pins to unpower
#define UNUSED_PINS_COUNT 2
#define UNUSED_PINS { 34, 35 }

#define HID_KEY_SPECIAL_LOWEST 0xFC
#define HID_KEY_SPECIAL_D 0xFC
#define HID_KEY_SPECIAL_L 0xFD
#define HID_KEY_SPECIAL_R 0xFE
#define HID_KEY_SPECIAL_U 0xFF

#endif