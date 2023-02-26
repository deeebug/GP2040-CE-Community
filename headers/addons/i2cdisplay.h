/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <unordered_map>
#include <string>
#include <hardware/i2c.h>
#include "OneBitDisplay.h"
#include "BoardConfig.h"
#include "gpaddon.h"
#include "gamepad.h"
#include "storagemanager.h"
#include "display_button_layouts.h"

#ifndef HAS_I2C_DISPLAY
#define HAS_I2C_DISPLAY -1
#endif

#ifndef DISPLAY_I2C_ADDR
#define DISPLAY_I2C_ADDR 0x3C
#endif

#ifndef I2C_SDA_PIN
#define I2C_SDA_PIN -1
#endif

#ifndef I2C_SCL_PIN
#define I2C_SCL_PIN -1
#endif

#ifndef I2C_BLOCK
#define I2C_BLOCK i2c0
#endif

#ifndef I2C_SPEED
#define I2C_SPEED 800000
#endif

#ifndef DISPLAY_SIZE
#define DISPLAY_SIZE OLED_128x64
#endif

#ifndef DISPLAY_FLIP
#define DISPLAY_FLIP 0
#endif

#ifndef DISPLAY_INVERT
#define DISPLAY_INVERT 0
#endif

#ifndef DISPLAY_USEWIRE
#define DISPLAY_USEWIRE 1
#endif

#ifndef DISPLAY_SAVER_TIMEOUT
#define DISPLAY_SAVER_TIMEOUT 0
#endif

#ifndef SPLASH_DURATION
#define SPLASH_DURATION 7500 // Duration in milliseconds
#endif

// i2c Display Module
#define I2CDisplayName "I2CDisplay"

enum DisplayPreviewMode {
	PREVIEW_MODE_NONE,
	PREVIEW_MODE_BUTTONS,
	PREVIEW_MODE_SPLASH
};

// i2C OLED Display
class I2CDisplayAddon : public GPAddon
{
public:
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
	virtual std::string name() { return I2CDisplayName; }
private:
	int initDisplay(int typeOverride);
	bool isSH1106(int detectedDisplay);
	void clearScreen(int render); // DisplayModule
	void drawStatusBar(Gamepad*);
	void drawText(int startX, int startY, std::string text);
	void initMenu(char**);
	//Adding my stuff here, remember to sort before PR
	void drawSplashScreen(int splashMode, uint8_t* splashChoice, int splashSpeed);
	BoardOptions getBoardOptions();
	bool isDisplayPowerOff();
	void setDisplayPower(uint8_t status);
	uint32_t displaySaverTimeout = 0;
	int32_t displaySaverTimer;
	uint8_t displayIsPowerOn = 1;
	uint32_t prevMillis;
	uint8_t ucBackBuffer[1024];
	OBDISP obd;
	std::string statusBar;
	Gamepad* gamepad;
	Gamepad* pGamepad;
	private:
	DisplayPreviewMode displayPreviewMode;
	uint16_t prevButtonState;
};

#endif
