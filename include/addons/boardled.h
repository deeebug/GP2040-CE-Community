#ifndef _BoardLed_H
#define _BoardLed_H

#include "gpaddon.h"
#include "enums.h"
#include "pico/stdlib.h"

// BoardLed Module Name
#define BoardLedName "BoardLed"

#define BOARD_LED_PIN 25
#define GAMEPAD_MASK_BUTTONS (0 | GAMEPAD_MASK_B1 | GAMEPAD_MASK_B2 | GAMEPAD_MASK_B3 | \
							  GAMEPAD_MASK_B4 | GAMEPAD_MASK_L1 | GAMEPAD_MASK_R1 | \
							  GAMEPAD_MASK_L2 | GAMEPAD_MASK_R2 | GAMEPAD_MASK_S1 | \
							  GAMEPAD_MASK_S2 | GAMEPAD_MASK_L3 | GAMEPAD_MASK_R3 | \
							  GAMEPAD_MASK_A1 | GAMEPAD_MASK_A2)

class BoardLedAddon : public GPAddon {
public:
	virtual bool available();   // GPAddon available
	virtual void setup();       // BoardLed Setup
	virtual void process();     // BoardLed Process
    virtual std::string name() { return BoardLedName; }
private:
	virtual void board_led_on(bool state);
	BoardLedMode mode = INPUT_TEST;
	bool prevState = true;
};

#endif  // _BoardLed_H_