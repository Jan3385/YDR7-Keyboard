#include "menu.h"

#include "config/board.h"
#include "config/user_config.h"
#include "setup/pin_setup.h"

#include "components/Display.h"
#include "components/Keyboard.h"

#include "pico/bootrom.h"

#include <malloc.h>
#include "hardware/clocks.h"

namespace Display{
    static int8_t selectedMenu = 0;
}

void Display::BTN_Up()
{
    Display::selectedMenu--;

    if(Display::selectedMenu < 0)
        Display::selectedMenu = 0;
}
void Display::BTN_Down()
{
    Display::selectedMenu++;

    if(Display::selectedMenu > 3)
        Display::selectedMenu = 3;
}
void Display::BTN_Left()
{

}
void Display::BTN_Right()
{
    if(Display::selectedMenu == 3)
        reset_usb_boot(0, 0);
}

uint8_t Display::GetSelectedMenuIndex()
{
    return Display::selectedMenu;
}

void RenderInfoScreen()
{
    constexpr uint8_t ctxCenter = MENU_SIDE_WIDTH + (DISPLAY_WIDTH - MENU_SIDE_WIDTH) / 2;
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

void Display::RenderCtxScreen()
{
    constexpr uint8_t ctxOffset = MENU_SIDE_WIDTH;
    constexpr uint8_t ctxWidth = DISPLAY_WIDTH - ctxOffset;
    constexpr uint8_t ctxHeight = DISPLAY_HEIGHT - ctxOffset;

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
        
        break;
    default:
        Display::DrawTextNeg(
            ctxOffset,
            5, 
            "? UNKNOWN MENU ?"
        );
        break;
    }
}