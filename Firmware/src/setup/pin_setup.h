#ifndef PIN_SETUP_H
#define PIN_SETUP_H

#include "hardware/i2c.h"
#include "config/board.h"

namespace Keyboard{
    void Setup(){
        // column pins as output
        for(uint8_t c = 0; c < K_COLS; c++){
            gpio_init(K_COL_PINS[c]);
            gpio_set_dir(K_COL_PINS[c], GPIO_OUT);
            gpio_put(K_COL_PINS[c], 1);
        }

        // row pins as input
        for(uint8_t r = 0; r < K_ROWS; r++){
            gpio_init(K_ROW_PINS[r]);
            gpio_set_dir(K_ROW_PINS[r], GPIO_IN);
            gpio_pull_up(K_ROW_PINS[r]);
        }
    }
}
namespace Display{
    void Setup(){
        i2c_init(OLED_I2C_PORT, OLED_I2C_FREQUENCY);

        gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(OLED_SDA_PIN);
        gpio_pull_up(OLED_SCL_PIN);
    }
}
namespace LED{
    void Setup(){
        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
    }
}

void PinSetup_ALL(){
    Keyboard::Setup();
    Display::Setup();
    LED::Setup();
};

#endif