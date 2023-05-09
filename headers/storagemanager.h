 /*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>
#include "NeoPico.hpp"
#include "FlashPROM.h"

#include "enums.h"
#include "helper.h"
#include "gamepad.h"

#include "config_legacy.h"
#include "config.pb.h"

#define SI Storage::getInstance()

// Storage manager for board, LED options, and thread-safe settings
class Storage {
public:
	Storage(Storage const&) = delete;
	void operator=(Storage const&)  = delete;
	static Storage& getInstance() // Thread-safe storage ensures cross-thread talk
	{
		static Storage instance;
		return instance;
	}

	Config& getConfig() { return config; }
	const AddonOptions& getAddonOptions() const { return config.addonOptions; }
	const PinMappings& getPinMappings() const { return config.pinMappings; }
	const KeyboardMapping& getKeyboardMapping() const { return config.keyboardMapping; }
	const HotkeyOptions& getHotkeyOptions() const { return config.hotkeyOptions; }

	bool save();

	void setBoardOptions(ConfigLegacy::BoardOptions);	// Board Options
	const ConfigLegacy::BoardOptions& getBoardOptions() { return boardOptions; }

	void setPreviewBoardOptions(const ConfigLegacy::BoardOptions&);	// Preview Board Options
	const ConfigLegacy::BoardOptions& getPreviewBoardOptions() { return previewBoardOptions; }

	void setLegacyAddonOptions(ConfigLegacy::AddonOptions); // Add-On Options
	const ConfigLegacy::AddonOptions& getLegacyAddonOptions() { return addonOptions; }

	void setSplashImage(const ConfigLegacy::SplashImage&);
	const ConfigLegacy::SplashImage& getSplashImage() { return splashImage; }

	void setLEDOptions(ConfigLegacy::LEDOptions);		// LED Options
	void setDefaultLEDOptions();
	const ConfigLegacy::LEDOptions& getLEDOptions() { return ledOptions; }

	void savePS4Options();     // PS4 Options
	void setDefaultPS4Options();
	ConfigLegacy::PS4Options * getPS4Options();

	void SetConfigMode(bool); 			// Config Mode (on-boot)
	bool GetConfigMode();

	void SetGamepad(Gamepad *); 		// MPGS Gamepad Get/Set
	Gamepad * GetGamepad();

	void SetProcessedGamepad(Gamepad *); // MPGS Processed Gamepad Get/Set
	Gamepad * GetProcessedGamepad();

	void SetFeatureData(uint8_t *); 	// USB Feature Data Get/Set
	void ClearFeatureData();
	uint8_t * GetFeatureData();

	void ResetSettings(); 				// EEPROM Reset Feature

private:
	Storage();
	void initBoardOptions();
	void initPreviewBoardOptions();
	void initAddonOptions();
	void initLEDOptions();
	void initSplashImage();
	void setDefaultBoardOptions();
	void setDefaultAddonOptions();
	void setDefaultSplashImage();
	void initPS4Options();
	bool CONFIG_MODE = false; 			// Config mode (boot)
	Gamepad * gamepad = nullptr;    		// Gamepad data
	Gamepad * processedGamepad = nullptr; // Gamepad with ONLY processed data
	ConfigLegacy::BoardOptions boardOptions;
	ConfigLegacy::BoardOptions previewBoardOptions;
	ConfigLegacy::AddonOptions addonOptions;
	ConfigLegacy::LEDOptions ledOptions;
	ConfigLegacy::PS4Options ps4Options;
	uint8_t featureData[32]; // USB X-Input Feature Data
	ConfigLegacy::SplashImage splashImage;

	Config config;
};

#endif
