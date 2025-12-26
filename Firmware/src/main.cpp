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
    
    Display::ClearInitialScreen();

    static float LEDTickTimer = 0;
    constexpr float LEDTickTimerInterval = KEYBOARD_UPDATE_FREQUENCY / static_cast<float>(LED_UPDATE_FREQUENCY);
    static float DisplayTickTimer = 0;
    constexpr float DisplayTickTimerInterval = KEYBOARD_UPDATE_FREQUENCY / static_cast<float>(DISPLAY_UPDATE_FREQUENCY);

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
                        LED::RGB(255,0,0), 
                        15
                    );
                } else break;
            }
                
            tud_hid_keyboard_report(0, 0, report);
        } else {
            tud_hid_keyboard_report(0, 0, nullptr);
            LED::SetBackgroundColor(LED::RGB(255, 255, 255), 15);
        }

        if(LEDTickTimer >= LEDTickTimerInterval){
            LEDTickTimer -= LEDTickTimerInterval;
            LED::Tick();
            LED::Show();
        }

        if(DisplayTickTimer >= DisplayTickTimerInterval){
            DisplayTickTimer -= DisplayTickTimerInterval;
            Display::UpdateMenu();
        }

        static constexpr float KEYBOARD_SLEEP_TIME_MS = 1.0f / KEYBOARD_UPDATE_FREQUENCY * 1000.0f;
        LEDTickTimer += KEYBOARD_SLEEP_TIME_MS;
        DisplayTickTimer += KEYBOARD_SLEEP_TIME_MS;
        sleep_ms(KEYBOARD_SLEEP_TIME_MS);
    }

    return 0;
}