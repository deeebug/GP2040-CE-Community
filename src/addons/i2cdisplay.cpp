/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "addons/i2cdisplay.h"
#include "enums.h"
#include "helper.h"
#include "storagemanager.h"
#include "pico/stdlib.h"
#include "bitmaps.h"

bool I2CDisplayAddon::available() {
	BoardOptions boardOptions = getBoardOptions();
	return boardOptions.hasI2CDisplay && 
		boardOptions.i2cSDAPin != (uint8_t)-1 && 
		boardOptions.i2cSCLPin != (uint8_t)-1;
}

void I2CDisplayAddon::setup() {
	displayPreviewMode = PREVIEW_MODE_NONE;
	prevButtonState = 0;
	BoardOptions boardOptions = getBoardOptions();
	obdI2CInit(&obd,
	    boardOptions.displaySize,
		boardOptions.displayI2CAddress,
		boardOptions.displayFlip,
		boardOptions.displayInvert,
		DISPLAY_USEWIRE,
		boardOptions.i2cSDAPin,
		boardOptions.i2cSCLPin,
		boardOptions.i2cBlock == 0 ? i2c0 : i2c1,
		-1,
		boardOptions.i2cSpeed);
	const int detectedDisplay = initDisplay(0);
	if (isSH1106(detectedDisplay)) {
		// The display is actually a SH1106 that was misdetected as a SSD1306 by OneBitDisplay.
		// Reinitialize as SH1106.
		initDisplay(OLED_132x64);
	}
 
	displayPreviewMode = PREVIEW_MODE_NONE;
	prevButtonState = 0;
	displaySaverTimeout = displaySaverTimer = boardOptions.displaySaverTimeout * 60000; // minute to ms
	
	obdSetContrast(&obd, 0xFF);
	obdSetBackBuffer(&obd, ucBackBuffer);
	clearScreen(1);
	gamepad = Storage::getInstance().GetGamepad();
	pGamepad = Storage::getInstance().GetProcessedGamepad();	
}

bool I2CDisplayAddon::isDisplayPowerOff()
{
	if (!displaySaverTimeout) return false;
	
	float diffTime = getMillis() - prevMillis;
	displaySaverTimer -= diffTime;

	if (gamepad->state.buttons || gamepad->state.dpad) {
		displaySaverTimer = displaySaverTimeout;
		setDisplayPower(1);
	} else if (displaySaverTimer <= 0) {
		setDisplayPower(0);
	}

	prevMillis = getMillis();

	return displaySaverTimer <= 0;
}

void I2CDisplayAddon::setDisplayPower(uint8_t status)
{
	if (displayIsPowerOn != status) {
		displayIsPowerOn = status;
		obdPower(&obd, status);
	}
}

