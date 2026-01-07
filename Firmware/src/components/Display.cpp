#include "Display.h"

#include "config/board.h"
#include "Font.h"
#include "components/LED.h"
#include "setup/pin_setup.h"

#include "menu.h"

#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include <malloc.h>

static uint8_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

void Command(uint8_t cmd){
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(OLED_I2C_PORT, OLED_I2C_ADDRESS, buf, 2, false);
}

static float timeSinceLastActivityMs = 0.0f;

void Display::SpecialkeyPressed(uint8_t key)
{
    if(timeSinceLastActivityMs > DISPLAY_SLEEP_TIMEOUT_MS){
        timeSinceLastActivityMs = 0.0f;
        return;
    }
    
    timeSinceLastActivityMs = 0.0f;
    if(key == HID_KEY_SPECIAL_U) Display::BTN_Up();
    if(key == HID_KEY_SPECIAL_D) Display::BTN_Down();
    if(key == HID_KEY_SPECIAL_L) Display::BTN_Left();
    if(key == HID_KEY_SPECIAL_R) Display::BTN_Right();
}

void Display::Setup()
{
    i2c_init(OLED_I2C_PORT, OLED_I2C_FREQUENCY);

    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);

    sleep_ms(100);

    Command(OLED_OFF);
    
    Command(OLED_MEMORY_MODE);
    Command(OLED_MEMORY_MODE_HORIZONTAL);

    //Command(OLED_SEGMENT_REMAP);
    //Command(OLED_COM_SCAN_REVERSE);

    Command(OLED_NORMAL_DISPLAY);

    Command(OLED_MULTIPLEX_RATIO);
    Command(0x7F); // 128MUX

    Command(OLED_ON);

    Clear();
    Show();
}

void Display::Clear()
{
    for (uint32_t i = 0; i < sizeof(buffer); i++)
        buffer[i] = 0x00;
}

void Display::Show() {
    for (uint8_t page = 0; page < 16; page++) {
        Command(0xB0 | page);
        Command(0x00);
        Command(0x10);

        uint8_t data[DISPLAY_WIDTH + 1];
        data[0] = 0x40;

        for (uint8_t col = 0; col < DISPLAY_WIDTH; col++) {
            data[col + 1] = buffer[page * DISPLAY_WIDTH + col];
        }

        i2c_write_blocking(OLED_I2C_PORT, OLED_I2C_ADDRESS, data, sizeof(data), false);
    }
}

void m_ShowInitialText(const char* text, uint8_t &yOffset, uint16_t delayMs)
{
    Display::DrawText(0, yOffset, text, true);
    Display::Show();
    yOffset += FONT_HEIGHT + 1;
    if(yOffset > DISPLAY_HEIGHT - FONT_HEIGHT){
        yOffset -= (FONT_HEIGHT+1);
        Display::MoveVertical(FONT_HEIGHT + 1);
    }

    if(delayMs > 0)
        sleep_ms(delayMs);
}

