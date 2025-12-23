#include "WS2812.hpp"
#include "WS2812.pio.h"

#if ENABLE_STDIO
#include <stdio.h>
#endif

WS2812::WS2812(uint pin, uint length, PIO pio, uint sm)  {
    initialize(pin, length, pio, sm, NONE, GREEN, RED, BLUE);
}

WS2812::WS2812(uint pin, uint length, PIO pio, uint sm, DataFormat format) {
    switch (format) {
        case FORMAT_RGB:
            initialize(pin, length, pio, sm, NONE, RED, GREEN, BLUE);
            break;
        case FORMAT_GRB:
            initialize(pin, length, pio, sm, NONE, GREEN, RED, BLUE);
            break;
        case FORMAT_WRGB:
            initialize(pin, length, pio, sm, WHITE, RED, GREEN, BLUE);
            break;
    }
}

WS2812::WS2812(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3) {
    initialize(pin, length, pio, sm, b1, b1, b2, b3);
}

WS2812::WS2812(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3, DataByte b4) {
    initialize(pin, length, pio, sm, b1, b2, b3, b4);
}

WS2812::~WS2812() {
    
}

uint32_t WS2812::GetColor(uint index) const
{
    return convertDataBack(data[index]);
}

void WS2812::initialize(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3, DataByte b4) {
    this->pin = pin;
    this->length = length;
    this->pio = pio;
    this->sm = sm;
    this->data = new uint32_t[length];
    this->bytes[0] = b1;
    this->bytes[1] = b2;
    this->bytes[2] = b3;
    this->bytes[3] = b4;
    uint offset = pio_add_program(pio, &ws2812_program);
    uint bits = (b1 == NONE ? 24 : 32);
    #if ENABLE_STDIO
    printf("WS2812 / Initializing SM %u with offset %X at pin %u and %u data bits...\n", sm, offset, pin, bits);
    #endif
    ws2812_program_init(pio, sm, offset, pin, 800000, bits);
}

uint32_t WS2812::convertData(uint32_t rgbw) {
    uint32_t result = 0;
    for (uint b = 0; b < 4; b++) {
        switch (bytes[b]) {
            case RED:
                result |= (rgbw & 0xFF);
                break;
            case GREEN:
                result |= (rgbw & 0xFF00) >> 8;
                break;
            case BLUE:
                result |= (rgbw & 0xFF0000) >> 16;
                break;
            case WHITE:
                result |= (rgbw & 0xFF000000) >> 24;
                break;
        }
        result <<= 8;
    }
    return result;
}
uint32_t WS2812::convertDataBack(uint32_t data) const {
    // Compensate for extra shift in convertData
    data >>= 8;
    uint8_t r = 0, g = 0, b = 0, w = 0;
    for (int i = 3; i >= 0; i--) {
        uint8_t byte = data & 0xFF;
        switch (bytes[i]) {
            case RED:
                r = byte;
                break;
            case GREEN:
                g = byte;
                break;
            case BLUE:
                b = byte;
                break;
            case WHITE:
                w = byte;
                break;
            default:
                break;
        }
        data >>= 8;
    }
    return ((uint32_t)w << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | r;
}

void WS2812::setPixelColor(uint index, uint32_t color) {
    if (index < length) {
        data[index] = convertData(color);
    }
}

void WS2812::setPixelColor(uint index, uint8_t red, uint8_t green, uint8_t blue) {
    setPixelColor(index, RGB(red, green, blue));
}

void WS2812::setPixelColor(uint index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    setPixelColor(index, RGBW(red, green, blue, white));
}

void WS2812::fill(uint32_t color) {
    fill(color, 0, length);
}

void WS2812::fill(uint32_t color, uint first) {
    fill(color, first, length-first);
}

void WS2812::fill(uint32_t color, uint first, uint count) {
    uint last = (first + count);
    if (last > length) {
        last = length;
    }
    color = convertData(color);
    for (uint i = first; i < last; i++) {
        data[i] = color;
    }
}

void WS2812::show() {
    #ifdef ENABLE_STDIO
    for (uint i = 0; i < length; i++) {
        printf("WS2812 / Put data: %08X\n", data[i]);
    }
    #endif
    for (uint i = 0; i < length; i++) {
        pio_sm_put_blocking(pio, sm, data[i]);
    }
}

void WS2812::pushColor(uint32_t color)
{
    for (int i = length - 1; i > 0; --i) {
        data[i] = data[i - 1];
    }
    data[0] = convertData(color);
}
