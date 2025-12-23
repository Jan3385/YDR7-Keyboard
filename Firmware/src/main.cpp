#include <pico/stdlib.h>
#include <stdio.h> // Add for debug output

#include "config/user_config.h"

#include "setup/HID_setup.h"
#include "setup/TUD_setup.h"
#include "setup/pin_setup.h"

#include "components/Keyboard.h"

int main(){
    PinSetup_ALL();

    tusb_init();

    while (true) {
        tud_task();

        if( !tud_hid_ready() ) {
            sleep_ms(1);
            continue;
        }

        uint8_t* pressedKeys = Keyboard::GetKeyPressIndexes();
        if (pressedKeys) {
            LED::LedArray.fill(WS2812::RGB(LED_BRIGHTNESS_VALUE_CAP));

            uint8_t report[6] = { 0 };

            for(uint8_t i = 0; i < 6; i++){
                if(pressedKeys[i] != 255){
                    report[i] = Keyboard::KeyMap[pressedKeys[i]];
                    LED::LedArray.setPixelColor(
                        KEYBOARD_KEY_TO_LED_INDEX[pressedKeys[i]], 
                        WS2812::RGB(0,LED_BRIGHTNESS_VALUE_CAP,0)
                    );
                } else break;
            }
                
            tud_hid_keyboard_report(0, 0, report);
            LED::LedArray.show();
        } else {
            tud_hid_keyboard_report(0, 0, nullptr);
            LED::LedArray.fill(WS2812::RGB(0,0,5));
            LED::LedArray.show();
        }

        sleep_ms(10);
    }

    return 0;
}