/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#pragma once

#include "gamepad/GamepadDescriptors.h"
#include "enums.pb.h"

typedef enum
{
	USB_MODE_HID,
	USB_MODE_NET,
} UsbMode;

InputMode get_input_mode(void);
uint32_t get_xinput_subtype(void);
bool get_usb_mounted(void);
void initialize_driver(InputMode mode, XInputSubtype xinputSubtype);
void receive_report(uint8_t *buffer);
void send_report(void *report, uint16_t report_size);

