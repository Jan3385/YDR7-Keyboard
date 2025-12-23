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

namespace Display{
    void Setup();

    void Clear();
    void Update();

    void InitialScreenWTest();
    void ClearInitialScreen();

    void MoveVertical(uint8_t offset);

    void DrawPixel(uint8_t x, uint8_t y, bool on);
    void DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool on);
    void DrawChar(uint8_t x, uint8_t y, char c, bool on);
    void DrawText(uint8_t x, uint8_t y, const char* text, bool on);
}

#endif