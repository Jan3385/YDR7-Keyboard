#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>

#define OLED_OFF 0xAE
#define OLED_ON 0xAF
#define OLED_NORMAL_DISPLAY 0xA6
#define OLED_INVERT_DISPLAY 0xA7
#define OLED_MEMORY_MODE 0x20
#define OLED_MEMORY_MODE_HORIZONTAL 0x00
#define OLED_SEGMENT_REMAP 0xA1
#define OLED_COM_SCAN_NORMAL 0xC0
#define OLED_COM_SCAN_REVERSE 0xC8
#define OLED_MULTIPLEX_RATIO 0xA8

#define OLED_DISPLAY_NORMAL 0xA4
#define OLED_DISPLAY_ALL_ON 0xA5

#define MENU_SIDE_WIDTH 28

namespace Display{
    static int8_t selectedMenu = 0;
    void BTN_Up();
    void BTN_Down();
    void BTN_Left();
    void BTN_Right();

    void Setup();

    void InitialScreenWTest();
    void ClearInitialScreen();

    void UpdateMenu();
}

#endif