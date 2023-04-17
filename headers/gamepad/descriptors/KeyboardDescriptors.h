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

#define KEYBOARD_ENDPOINT_SIZE 64

// HAT1 report (8 bits)
#define KEYBOARD_MASK_LEFT    0x00
#define KEYBOARD_MASK_RIGHT   0xff

// HAT2 report (8 bits)
#define KEYBOARD_MASK_UP      0x00
#define KEYBOARD_MASK_DOWN    0xff

// Buttons 1 (8 bits)
#define KEYBOARD_MASK_X       0x80
#define KEYBOARD_MASK_A       0x40
#define KEYBOARD_MASK_B       0x20
#define KEYBOARD_MASK_Y       0x10

// Buttons 2 (8 bits)
#define KEYBOARD_MASK_C       0x02
#define KEYBOARD_MASK_Z       0x01
#define KEYBOARD_MASK_START   0x20
#define KEYBOARD_MASK_MODE    0x10

/// Standard HID Boot Protocol Keyboard Report.
typedef struct TU_ATTR_PACKED
{
  uint8_t modifier;   /**< Keyboard modifier (KEYBOARD_MODIFIER_* masks). */
  uint8_t reserved;   /**< Reserved for OEM use, always set to 0. */
  uint8_t keycode[6]; /**< Key codes of the currently pressed keys. */
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
  0x00, 0x02,
  0x00,
  0x00,
  0x00,
  CFG_TUD_ENDPOINT0_SIZE,

  0xfe, 0xCa,
  0x01, 0x00,
  0x00, 0x01,

  0x01,
  0x02,
  0x03,

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
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD))
};

// Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
static const uint8_t keyboard_hid_descriptor[] = {
  TUD_HID_DESCRIPTOR(ITF_NUM_HID_KEYBOARD, 0, HID_ITF_PROTOCOL_NONE, sizeof(keyboard_report_descriptor), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)
};

static const uint8_t keyboard_configuration_descriptor[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL_KEYBOARD, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
  
};