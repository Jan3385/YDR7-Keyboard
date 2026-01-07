#include "menu.h"

#include "config/board.h"
#include "config/user_config.h"
#include "config/themes.h"
#include "setup/pin_setup.h"

#include "components/Display.h"
#include "components/Font.h"
#include "components/Keyboard.h"

#include "pico/bootrom.h"

#include <malloc.h>
#include "hardware/clocks.h"

struct Vec2{
    Vec2(): x(0), y(0) {}
    Vec2(uint8_t _x, uint8_t _y) : x(_x), y(_y) {}
    uint8_t x;
    uint8_t y;
    uint8_t maxX;
    uint8_t maxY;
    void Left()  { if(this->x > 0)    this->x--; }
    void Right() { if(this->x < maxX) this->x++; }
    void Up()    { if(this->y > 0)    this->y--; }
    void Down()  { if(this->y < maxY) this->y++; }
};

static Vec2 CursorMenuLimits[] = {
    Vec2(0,0), // Info
    Vec2(0,0), // Settings
    Vec2(1,THEME_COUNT-1), // Themes
    Vec2(1,0)  // Bootloader
};

namespace Display{
    static Vec2 cursor(0,0);
    static uint8_t selectedMenu = 0;

    void NewMenuSelected(){
        cursor = Vec2(0,0);
        cursor.maxX = CursorMenuLimits[selectedMenu].x;
        cursor.maxY = CursorMenuLimits[selectedMenu].y;

        if(selectedMenu == 2)
            cursor.y = Theme::GetCurrentThemeIndex();
    }
}

void Display::BTN_Up()
{
    if(cursor.x == 0){
        if(selectedMenu > 0){
            selectedMenu--;
            NewMenuSelected();
        }
    }else{
        Display::cursor.Up();

        if(selectedMenu == 2)
            Theme::ApplyTheme(Display::cursor.y);
    }
}
void Display::BTN_Down()
{
    if(cursor.x == 0){
        if(selectedMenu < 3){
            selectedMenu++;
            NewMenuSelected();
        }
    }else{
        Display::cursor.Down();

        if(selectedMenu == 2)
            Theme::ApplyTheme(Display::cursor.y);
    }
}
void Display::BTN_Left()
{
    Display::cursor.Left();
}
void Display::BTN_Right()
{
    if(selectedMenu == 3 && Display::cursor.x == 1)
        reset_usb_boot(0, 0);

    Display::cursor.Right();
}

void Display::ResetCursor()
{
    cursor = Vec2(0,0);
    cursor.maxX = CursorMenuLimits[selectedMenu].x;
    cursor.maxY = CursorMenuLimits[selectedMenu].y;

    if(selectedMenu == 2)
        cursor.y = Theme::GetCurrentThemeIndex();
}

uint8_t Display::GetSelectedMenuIndex()
{
    return Display::selectedMenu;
}

void RenderInfoScreen()
{
    constexpr uint8_t ctxCenter = MENU_SIDE_OFFSET + (DISPLAY_WIDTH - MENU_SIDE_OFFSET) / 2;
    constexpr uint8_t txtSpacing = 6;
    constexpr uint8_t txtYOffset = 5;
    uint8_t txtNum = 0;

    Display::DrawTextCenter( ctxCenter, txtYOffset,
        PRODUCT_NAME, true
    );

    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        "by " MANUFACTURER_NAME, true
    );
    ++txtNum;

    char text[18];
    snprintf(text, sizeof(text), "CPU: %u kHz", clock_get_hz(clk_sys) / 1000);
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );

#ifdef PICO_FLASH_SIZE_BYTES
    snprintf(text, sizeof(text), "Flash: %lu KB", PICO_FLASH_SIZE_BYTES / 1024);
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );
#elif
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        "Flash: N/A KB", true
    );
#endif
    struct mallinfo mi = mallinfo();
    snprintf(text, sizeof(text), "Heap: %d / %d B", mi.ordblks, mi.fordblks);
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );
    snprintf(text, sizeof(text), "Total Heap: %d B", mi.arena);
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );

    snprintf(text, sizeof(text), "Uptime: %5.1f s", to_ms_since_boot(get_absolute_time()) / 1000.0f);
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );

    float temp = ReadInternalTemperatureC();
    snprintf(text, sizeof(text), "Temp: %.1f C", temp);
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );

    snprintf(text, sizeof(text), "CapsLock: %s", Keyboard::capsLockState ? "ON" : "OFF");
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );
    snprintf(text, sizeof(text), "NumLock: %s", Keyboard::numLockState ? "ON" : "OFF");
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );
    snprintf(text, sizeof(text), "ScrollLock: %s", Keyboard::scrollLockState ? "ON" : "OFF");
    Display::DrawTextCenter( ctxCenter, txtYOffset + txtSpacing * ++txtNum,
        text, true
    );
}

void RenderThemesScreen(){
    constexpr uint8_t ctxOffset = MENU_SIDE_OFFSET;
    constexpr uint8_t leftRightSpacing = 10;
    constexpr uint8_t txtSpacing = 8;
    constexpr uint8_t txtYOffset = 5;
    uint8_t txtNum = 0;

    for(uint8_t i = 0; i < THEME_COUNT; i++){
        if(i == Display::cursor.y){
            Display::DrawRect(
                leftRightSpacing + ctxOffset, 
                txtYOffset + txtSpacing * txtNum,
                DISPLAY_WIDTH - 2*leftRightSpacing - ctxOffset,
                FONT_HEIGHT + 4,
                true
            );
        }
        Display::DrawTextWithBorder(
            leftRightSpacing + ctxOffset, txtYOffset + txtSpacing * txtNum,
            Theme::themes[i].name, 1, DISPLAY_WIDTH - 2 * leftRightSpacing - ctxOffset
        );
        txtNum++;
    }
}
void RenderBootloaderScreen(){
    constexpr uint8_t ctxCenter = MENU_SIDE_OFFSET + (DISPLAY_WIDTH - MENU_SIDE_OFFSET) / 2;
    constexpr uint8_t txtYOffset = 50;

    if(Display::cursor.x == 0){
        Display::DrawTextCenter( ctxCenter, txtYOffset,
            "Press RIGHT to", true
        );
        Display::DrawTextCenter( ctxCenter, txtYOffset + FONT_HEIGHT + 2,
            "flash new firmware", true
        );
        Display::DrawTextCenter( ctxCenter, txtYOffset + FONT_HEIGHT + 10,
            "this erases all data", true
        );
    }else{
        Display::DrawTextCenter( ctxCenter, txtYOffset,
            "Are you", true
        );
        Display::DrawTextCenter( ctxCenter, txtYOffset + FONT_HEIGHT + 2,
            "REALLY sure??", true
        );
    }
}

void Display::RenderCtxScreen()
{
    constexpr uint8_t ctxOffset = MENU_SIDE_OFFSET;
    constexpr uint8_t ctxWidth = DISPLAY_WIDTH - ctxOffset;
    constexpr uint8_t ctxHeight = DISPLAY_HEIGHT;

    Display::DrawBorder(ctxOffset, 0,
        ctxWidth, ctxHeight, 1, true);

    switch (Display::GetSelectedMenuIndex())
    {
    case 0:
        RenderInfoScreen();
        break;
    case 1:
        
        break;
    case 2:
        RenderThemesScreen();
        break;
    case 3:
        RenderBootloaderScreen();
        break;
    default:
        Display::DrawTextNeg(
            ctxOffset + 5,
            5, 
            "? UNKNOWN MENU ?"
        );
        break;
    }
}