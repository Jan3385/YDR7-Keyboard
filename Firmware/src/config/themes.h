/**
 * COLOR THEMES CONFIG FILE
 * this file is for defining color themes avalible on the keyboard
 * they act on the neopixel colors
 */

#ifndef THEMES_H
#define THEMES_H

#define THEME_COUNT 8

#include "components/LED.h"

namespace Theme{
    struct ThemeColors{
        const char* name;
        LED::RGB backgroundColor;
        LED::RGB activeColor;
        LED::RGB passiveActiveColor;
    };
    extern const ThemeColors themes[THEME_COUNT];

    void ApplyTheme(uint8_t themeIndex);
    uint8_t GetCurrentThemeIndex();
    const char* GetThemeName();
    LED::RGB GetBackgroundColor();
    LED::RGB GetActiveColor();
    LED::RGB GetPassiveActiveColor();
}

#endif