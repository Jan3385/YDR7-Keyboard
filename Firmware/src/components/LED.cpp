#include "LED.h"

#include "setup/pin_setup.h"
#include "config/board.h"

struct ColorChange
{
    uint32_t start;
    uint32_t end;
    int16_t ticksTotal;
    int16_t ticksElapsed = -1;
};
bool IsActiveChange(const ColorChange& change) { return change.ticksElapsed >= 0; }

uint32_t ColorLerp(uint32_t colorA, uint32_t colorB, float t)
{
    // Clamp t
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // Extract user channels (R-G-B-W)
    uint8_t a_r = (colorA) & 0xFF;
    uint8_t a_g = (colorA >> 8) & 0xFF;
    uint8_t a_b = (colorA >> 16) & 0xFF;
    uint8_t a_w = (colorA >> 24) & 0xFF;

    uint8_t b_r = (colorB) & 0xFF;
    uint8_t b_g = (colorB >> 8) & 0xFF;
    uint8_t b_b = (colorB >> 16) & 0xFF;
    uint8_t b_w = (colorB >> 24) & 0xFF;

    // Lerp each channel
    uint8_t r = static_cast<uint8_t>(a_r + (b_r - a_r) * t + 0.5f);
    uint8_t g = static_cast<uint8_t>(a_g + (b_g - a_g) * t + 0.5f);
    uint8_t b = static_cast<uint8_t>(a_b + (b_b - a_b) * t + 0.5f);
    uint8_t w = static_cast<uint8_t>(a_w + (b_w - a_w) * t + 0.5f);

    // Pack back in user format (R-G-B-W)
    return (w << 24) | (b << 16) | (g << 8) | r;
}

namespace LED{
    WS2812 LedArray(
        LED_PIN,
        NUM_OF_LEDS,
        pio0,
        0,
        WS2812::FORMAT_GRB
    );

    static uint8_t brightness = 100;
    static uint32_t backgroundColor = 0;
    static uint16_t backgroundColorTime;
    static ColorChange activeColorChanges[NUM_OF_LEDS];
}

void LED::SetLocalBrightness(uint8_t brightness)
{
    if(brightness > 100) brightness = 100;

    LED::brightness = brightness;
}

void LED::SetBackgroundColor(uint32_t color, uint16_t duration)
{
    backgroundColor = color;
    backgroundColorTime = duration;
}

void LED::ChangeColor(uint16_t ledIndex, uint32_t color, uint16_t duration)
{
    if(ledIndex >= NUM_OF_LEDS) return;

    ColorChange& change = activeColorChanges[ledIndex];
    change.start = LED::LedArray.GetColor(ledIndex);
    change.end = color;
    change.ticksTotal = duration;
    change.ticksElapsed = 0;
}

void LED::Tick()
{
    for(uint16_t i = 0; i < NUM_OF_LEDS; i++){
        ColorChange& change = activeColorChanges[i];
        if(IsActiveChange(change)){
            change.ticksElapsed++;
            if(change.ticksElapsed >= change.ticksTotal){
                LED::LedArray.setPixelColor(i, change.end);
                change.ticksElapsed = -1;

            } else {
                float t = static_cast<float>(change.ticksElapsed) / static_cast<float>(change.ticksTotal);
                uint32_t lerpedColor = ColorLerp(change.start, change.end, t);
                LED::LedArray.setPixelColor(i, lerpedColor);
            }
        } else {
            uint32_t ledColor = LED::LedArray.GetColor(i);
            if(ledColor != backgroundColor){
                LED::ChangeColor(i, backgroundColor, backgroundColorTime);
            }
        }
    }
}

void LED::Show()
{
    LED::LedArray.show();
}
