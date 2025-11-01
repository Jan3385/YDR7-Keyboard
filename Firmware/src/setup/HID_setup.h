#ifndef HID_SETUP_H
#define HID_SETUP_H

// HID descriptor and callbacks

#include "tusb.h"

uint8_t const HID_DESCRIPTOR[] = { TUD_HID_REPORT_DESC_KEYBOARD() };
uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance)
{
    return HID_DESCRIPTOR;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t type,
                           uint8_t const* buffer, uint16_t bufsize)
{
    // Handle Num-Lock, Caps-Lock, Scroll-Lock etc
}

// No response needed
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t type,
                               uint8_t* buffer, uint16_t reqlen) { return 0; }

#endif