void I2CDisplayAddon::process() {
	bool configMode = Storage::getInstance().GetConfigMode();

	if (!configMode && isDisplayPowerOff()) return;

	int splashDuration = Storage::getInstance().GetSplashDuration();
	splashDuration = splashDuration == 0 ? SPLASH_DURATION : splashDuration;

	clearScreen(0);
	if (configMode) {
		gamepad->read();
		uint16_t buttonState = gamepad->state.buttons;
		if (prevButtonState && !buttonState) {
				if (prevButtonState == GAMEPAD_MASK_B1)
					displayPreviewMode = displayPreviewMode == PREVIEW_MODE_BUTTONS ? PREVIEW_MODE_NONE : PREVIEW_MODE_BUTTONS;
				else if (prevButtonState == GAMEPAD_MASK_B2)
					displayPreviewMode = displayPreviewMode == PREVIEW_MODE_SPLASH ? PREVIEW_MODE_NONE : PREVIEW_MODE_SPLASH;
				else
					displayPreviewMode = PREVIEW_MODE_NONE;
		}
		prevButtonState = buttonState;
	}

	if (configMode && displayPreviewMode == PREVIEW_MODE_NONE) {
		drawStatusBar(gamepad);
		drawText(0, 2, "[Web Config Mode]");
		drawText(0, 3, std::string("GP2040-CE : ") + std::string(GP2040VERSION));
		drawText(0, 4, "[http://192.168.7.1]");
		drawText(0, 5, "Preview:");
		drawText(5, 6, "B1 > Button");
		drawText(5, 7, "B2 > Splash");
	} else if ((configMode && displayPreviewMode == PREVIEW_MODE_SPLASH) ||
			   (!configMode && (splashDuration == -1 || getMillis() < splashDuration)
			    && Storage::getInstance().GetSplashMode() != NOSPLASH)) {
		const uint8_t* splashChoice = Storage::getInstance().getSplashImage().data;
		drawSplashScreen(Storage::getInstance().GetSplashMode(), (uint8_t *)splashChoice, 90);
	} else {
		drawStatusBar(gamepad);

		DisplayButtonLayout* left = getDisplayButtonLayouts()[(getBoardOptions().buttonLayout)];
		DisplayButtonLayoutParams paramsLeft = getBoardOptions().displayButtonLayoutParams;
		left->draw(obd, gamepad, pGamepad, paramsLeft.startX, paramsLeft.startY, paramsLeft.buttonRadius, paramsLeft.buttonPadding);
		DisplayButtonLayout* right = getDisplayButtonLayoutsRight()[(getBoardOptions().buttonLayoutRight)];
		DisplayButtonLayoutParams paramsRight = getBoardOptions().displayButtonLayoutParamsRight;
		right->draw(obd, gamepad, pGamepad, paramsRight.startX, paramsRight.startY, paramsRight.buttonRadius, paramsRight.buttonPadding);
	}

	obdDumpBuffer(&obd, NULL);
}

BoardOptions I2CDisplayAddon::getBoardOptions() {
	bool configMode = Storage::getInstance().GetConfigMode();	
	return configMode ? Storage::getInstance().getPreviewBoardOptions() : Storage::getInstance().getBoardOptions();
}

int I2CDisplayAddon::initDisplay(int typeOverride) {
	BoardOptions boardOptions = Storage::getInstance().getBoardOptions();
	return obdI2CInit(&obd,
	    typeOverride > 0 ? typeOverride : boardOptions.displaySize,
		boardOptions.displayI2CAddress,
		boardOptions.displayFlip,
		boardOptions.displayInvert,
		DISPLAY_USEWIRE,
		boardOptions.i2cSDAPin,
		boardOptions.i2cSCLPin,
		boardOptions.i2cBlock == 0 ? i2c0 : i2c1,
		-1,
		boardOptions.i2cSpeed);
}

bool I2CDisplayAddon::isSH1106(int detectedDisplay) {
	// Only attempt detection if we think we are using a SSD1306 or if auto-detection failed.
	if (detectedDisplay != OLED_SSD1306_3C &&
		detectedDisplay != OLED_SSD1306_3D &&
		detectedDisplay != OLED_NOT_FOUND) {
		return false;
	}

	// To detect an SH1106 we make use of the fact that SH1106 supports read-modify-write operations over I2C, whereas
	// SSD1306 does not.
	// We perform a number of read-modify-write operations and check whether the data we read back matches the data we
	// previously wrote. If it does we can be reasonably confident that we are using a SH1106.

	// We turn the display off for the remainder of this function, we do not want users to observe the random data we
	// are writing.
	obdPower(&obd, false);

	const uint8_t RANDOM_DATA[] = { 0xbf, 0x88, 0x13, 0x41, 0x00 };
	uint8_t buffer[4];
	int i = 0;
	for (; i < sizeof(RANDOM_DATA); ++i) {
		buffer[0] = 0x80; // one command
		buffer[1] = 0xE0; // read-modify-write
		buffer[2] = 0xC0; // one data
		if (I2CWrite(&obd.bbi2c, obd.oled_addr, buffer, 3) == 0) {
			break;
		}

		// Read two bytes back, the first byte is a dummy read and the second byte is the byte was actually want.
		if (I2CRead(&obd.bbi2c, obd.oled_addr, buffer, 2) == 0) {
			break;
		}

		// Check whether the byte we read is the byte we previously wrote.
		if (i > 0 && buffer[1] != RANDOM_DATA[i - 1]) {
			break;
		}

		// Write the current byte, we will attempt to read it in the next loop iteration.
		buffer[0] = 0xc0; // one data
		buffer[1] = RANDOM_DATA[i]; // data byte
		buffer[2] = 0x80; // one command
		buffer[3] = 0xEE; // end read-modify-write
		if (I2CWrite(&obd.bbi2c, obd.oled_addr, buffer, 4) == 0) {
			break;
		}
	}

	obdPower(&obd, true);
	return i == sizeof(RANDOM_DATA);
}

