#ifndef MENU_H
#define MENU_H

#include <cstdint>

namespace Display{
    void BTN_Up();
    void BTN_Down();
    void BTN_Left();
    void BTN_Right();

    void ResetCursor();

    uint8_t GetSelectedMenuIndex();

    void RenderCtxScreen();
}

#endif