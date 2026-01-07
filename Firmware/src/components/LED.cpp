#include "LED.h"

#include "components/Keyboard.h"
#include "setup/pin_setup.h"
#include "config/board.h"
#include "config/themes.h"

struct ColorChange
{
    uint32_t start;
    uint32_t end;
    int16_t ticksTotal;
    int16_t ticksElapsed = -1;
};
bool IsActiveChange(const ColorChange& change) { return change.ticksElapsed >= 0; }

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

uint32_t RGBToPackedColor(const LED::RGB& color)
{
    static constexpr float COLOR_TO_RGB_CAP = LED_BRIGHTNESS_VALUE_CAP / 255.0f;

    uint16_t totalRGBValue = ((color.r + color.g + color.b) * COLOR_TO_RGB_CAP);
    float globalScale = 1.0f;
    if(totalRGBValue > LED_BRIGHTNESS_VALUE_CAP){
        globalScale = static_cast<float>(LED_BRIGHTNESS_VALUE_CAP) / static_cast<float>(totalRGBValue * 3.0f);
    }

    const float brightnessScale = static_cast<float>(LED::brightness) / 100.0f;
    return WS2812::RGB(
        color.r * globalScale * brightnessScale,
        color.g * globalScale * brightnessScale,
        color.b * globalScale * brightnessScale
    );
}

void LED::SetLocalBrightness(uint8_t brightness)
{
    if(brightness > 100) brightness = 100;

    LED::brightness = brightness;
}

void LED::SetBackgroundColor(LED::RGB color, uint16_t duration)
{
    uint32_t packedColor = RGBToPackedColor(color);

    backgroundColor = packedColor;
    backgroundColorTime = duration;
}

void LED::ChangeColor(uint16_t ledIndex, LED::RGB color, uint16_t duration)
{
    if(ledIndex >= NUM_OF_LEDS) return;

    ColorChange& change = activeColorChanges[ledIndex];
    change.start = LED::LedArray.GetColor(ledIndex);

    change.end = RGBToPackedColor(color);

    change.ticksTotal = duration;
    change.ticksElapsed = 0;
}
void m_ChangeColor(uint16_t ledIndex, uint32_t color, uint16_t duration)
{
    if(ledIndex >= NUM_OF_LEDS) return;

    ColorChange& change = LED::activeColorChanges[ledIndex];
    change.start = LED::LedArray.GetColor(ledIndex);
    change.end = color;
    change.ticksTotal = duration;
    change.ticksElapsed = 0;
}

constexpr uint8_t NUM_SPECIAL_LEDS = 3;
static const uint8_t specialLED[NUM_SPECIAL_LEDS] = {
    K_CAPSLOCK_LED_INDEX,
    K_NUMLOCK_LED_INDEX,
    K_SCROLLLOCK_LED_INDEX
};
bool IsNumberInArray(uint8_t number, const uint8_t* array, size_t arraySize)
{
    for(size_t i = 0; i < arraySize; i++){
        if(array[i] == number) return true;
    }
    return false;
}

void HandleSpecialLED(uint8_t ledIndex){
    if(ledIndex == K_CAPSLOCK_LED_INDEX && Keyboard::capsLockState){
        LED::LedArray.setPixelColor(ledIndex, RGBToPackedColor(Theme::GetPassiveActiveColor()));
    } else if(ledIndex == K_NUMLOCK_LED_INDEX && Keyboard::numLockState){
        LED::LedArray.setPixelColor(ledIndex, RGBToPackedColor(Theme::GetPassiveActiveColor()));
    } else if(ledIndex == K_SCROLLLOCK_LED_INDEX && Keyboard::scrollLockState){
        LED::LedArray.setPixelColor(ledIndex, RGBToPackedColor(Theme::GetPassiveActiveColor()));
    } else{
        LED::LedArray.setPixelColor(ledIndex, LED::backgroundColor);
    }
}

void LED::Tick()
{
    for(uint16_t i = 0; i < NUM_OF_LEDS; i++){
        if(IsNumberInArray(i, specialLED, NUM_SPECIAL_LEDS) && i != 255){
            HandleSpecialLED(i);
            continue;
        }

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
                m_ChangeColor(i, backgroundColor, backgroundColorTime);
            }
        }
    }
}

void LED::Show()
{
    LED::LedArray.show();
}
