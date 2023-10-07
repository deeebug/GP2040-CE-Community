#include "addons/input_macro.h"
#include "storagemanager.h"
#include "GamepadState.h"

#include "hardware/gpio.h"

bool InputMacro::available() {
    inputMacroOptions = Storage::getInstance().getAddonOptions().macroOptions;
	return inputMacroOptions.enabled;
}

void InputMacro::setup() {
    inputMacroOptions = Storage::getInstance().getAddonOptions().macroOptions;

	gpio_init(inputMacroOptions.pin);             // Initialize pin
	gpio_set_dir(inputMacroOptions.pin, GPIO_IN); // Set as INPUT
	gpio_pull_up(inputMacroOptions.pin);          // Set as PULLUP

    for (int i = 0; i < inputMacroOptions.macroList_count; i++) {
        Macro& macro = inputMacroOptions.macroList[i];
        if (!macro.enabled) continue;
        if (!macro.useMacroTriggerButton && !isValidPin(macro.macroTriggerPin)) continue;

        gpio_init(macro.macroTriggerPin);             // Initialize pin
        gpio_set_dir(macro.macroTriggerPin, GPIO_IN); // Set as INPUT
        gpio_pull_up(macro.macroTriggerPin);          // Set as PULLUP
    }
}

void InputMacro::preprocess()
{
    Gamepad * gamepad = Storage::getInstance().GetGamepad();
	uint32_t allPins = ~gpio_get_all();

    if (macroPosition == -1 || inputMacroOptions.macroList[macroPosition].interruptible) {
        int newMacroPosition = -1;
        for (int i = 0; i < inputMacroOptions.macroList_count; i++) {
            Macro& macro = inputMacroOptions.macroList[i];
            if (!macro.enabled) continue;

            if (macro.useMacroTriggerButton) {
                if (macro.macroTriggerButton == 0) continue;
                if ((allPins & 1 << inputMacroOptions.pin) &&
                    ((gamepad->state.buttons & macro.macroTriggerButton) ||
                    (gamepad->state.dpad & (macro.macroTriggerButton >> 16)))) {
                    macroInputPressed = true;
                    newMacroPosition = i; break;
                }
            } else {
                if (!isValidPin(macro.macroTriggerPin)) continue;
                if ((allPins & 1 << macro.macroTriggerPin)) {
                    macroInputPressed = true;
                    newMacroPosition = i; break;
                }
            }
        } 

        if (macroPosition == -1 && newMacroPosition == -1) return;

        if (macroPosition != -1 && newMacroPosition != -1 && newMacroPosition != macroPosition) {
            reset();
            return;
        }

        if (newMacroPosition != -1 && !isMacroRunning) {
            macroPosition = newMacroPosition;
            macroStartTime = 0;
        }
    }

    Macro& macro = inputMacroOptions.macroList[macroPosition];

    if (macro.useMacroTriggerButton) {
        macroInputPressed = (allPins & 1 << inputMacroOptions.pin) &&
                            ((gamepad->state.buttons & macro.macroTriggerButton) ||
                             (gamepad->state.dpad & (macro.macroTriggerButton >> 16)));
    } else {
        macroInputPressed = (allPins & 1 << macro.macroTriggerPin);
    }

    uint32_t currentMillis = getMillis();

    if (!isMacroRunning && macroInputPressed && macroTriggerDebounceStartTime == 0) {
        macroTriggerDebounceStartTime = currentMillis;
        return;
    }

    if (macroTriggerDebounceStartTime != 0) {
        if (((currentMillis - macroTriggerDebounceStartTime) > 5)) {
            macroTriggerDebounceStartTime = 0;
        } else {
            return;
        }
    }

    if (!isMacroRunning) {
        switch (macro.macroType) {
            case ON_TOGGLE:
            case ON_PRESS:
                isMacroTriggerHeld = prevMacroInputPressed && !macroInputPressed;
                break;
            case ON_HOLD_REPEAT:
                isMacroTriggerHeld = macroInputPressed;
                break;
            default:
                break;
        }
    } else {
        switch (macro.macroType) {
            case ON_PRESS:
                break; // no-op
            case ON_TOGGLE:
                if (prevMacroInputPressed && !macroInputPressed)
                    isMacroTriggerHeld = false;
                break;
            case ON_HOLD_REPEAT:
                isMacroTriggerHeld = macroInputPressed;
                break;
        }
    }
    prevMacroInputPressed = macroInputPressed;

    MacroInput& macroInput = macro.macroInputs[macroInputPosition];
    uint32_t macroInputDuration = macroInput.duration + macroInput.waitDuration;

    if (!isMacroRunning && isMacroTriggerHeld) {
        isMacroRunning = true;
        macroStartTime = currentMillis;
        macroInputHoldTime = macroInputDuration <= 0 ? INPUT_HOLD_MS : macroInputDuration;
    }
    
    if (!isMacroRunning) return;
    
    if ((!isMacroTriggerHeld && macro.interruptible)) {
        reset();
        return;
    }

    if (macro.exclusive) {
        gamepad->state.dpad = 0;
        gamepad->state.buttons = 0;
    } else {
        if (macro.useMacroTriggerButton) {
            gamepad->state.dpad &= ~(macro.macroTriggerButton >> 16);
            gamepad->state.buttons &= ~macro.macroTriggerButton;
        }
        if (macro.interruptible && (gamepad->state.buttons != 0 || gamepad->state.dpad != 0)) {
            reset();
            return;
        }
    }

    if ((currentMillis - macroStartTime) <= macroInput.duration) {
        uint32_t buttonMask = macroInput.buttonMask;
        if (buttonMask & GAMEPAD_MASK_DU) {
            gamepad->state.dpad |= GAMEPAD_MASK_UP;
        }
        if (buttonMask & GAMEPAD_MASK_DD) {
            gamepad->state.dpad |= GAMEPAD_MASK_DOWN;
        }
        if (buttonMask & GAMEPAD_MASK_DL) {
            gamepad->state.dpad |= GAMEPAD_MASK_LEFT;
        }
        if (buttonMask & GAMEPAD_MASK_DR) {
            gamepad->state.dpad |= GAMEPAD_MASK_RIGHT;
        }
        gamepad->state.buttons |= buttonMask;
    }

    if ((currentMillis - macroStartTime) >= macroInputHoldTime) {
        macroStartTime = currentMillis; macroInputPosition++;
        macroInputHoldTime = macroInputDuration <= 0 ? INPUT_HOLD_MS : macroInputDuration;
    }
    
    if (isMacroRunning && macroInputPosition >= (macro.macroInputs_count)) {
        macroInputPosition = 0;
        bool isMacroTypeLoopable = macro.macroType == ON_TOGGLE || macro.macroType == ON_HOLD_REPEAT;
        isMacroTriggerHeld = isMacroTriggerHeld && isMacroTypeLoopable;
        isMacroRunning = isMacroTriggerHeld;
        macroPosition = (isMacroTypeLoopable && isMacroTriggerHeld) ? macroPosition : -1;
        if (isMacroTypeLoopable && !isMacroTriggerHeld) {
            macroStartTime = 0;
            macroInputHoldTime = INPUT_HOLD_MS;
        }
    }
}

void InputMacro::reset() {
    macroPosition = -1;
    isMacroRunning = false;
    macroStartTime = 0;
    prevMacroInputPressed = false;
}