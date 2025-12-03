#ifndef PIN_SETUP_H
#define PIN_SETUP_H

#include "hardware/i2c.h"
#include "config/board.h"

#include "lib/ws2812/WS2812.hpp"

namespace Keyboard{
    void Setup(){
        // row pins as input
        for(uint8_t r = 0; r < K_ROWS; r++){
            gpio_init(K_ROW_PINS[r]);
            gpio_set_dir(K_ROW_PINS[r], GPIO_IN);
            gpio_pull_down(K_ROW_PINS[r]);
        }

        // column pins as output
        for(uint8_t c = 0; c < K_COLS; c++){
            gpio_init(K_COL_PINS[c]);
            gpio_set_dir(K_COL_PINS[c], GPIO_OUT);
            gpio_put(K_COL_PINS[c], 0);
        }
    }
}
namespace Display{
    void Setup(){
        i2c_init(OLED_I2C_PORT, OLED_I2C_FREQUENCY);

        gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    }
}
namespace LED{
    WS2812 LedArray(
        LED_PIN, 
        NUM_OF_LEDS, 
        pio0, 
        0, 
        WS2812::FORMAT_GRB
    );
    void Setup(){
        LedArray.fill(WS2812::RGB(0,0,0));
        LedArray.show();
    }
}

void PinSetup_ALL(){
    Keyboard::Setup();
    Display::Setup();
    LED::Setup();

    // Unpower unused pins
    const uint8_t unused_pins[] = UNUSED_PINS;
    for(uint8_t i = 0; i < UNUSED_PINS_COUNT; i++){
        gpio_init(unused_pins[i]);
        gpio_set_dir(unused_pins[i], GPIO_IN);
        gpio_put(unused_pins[i], 0);
    }
};

#endif