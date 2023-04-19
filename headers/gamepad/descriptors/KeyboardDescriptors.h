#pragma once

#include <stdint.h>
#include "tusb.h"

#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

enum {
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_MOUSE,
  REPORT_ID_CONSUMER_CONTROL,
  REPORT_ID_GAMEPAD,
  REPORT_ID_COUNT
};

#endif /* USB_DESCRIPTORS_H_ */
#define REPORT_BYTES 8

/// Standard HID Boot Protocol Keyboard Report.
typedef struct TU_ATTR_PACKED
{
  uint8_t modifier;   /**< Keyboard modifier (KEYBOARD_MODIFIER_* masks). */
 // uint8_t reserved;   /**< Reserved for OEM use, always set to 0. */
  uint8_t keycode[REPORT_BYTES - 1]; /**< Key codes of the currently pressed keys. */
} KeyboardReport;

static const uint8_t keyboard_string_language[]    = { 0x09, 0x04 };
static const uint8_t keyboard_string_manfacturer[] = "Open Stick Community";
static const uint8_t keyboard_string_product[]     = "GP2040-CE (Keyboard)";
static const uint8_t keyboard_string_version[]     = "1.0";

static const uint8_t *keyboard_string_descriptors[] =
{
	keyboard_string_language,
	keyboard_string_manfacturer,
	keyboard_string_product,
	keyboard_string_version
};

static const uint8_t keyboard_device_descriptor[] =
{
	sizeof(tusb_desc_device_t),
  TUSB_DESC_DEVICE,
  0x10, 0x01,
  0x00,
  0x00,
  0x00,
  64,
  0xfe, 0xca,
  0x01, 0x00,
  0x00, 0x01,
  0x01,
  0x02,
  0x00,
  0x01
};

enum
{
  ITF_NUM_HID_KEYBOARD,
  ITF_NUM_TOTAL_KEYBOARD
};

#define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#define EPNUM_HID   0x81

static const uint8_t keyboard_report_descriptor[] = {
  0x05, 0x01,                     // Usage Page (Generic Desktop),
        0x09, 0x06,                     // Usage (Keyboard),
        0xA1, 0x01,                     // Collection (Application),
        // bitmap of modifiers
        0x75, 0x01,                     //   Report Size (1),
        0x95, 0x08,                     //   Report Count (8),
        0x05, 0x07,                     //   Usage Page (Key Codes),
        0x19, 0xE0,                     //   Usage Minimum (224),
        0x29, 0xE7,                     //   Usage Maximum (231),
        0x15, 0x00,                     //   Logical Minimum (0),
        0x25, 0x01,                     //   Logical Maximum (1),
        0x81, 0x02,                     //   Input (Data, Variable, Absolute), ;Modifier byte
        // LED output report
        0x95, 0x05,                     //   Report Count (5),
        0x75, 0x01,                     //   Report Size (1),
        0x05, 0x08,                     //   Usage Page (LEDs),
        0x19, 0x01,                     //   Usage Minimum (1),
        0x29, 0x05,                     //   Usage Maximum (5),
        0x91, 0x02,                     //   Output (Data, Variable, Absolute),
        0x95, 0x01,                     //   Report Count (1),
        0x75, 0x03,                     //   Report Size (3),
        0x91, 0x03,                     //   Output (Constant),
        // bitmap of keys
        0x95, (REPORT_BYTES-1)*8,	//   Report Count (),
        0x75, 0x01,                     //   Report Size (1),
        0x15, 0x00,                     //   Logical Minimum (0),
        0x25, 0x01,                     //   Logical Maximum(1),
        0x05, 0x07,                     //   Usage Page (Key Codes),
        0x19, 0x00,                     //   Usage Minimum (0),
        0x29, (REPORT_BYTES-1)*8-1,	//   Usage Maximum (),
        0x81, 0x02,                     //   Input (Data, Variable, Absolute),
        0xc0                            // End Collection
};

// Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
static const uint8_t keyboard_hid_descriptor[] = {
    0x09,								 // bLength
		0x21,								 // bDescriptorType (HID)
		0x11, 0x01,							 // bcdHID 1.11
		0x00,								 // bCountryCode
		0x01,								 // bNumDescriptors
		0x22,								 // bDescriptorType[0] (HID)
		sizeof(keyboard_report_descriptor), 0x00, // wDescriptorLength[0] 90
};

static const uint8_t keyboard_configuration_descriptor[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL_KEYBOARD, 0, CONFIG_TOTAL_LEN, 32, 100),

  // Interface number, string index, protocol, report descriptor len, EP Out & In address, size & polling interval
  TUD_HID_DESCRIPTOR(ITF_NUM_HID_KEYBOARD, 0, HID_ITF_PROTOCOL_KEYBOARD, sizeof(keyboard_report_descriptor), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 1)
};