void I2CDisplayAddon::clearScreen(int render) {
	obdFill(&obd, 0, render);
}

void I2CDisplayAddon::drawSplashScreen(int splashMode, uint8_t * splashChoice, int splashSpeed)
{
    int mils = getMillis();
    switch (splashMode)
	{
		case STATICSPLASH: // Default, display static or custom image
			obdDrawSprite(&obd, splashChoice, 128, 64, 16, 0, 0, 1);
			break;
		case CLOSEIN: // Close-in. Animate the GP2040 logo
			obdDrawSprite(&obd, (uint8_t *)bootLogoTop, 43, 39, 6, 43, std::min<int>((mils / splashSpeed) - 39, 0), 1);
			obdDrawSprite(&obd, (uint8_t *)bootLogoBottom, 80, 21, 10, 24, std::max<int>(64 - (mils / (splashSpeed * 2)), 44), 1);
			break;
        case CLOSEINCUSTOM: // Close-in on custom image or delayed close-in if custom image does not exist
            obdDrawSprite(&obd, splashChoice, 128, 64, 16, 0, 0, 1);
            if (mils > 2500) {
                int milss = mils - 2500;
                obdRectangle(&obd, 0, 0, 127, 1 + (milss / splashSpeed), 0, 1);
                obdRectangle(&obd, 0, 63, 127, 62 - (milss / (splashSpeed * 2)), 0, 1);
                obdDrawSprite(&obd, (uint8_t *)bootLogoTop, 43, 39, 6, 43, std::min<int>((milss / splashSpeed) - 39, 0), 1);
                obdDrawSprite(&obd, (uint8_t *)bootLogoBottom, 80, 21, 10, 24, std::max<int>(64 - (milss / (splashSpeed * 2)), 44), 1);
            }
            break;
	}
}

void I2CDisplayAddon::drawText(int x, int y, std::string text) {
	obdWriteString(&obd, 0, x, y, (char*)text.c_str(), FONT_6x8, 0, 0);
}

void I2CDisplayAddon::drawStatusBar(Gamepad * gamepad)
{
	BoardOptions boardOptions = getBoardOptions();
	AddonOptions addonOptions = Storage::getInstance().getAddonOptions();

	// Limit to 21 chars with 6x8 font for now
	statusBar.clear();

	switch (gamepad->options.inputMode)
	{
		case INPUT_MODE_HID:    statusBar += "DINPUT"; break;
		case INPUT_MODE_SWITCH: statusBar += "SWITCH"; break;
		case INPUT_MODE_XINPUT: statusBar += "XINPUT"; break;
		case INPUT_MODE_CONFIG: statusBar += "CONFIG"; break;
	}

	if ( addonOptions.pinButtonTurbo != (uint8_t)-1 ) {
		statusBar += " T";
		if ( addonOptions.turboShotCount < 10 ) // padding
			statusBar += "0";
		statusBar += std::to_string(addonOptions.turboShotCount);
	} else {
		statusBar += "    "; // no turbo, don't show Txx setting
	}
	switch (gamepad->options.dpadMode)
	{

		case DPAD_MODE_DIGITAL:      statusBar += " DP"; break;
		case DPAD_MODE_LEFT_ANALOG:  statusBar += " LS"; break;
		case DPAD_MODE_RIGHT_ANALOG: statusBar += " RS"; break;
	}

	switch (gamepad->options.socdMode)
	{
		case SOCD_MODE_NEUTRAL:               statusBar += " SOCD-N"; break;
		case SOCD_MODE_UP_PRIORITY:           statusBar += " SOCD-U"; break;
		case SOCD_MODE_SECOND_INPUT_PRIORITY: statusBar += " SOCD-L"; break;
	}
	drawText(0, 0, statusBar);
}