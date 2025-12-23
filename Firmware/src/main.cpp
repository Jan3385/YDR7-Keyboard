#include <pico/stdlib.h>
#include <stdio.h> // Add for debug output

#include "config/user_config.h"

#include "setup/HID_setup.h"
#include "setup/TUD_setup.h"
#include "setup/pin_setup.h"

#include "components/Keyboard.h"
#include "components/LED.h"

int main(){
    PinSetup_ALL();
    Display::InitialScreenWTest();

    tusb_init();

    do{
        tud_task();
        sleep_ms(1);
    }while(!tud_hid_ready());
    
    sleep_ms(500);
    Display::ClearInitialScreen();

    while (true) {
        tud_task();

        uint8_t* pressedKeys = Keyboard::GetKeyPressIndexes();
        if (pressedKeys) {
            uint8_t report[6] = { 0 };

            for(uint8_t i = 0; i < 6; i++){
                if(pressedKeys[i] != 255){
                    report[i] = Keyboard::KeyMap[pressedKeys[i]];
                    
                    LED::ChangeColor(
                        KEYBOARD_KEY_TO_LED_INDEX[pressedKeys[i]], 
                        WS2812::RGB(LED_BRIGHTNESS_VALUE_CAP * 1.5,0,0), 
                        15
                    );
                } else break;
            }
                
            tud_hid_keyboard_report(0, 0, report);
        } else {
            tud_hid_keyboard_report(0, 0, nullptr);
            LED::SetBackgroundColor(WS2812::RGB(LED_BRIGHTNESS_VALUE_CAP / 3, LED_BRIGHTNESS_VALUE_CAP / 3, LED_BRIGHTNESS_VALUE_CAP / 3), 15);
        }
        LED::Tick();
        LED::Show();

        sleep_ms(10);
    }

    return 0;
}