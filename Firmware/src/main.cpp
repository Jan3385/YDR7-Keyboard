#include <pico/stdlib.h>

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

    while (true) {
        tud_task();

        uint8_t report[6] = { HID_KEY_A };
        tud_hid_keyboard_report(0, 0, report);

        sleep_ms(50); // delay between presses

        // release all keys
        tud_hid_keyboard_report(0, 0, nullptr);

        sleep_ms(10);
    }

    return 0;
}