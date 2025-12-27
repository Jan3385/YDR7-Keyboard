#include "Keyboard.h"

#include "pico/time.h"
#include "hardware/pio.h"

#include "config/board.h"
#include "config/user_config.h"

#include "components/Display.h"

namespace Keyboard{

#ifdef ENABLE_DEBUG_FUNCTIONS

bool IsKeyboardKeyPressed()
{
    bool found = false;
    for(uint8_t c = 0; c < K_COLS; c++){
        gpio_put(K_COL_PINS[c], 1);
        sleep_us(10);

        for(uint8_t r = 0; r < K_ROWS; r++){
            if(gpio_get(K_ROW_PINS[r])){
                found = true;
            }
        }

        gpio_put(K_COL_PINS[c], 0);
    }

    return found;
}

#endif

uint8_t* GetKeyPressIndexes()
{
    static uint8_t keyIndexes[6] = { 255, 255, 255, 255, 255, 255 };

    // Initialize all to 255 (no key)
    for(uint8_t i = 0; i < 6; i++){
        keyIndexes[i] = 255;
    }

    uint8_t index = 0;

    for(uint8_t c = 0; c < K_COLS; c++){
        gpio_put(K_COL_PINS[c], 1);
        sleep_us(10);

        for(uint8_t r = 0; r < K_ROWS; r++){
            if(gpio_get(K_ROW_PINS[r])){
                if(KeyMap[r * K_COLS + c] >= HID_KEY_SPECIAL_LOWEST){
                    uint8_t specialKey = KeyMap[r * K_COLS + c];
                    if(specialKey == HID_KEY_SPECIAL_R) Display::BTN_Right();
                    if(specialKey == HID_KEY_SPECIAL_L) Display::BTN_Left();
                    if(specialKey == HID_KEY_SPECIAL_D) Display::BTN_Down();
                    if(specialKey == HID_KEY_SPECIAL_U) Display::BTN_Up();

                    // wait for release
                    while(gpio_get(K_ROW_PINS[r])){ sleep_us(50); }

                    continue;
                }

                if(index < 6){
                    keyIndexes[index++] = r * K_COLS + c;
                }
            }
        }

        gpio_put(K_COL_PINS[c], 0);
    }

    if(index == 0) return nullptr;

    return keyIndexes;
}

}