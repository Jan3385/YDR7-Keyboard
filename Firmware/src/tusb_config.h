#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// Common configuration
#define CFG_TUSB_MCU OPT_MCU_RP2040
#define CFG_TUSB_OS  OPT_OS_PICO
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

// Device configuration
#define CFG_TUD_ENDPOINT0_SIZE 64

// -------- CLASS DRIVER CONFIGURATION --------

// HID (for keyboard)
#define CFG_TUD_HID 1       // Human Interface Device enabled
#define CFG_TUD_CDC 0       // USB to Serial disabled
#define CFG_TUD_MSC 0       // Mass Storage disabled
#define CFG_TUD_MIDI 0      // Musical Instrument Digital Interface disabled
#define CFG_TUD_VENDOR 0    // Vendor Class disabled

// HID buffer sizes
#define CFG_TUD_HID_EP_BUFSIZE 16

#endif