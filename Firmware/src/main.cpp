#include <pico/stdlib.h>
#include <stdio.h> // Add for debug output

#include "config/user_config.h"

#include "setup/HID_setup.h"
#include "setup/TUD_setup.h"
#include "setup/pin_setup.h"

int main(){
    #if ENABLE_STDIO
    stdio_init_all();
    #endif

    PinSetup_ALL();

    tusb_init();

    gpio_put(K_COL3_PIN, 1); // For testing: set one column high

    while (true) {
        tud_task();

        if( !tud_hid_ready() ) continue;

        if (gpio_get(K_ROW5_PIN)) {
            uint8_t report[6] = { HID_KEY_A };
            tud_hid_keyboard_report(0, 0, report);
            LED::LedArray.fill(WS2812::RGB(200,0,0));
            LED::LedArray.show();
        } else {
            tud_hid_keyboard_report(0, 0, nullptr);
            LED::LedArray.fill(WS2812::RGB(0,0,0));
            LED::LedArray.show();
        }

        sleep_ms(10);
    }

    return 0;
}