void Display::InitialScreenWTest()
{
    constexpr uint8_t TEXT_TIME_MS = 60;
    uint8_t yOffset = 0;

    m_ShowInitialText(PRODUCT_NAME, yOffset, 0);

    m_ShowInitialText("by " MANUFACTURER_NAME, yOffset, 0);

    m_ShowInitialText("SERIAL NUM: " SERIAL_NUMBER, yOffset, 0);
    m_ShowInitialText("https://github.com/Jan3385/YDR7-Keyboard", yOffset, 0);
    yOffset += FONT_HEIGHT + 8;

    Display::DrawRect(5, yOffset-4, DISPLAY_WIDTH-10, 1, true);
    sleep_ms(200);

    Command(OLED_INVERT_DISPLAY);
    sleep_ms(80);
    Command(OLED_NORMAL_DISPLAY);
    sleep_ms(10);

    char text[32];
    snprintf(text, sizeof(text), "CPU: %u kHz", clock_get_hz(clk_sys) / 1000);
    m_ShowInitialText(text, yOffset, TEXT_TIME_MS);

#ifdef PICO_FLASH_SIZE_BYTES
    snprintf(text, sizeof(text), "Flash: %lu KB", PICO_FLASH_SIZE_BYTES / 1024);
    m_ShowInitialText(text, yOffset, TEXT_TIME_MS);
#endif

    snprintf(text, sizeof(text), "Uptime: %lu ms", to_ms_since_boot(get_absolute_time()));
    m_ShowInitialText(text, yOffset, TEXT_TIME_MS);

    struct mallinfo mi = mallinfo();
    snprintf(text, sizeof(text), "Heap: %d / %d B", mi.ordblks, mi.fordblks);
    m_ShowInitialText(text, yOffset, 0);
    snprintf(text, sizeof(text), "Total Heap: %d B", mi.arena);
    m_ShowInitialText(text, yOffset, TEXT_TIME_MS);

    // Temperature sensor channel
    float temp = ReadInternalTemperatureC();
    snprintf(text, sizeof(text), "Temp: %.1f C", temp);
    m_ShowInitialText(text, yOffset, TEXT_TIME_MS);

    yOffset += 6;
    Display::DrawRect(5, yOffset-3, DISPLAY_WIDTH-10, 1, true);
    Display::Show();
    sleep_ms(50);

    Display::Clear();
    Display::Show();

    uint8_t halfWidth = DISPLAY_WIDTH / 2 - (strlen(PRODUCT_NAME) * FONT_WIDTH) / 2;
    uint8_t halfHeight = DISPLAY_HEIGHT / 2 - FONT_HEIGHT / 2;
    Command(OLED_INVERT_DISPLAY);
    Display::DrawText(halfWidth, halfHeight, PRODUCT_NAME, true);
    Display::DrawRect(0, DISPLAY_HEIGHT - FONT_HEIGHT, DISPLAY_WIDTH, FONT_HEIGHT, true);
    Display::DrawText(0, DISPLAY_HEIGHT - FONT_HEIGHT, "Waiting for USB protocol", false);
    Display::Show();
}

void Display::ClearInitialScreen()
{
    Command(OLED_NORMAL_DISPLAY);
    Display::Clear();
    Display::Show();
}

void Display::UpdateMenu()
{
    Display::Clear();

    if(timeSinceLastActivityMs >= DISPLAY_SLEEP_TIMEOUT_MS){
        return;
    }

    constexpr uint8_t LONGEST_TEXT_LENGTH = 5;
    const char* menus[] = {
        "INFO",
        "RGB",
        "THEME",
        "BOOT"
    };

    Display::DrawRect(1, 1 + Display::GetSelectedMenuIndex() * (FONT_HEIGHT + 5), 
        MENU_SIDE_OFFSET - 1, FONT_HEIGHT + 2, true);

    Display::DrawTextWithBorder(0, 0,
        menus[0], 1, MENU_SIDE_OFFSET);
    Display::DrawTextWithBorder(0, FONT_HEIGHT + 5,
        menus[1], 1, MENU_SIDE_OFFSET);
    Display::DrawTextWithBorder(0, 2 * (FONT_HEIGHT + 5),
        menus[2], 1, MENU_SIDE_OFFSET);
    Display::DrawTextWithBorder(0, 3 * (FONT_HEIGHT + 5),
        menus[3], 1, MENU_SIDE_OFFSET);
    
    Display::RenderCtxScreen();

    timeSinceLastActivityMs += 1000.0f / DISPLAY_UPDATE_FREQUENCY;
}

void Display::MoveVertical(uint8_t offset)
{
    if (offset == 0 || offset >= DISPLAY_HEIGHT) {
        Clear();
        return;
    }

    for (uint8_t x = 0; x < DISPLAY_WIDTH; x++) {
        for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++) {
            uint8_t srcY = y + offset;
            bool pixel = false;

            // get pixel from srcY
            if (srcY < DISPLAY_HEIGHT) {
                uint16_t srcIndex = x + (srcY / 8) * DISPLAY_WIDTH;
                uint8_t srcBit = 1 << (srcY % 8);
                pixel = (buffer[srcIndex] & srcBit) != 0;
            }

            uint16_t dstIndex = x + (y / 8) * DISPLAY_WIDTH;
            uint8_t dstBit = 1 << (y % 8);
            if (pixel)
                buffer[dstIndex] |= dstBit;
            else
                buffer[dstIndex] &= ~dstBit;
        }
    }

    // Clear the bottom
    for (uint8_t y = DISPLAY_HEIGHT - offset; y < DISPLAY_HEIGHT; y++) {
        for (uint8_t x = 0; x < DISPLAY_WIDTH; x++) {
            uint16_t idx = x + (y / 8) * DISPLAY_WIDTH;
            uint8_t bit = 1 << (y % 8);
            buffer[idx] &= ~bit;
        }
    }
}

