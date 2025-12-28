#include "HID_setup.h"

#include "components/Keyboard.h"

uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance)
{
    return HID_DESCRIPTOR;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t type,
                           uint8_t const* buffer, uint16_t bufsize)
{
    if(type == HID_REPORT_TYPE_OUTPUT && bufsize > 0)
    {
        Keyboard::numLockState = (buffer[0] >> 0) & 0x01;
        Keyboard::capsLockState = (buffer[0] >> 1) & 0x01;
        Keyboard::scrollLockState = (buffer[0] >> 2) & 0x01;
    }
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t type,
                               uint8_t* buffer, uint16_t reqlen) { return 0; }