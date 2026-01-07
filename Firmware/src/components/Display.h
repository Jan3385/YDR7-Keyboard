#ifndef DISPLAY_H
#define DISPLAY_H

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 128

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

#define MENU_SIDE_OFFSET 28

namespace Display{
    void SpecialkeyPressed(uint8_t key);

    void Setup();

    void Clear();

    // draw functions ---
    void DrawPixel(uint8_t x, uint8_t y, bool on);
    void FlipPixel(uint8_t x, uint8_t y);

    void DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool on);
    void DrawBorder(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t thickness, bool on);

    void DrawChar(uint8_t x, uint8_t y, char c, bool on);
    void DrawCharNeg(uint8_t x, uint8_t y, char c);
    void DrawText(uint8_t x, uint8_t y, const char* text, bool on);
    void DrawTextCenter(uint8_t x, uint8_t y, const char* text, bool on);
    void DrawTextNeg(uint8_t x, uint8_t y, const char* text);

    void DrawTextWithBorder(uint8_t x, uint8_t y, const char* text, uint8_t borderThickness, uint8_t minBorderWidth);
    // ------------------

    void MoveVertical(uint8_t offset);

    void InitialScreenWTest();
    void ClearInitialScreen();

    void UpdateMenu();

    void Show();
}

#endif