void Display::DrawPixel(uint8_t x, uint8_t y, bool on) {
    if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;

    uint16_t byteIndex = x + (y / 8) * DISPLAY_WIDTH;
    uint8_t bitIndex = 1 << (y % 8);

    on  ? buffer[byteIndex] |= bitIndex 
        : buffer[byteIndex] &= ~bitIndex;
}

void Display::FlipPixel(uint8_t x, uint8_t y)
{
    if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;

    uint16_t byteIndex = x + (y / 8) * DISPLAY_WIDTH;
    uint8_t bitIndex = 1 << (y % 8);

    buffer[byteIndex] ^= bitIndex;
}

void Display::DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool on)
{
    for(uint8_t i = 0; i < width; i++){
        for(uint8_t j = 0; j < height; j++){
            DrawPixel(x + i, y + j, on);
        }
    }
}

void Display::DrawBorder(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t thickness, bool on)
{
    DrawRect(x, y, width, thickness, on);
    DrawRect(x, y + height - thickness, width, thickness, on);

    DrawRect(x, y, thickness, height, on);
    DrawRect(x + width - thickness, y, thickness, height, on);
}

void Display::DrawChar(uint8_t x, uint8_t y, char c, bool on)
{
    if(c < 32 || c > 127) return;

    const uint8_t* charData = &Font[(c - 32) * FONT_WIDTH];

    for(uint8_t col = 0; col < FONT_WIDTH; col++){
        uint8_t line = charData[col];
        for(uint8_t row = 0; row < FONT_HEIGHT; row++){
            bool pixelOn = line & 0x01;
            if(pixelOn){
                DrawPixel(
                    x + col, 
                    y + row, 
                    on
                );
            }
            line >>= 1;
        }
    }
}

void Display::DrawCharNeg(uint8_t x, uint8_t y, char c)
{
    if(c < 32 || c > 127) return;

    const uint8_t* charData = &Font[(c - 32) * FONT_WIDTH];

    for(uint8_t col = 0; col < FONT_WIDTH; col++){
        uint8_t line = charData[col];
        for(uint8_t row = 0; row < FONT_HEIGHT; row++){
            bool pixelOn = line & 0x01;
            if(pixelOn){
                FlipPixel(
                    x + col, 
                    y + row
                );
            }
            line >>= 1;
        }
    }
}

void Display::DrawText(uint8_t x, uint8_t y, const char *text, bool on)
{
    uint8_t cursorX = x;
    uint8_t cursorY = y;

    for(size_t i = 0; text[i] != '\0'; i++){
        char c = text[i];
        if(c == '\n'){
            cursorX = x;
            cursorY += FONT_HEIGHT+1;
        } else {
            DrawChar(cursorX, cursorY, c, on);
            cursorX += FONT_WIDTH;
            if(cursorX + FONT_WIDTH > DISPLAY_WIDTH){
                cursorX = x;
                cursorY += FONT_HEIGHT+1;
            }
        }
    }
}

void Display::DrawTextCenter(uint8_t x, uint8_t y, const char *text, bool on)
{
    uint8_t textLength = strlen(text);
    uint8_t startX = x - (textLength * FONT_WIDTH) / 2;
    DrawText(startX, y, text, on);
}

void Display::DrawTextNeg(uint8_t x, uint8_t y, const char *text)
{
    uint8_t cursorX = x;
    uint8_t cursorY = y;

    for(size_t i = 0; text[i] != '\0'; i++){
        char c = text[i];
        if(c == '\n'){
            cursorX = x;
            cursorY += FONT_HEIGHT+1;
        } else {
            DrawCharNeg(cursorX, cursorY, c);
            cursorX += FONT_WIDTH;
            if(cursorX + FONT_WIDTH > DISPLAY_WIDTH){
                cursorX = x;
                cursorY += FONT_HEIGHT+1;
            }
        }
    }
}

void Display::DrawTextWithBorder(uint8_t x, uint8_t y, const char *text, uint8_t borderThickness, uint8_t minBorderWidth)
{
    size_t textLength = strlen(text);

    uint8_t textWidth = textLength * FONT_WIDTH;
    if(textWidth < minBorderWidth)
        textWidth = minBorderWidth;

    uint8_t textHeight = FONT_HEIGHT;

    DrawBorder(x, y, 
        textWidth + borderThickness - 1, textHeight + borderThickness + 3, 
        borderThickness, true);

    DrawTextNeg(x+borderThickness+1, y+borderThickness+1, text);
}
