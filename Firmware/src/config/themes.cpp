#include "themes.h"

const Theme::ThemeColors Theme::themes[THEME_COUNT] = {
    Theme::ThemeColors{
        "White Red",
        LED::RGB(128, 128, 128),
        LED::RGB(255, 50, 50),
        LED::RGB(100, 20, 20)
    },
    Theme::ThemeColors{
        "Neon Purple",
        LED::RGB(20, 20, 30),
        LED::RGB(255, 0, 200),
        LED::RGB(0, 200, 255)
    },
    Theme::ThemeColors{
        "Inverted",
        LED::RGB(30, 45, 35),
        LED::RGB(0, 0, 0),
        LED::RGB(10, 20, 10)
    },
    Theme::ThemeColors{
        "Old woods",
        LED::RGB(60, 30, 20),
        LED::RGB(255, 120, 40),
        LED::RGB(180, 60, 30)
    },
    Theme::ThemeColors{
        "Purple",
        LED::RGB(40, 20, 60),
        LED::RGB(255, 80, 180),
        LED::RGB(80, 200, 255)
    },
    Theme::ThemeColors{
        "Dark Forest",
        LED::RGB(10, 20, 10),
        LED::RGB(120, 255, 0),
        LED::RGB(40, 120, 0)
    },
    Theme::ThemeColors{
        "Modern Yellow",
        LED::RGB(50, 50, 50),
        LED::RGB(255, 180, 40),
        LED::RGB(160, 100, 20)
    },
    Theme::ThemeColors{
        "Dark Space",
        LED::RGB(5, 5, 20),
        LED::RGB(140, 120, 255),
        LED::RGB(60, 40, 120)
    }
};

uint8_t currentThemeIndex = 0;
void Theme::ApplyTheme(uint8_t themeIndex)
{
    if(themeIndex >= THEME_COUNT) return;

    currentThemeIndex = themeIndex;

    const ThemeColors& theme = themes[themeIndex];
    LED::SetBackgroundColor(theme.backgroundColor, 10);
}

uint8_t Theme::GetCurrentThemeIndex()
{
    return currentThemeIndex;
}

const char *Theme::GetThemeName()
{
    return themes[currentThemeIndex].name;
}

LED::RGB Theme::GetBackgroundColor()
{
    return themes[currentThemeIndex].backgroundColor;
}

LED::RGB Theme::GetActiveColor()
{
    return themes[currentThemeIndex].activeColor;
}

LED::RGB Theme::GetPassiveActiveColor()
{
    return themes[currentThemeIndex].passiveActiveColor;
}
