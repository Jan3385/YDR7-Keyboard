#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/mutex.h>

#include <atomic>

#include "config/themes.h"
#include "config/user_config.h"

#include "setup/HID_setup.h"
#include "setup/TUD_setup.h"
#include "setup/pin_setup.h"

#include "components/Keyboard.h"
#include "components/LED.h"

mutex_t LEDMutex;
mutex_t DisplayMutex;

std::atomic<bool> updateLEDRequest{false};
std::atomic<bool> updateDisplayRequest{false};

void MULTICORE_DataPushHandler(){
    while(true){
        if(updateLEDRequest.load(std::memory_order_acquire)){
            mutex_enter_blocking(&LEDMutex);
            LED::Show();
            mutex_exit(&LEDMutex);

            updateLEDRequest.store(false,  std::memory_order_release);
        }
        if(updateDisplayRequest.load(std::memory_order_acquire)){
            mutex_enter_blocking(&DisplayMutex);
            Display::Show();
            mutex_exit(&DisplayMutex);
            
            updateDisplayRequest.store(false,  std::memory_order_release);
        }
        sleep_ms(1);
    }
}

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

    mutex_init(&LEDMutex);
    mutex_init(&DisplayMutex);

    multicore_launch_core1(MULTICORE_DataPushHandler);

    Theme::ApplyTheme(1);

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
                        Theme::GetActiveColor(), 
                        15
                    );
                } else break;
            }
            tud_hid_keyboard_report(0, 0, report);
        } else {
            tud_hid_keyboard_report(0, 0, nullptr);
            
        }

        if(LEDTickTimer >= LEDTickTimerInterval){
            LEDTickTimer -= LEDTickTimerInterval;

            mutex_enter_blocking(&LEDMutex);
            LED::Tick();
            mutex_exit(&LEDMutex);
            
            updateLEDRequest.store(true, std::memory_order_release);
        }

        if(DisplayTickTimer >= DisplayTickTimerInterval){
            DisplayTickTimer -= DisplayTickTimerInterval;

            mutex_enter_blocking(&DisplayMutex);
            Display::UpdateMenu();
            mutex_exit(&DisplayMutex);

            updateDisplayRequest.store(true, std::memory_order_release);
        }

        static constexpr float KEYBOARD_SLEEP_TIME_MS = 1.0f / KEYBOARD_UPDATE_FREQUENCY * 1000.0f;
        LEDTickTimer += KEYBOARD_SLEEP_TIME_MS;
        DisplayTickTimer += KEYBOARD_SLEEP_TIME_MS;
        sleep_ms(KEYBOARD_SLEEP_TIME_MS);
    }

    return 0;
}