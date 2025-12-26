#ifndef PIN_SETUP_H
#define PIN_SETUP_H

#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "config/user_config.h"
#include "config/board.h"
#include "components/Display.h"

namespace Keyboard{
    inline void Setup(){
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

inline float ReadInternalVoltage(){
    adc_select_input(3);
    uint16_t raw = adc_read();

    return raw * 3.3f / (1 << 12) * 3.0f;
}

inline float ReadInternalTemperatureC(){
    adc_select_input(4);
    uint16_t raw = adc_read();

    // some evil math
    float voltage = raw * 3.3f / (1 << 12);
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

inline void PinSetup_ALL(){
    Display::Setup();
    Keyboard::Setup();

    // Setup ADC for temperature reading
    adc_init();
    adc_set_temp_sensor_enabled(true);

    // Unpower unused pins
    const uint8_t unused_pins[] = UNUSED_PINS;
    for(uint8_t i = 0; i < UNUSED_PINS_COUNT; i++){
        gpio_init(unused_pins[i]);
        gpio_set_dir(unused_pins[i], GPIO_IN);
        gpio_put(unused_pins[i], 0);
    }
};

#endif