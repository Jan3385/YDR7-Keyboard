#ifndef TUD_SETUP_H
#define TUD_SETUP_H

// USB device identity

#include "bsp/board.h"
#include "tusb.h"

tusb_desc_device_t const DESC_DEVICE = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,

    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,

    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = USB_VENDOR_ID,
    .idProduct          = PRODUCT_ID,
    .bcdDevice          = DEVICE_RELEASE_NUMBER,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

uint8_t const *tud_descriptor_device_cb(void) {
    return (uint8_t const *)&DESC_DEVICE;
};

enum { ITF_NUM_HID, ITF_NUM_TOTAL };

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define EPNUM_HID         0x81

uint8_t const desc_configuration[] = {
    // Config descriptor, HID interface
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_KEYBOARD, sizeof(HID_DESCRIPTOR), EPNUM_HID, 8, 10)
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    return desc_configuration;
}

char const* DESC_ARR_INFO[] = {
    (const char[]){0x09, 0x04},      // 0: supported language is English (0x0409)
    MANUFACTURER_NAME,               // 1: Manufacturer
    PRODUCT_NAME,                    // 2: Product
    SERIAL_NUMBER,                   // 3: Serial
};

static uint16_t DESC_STR[32];

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    uint8_t charCount;

    if (index == 0) {
        memcpy(&DESC_STR[1], DESC_ARR_INFO[0], 2);
        charCount = 1;
    } else {
        const char* str = DESC_ARR_INFO[index];
        charCount = strlen(str);
        if (charCount > 31) charCount = 31;
        for (uint8_t i = 0; i < charCount; i++) {
            DESC_STR[1 + i] = str[i];
        }
    }

    DESC_STR[0] = (TUSB_DESC_STRING << 8) | (2 * charCount + 2);
    return DESC_STR;
}

#endif