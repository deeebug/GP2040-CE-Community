/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#pragma once

#include <stdint.h>
#include "GamepadEnums.h"

struct GamepadOptions
{
	InputMode inputMode {InputMode::INPUT_MODE_XINPUT}; 
	DpadMode dpadMode {DpadMode::DPAD_MODE_DIGITAL};
	SOCDMode socdMode {SOCDMode::SOCD_MODE_NEUTRAL};
	bool invertXAxis;
	bool invertYAxis;

	uint8_t keyDpadUp;
	uint8_t keyDpadDown;
	uint8_t keyDpadLeft;
	uint8_t keyDpadRight;
	uint8_t keyButtonB1;
	uint8_t keyButtonB2;
	uint8_t keyButtonB3;
	uint8_t keyButtonB4;
	uint8_t keyButtonL1;
	uint8_t keyButtonR1;
	uint8_t keyButtonL2;
	uint8_t keyButtonR2;
	uint8_t keyButtonS1;
	uint8_t keyButtonS2;
	uint8_t keyButtonL3;
	uint8_t keyButtonR3;
	uint8_t keyButtonA1;
	uint8_t keyButtonA2;

	uint32_t checksum;
};
