#ifndef HID_SETUP_H
#define HID_SETUP_H

// HID descriptor and callbacks

#include "tusb.h"

static const uint8_t HID_DESCRIPTOR[] = { TUD_HID_REPORT_DESC_KEYBOARD() };
uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance);

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t type,
                           uint8_t const* buffer, uint16_t bufsize);

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t type,
                               uint8_t* buffer, uint16_t reqlen);

#endif