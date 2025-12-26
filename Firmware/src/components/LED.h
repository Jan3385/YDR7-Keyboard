#ifndef LED_H
#define LED_H

#include "lib/ws2812/WS2812.hpp"

#include <cstdint>

namespace LED{
    struct RGB{
        RGB() : r(0), g(0), b(0) {};
        RGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {};

        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    
    extern WS2812 LedArray;
    
    /**
     * @brief Set local LED brightness. 0 is off, 100 is maximum brightness defined by LED_BRIGHTNESS_VALUE_CAP
     * @param brightness Brightness value (0-100) (%)
     */
    void SetLocalBrightness(uint8_t brightness);
    /**
     * @brief Set default color for all LEDs
     * @param color Color to set
     * @param duration Duration of the color change in ticks
     */
    void SetBackgroundColor(LED::RGB color, uint16_t duration);
    /**
     * @brief Change color of a specific LED over a duration
     * @param ledIndex Index of the LED to change
     * @param color Target color
     * @param duration Duration of the color change in ticks
     */
    void ChangeColor(uint16_t ledIndex, LED::RGB color, uint16_t duration);

    /**
     * @brief Update LED color changes)
     */
    void Tick();

    /**
     * @brief Apply current LED colors to the physical LEDs
     */
    void Show();
}

#endif