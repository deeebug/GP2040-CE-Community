#include "addons/input_macro.h"
#include "storagemanager.h"

#include "GamepadState.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/structs/ioqspi.h"
#include "hardware/structs/sio.h"

#define INPUT_HOLD_MS 100

bool __no_inline_not_in_flash_func(get_bootsel_button)() {
    const uint CS_PIN_INDEX = 1;

    // Must disable interrupts, as interrupt handlers may be in flash, and we
    // are about to temporarily disable flash access!
    uint32_t flags = save_and_disable_interrupts();

    // Set chip select to Hi-Z
    hw_write_masked(&ioqspi_hw->io[CS_PIN_INDEX].ctrl,
                    GPIO_OVERRIDE_LOW << IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB,
                    IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS);

    // Note we can't call into any sleep functions in flash right now
    for (volatile int i = 0; i < 1000; ++i);

    // The HI GPIO registers in SIO can observe and control the 6 QSPI pins.
    // Note the button pulls the pin *low* when pressed.
    bool button_state = !(sio_hw->gpio_hi_in & (1u << CS_PIN_INDEX));

    // Need to restore the state of chip select, else we are going to have a
    // bad time when we return to code in flash!
    hw_write_masked(&ioqspi_hw->io[CS_PIN_INDEX].ctrl,
                    GPIO_OVERRIDE_NORMAL << IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB,
                    IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS);

    restore_interrupts(flags);

    return button_state;
}

bool InputMacro::available() {
	return true;
    Gamepad * gamepad = Storage::getInstance().GetGamepad();
    return gamepad->pressedB4();
}

struct Input {
    GamepadState state;
    int duration = -1;
};


Input inputs[] = {
    {
        { .dpad = GAMEPAD_MASK_DOWN }
    },
    {
        { .dpad = GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT }
    },
    {
        { .dpad = GAMEPAD_MASK_RIGHT,
          .buttons = GAMEPAD_MASK_B4  }
    }
};

int position = 0;
bool isProcessing = 0;
int bootselRaised = -1;
int heldAt = 0;
int shouldHold = INPUT_HOLD_MS;

void InputMacro::setup() {
    position = 0;
    isProcessing = 0;
    bootselRaised = -1;
}

void InputMacro::process()
{
    if (getMillis() < 1000) return;

    if (bootselRaised == -1) {
        if (get_bootsel_button()) {
            bootselRaised = 0;
        }
    } else {
        if (!get_bootsel_button()) {
            if (bootselRaised == 0) {
                bootselRaised = 1;
            }
        }
    }

    if (!isProcessing && bootselRaised > 0) {
        isProcessing = 1;
        heldAt = getMillis();
        shouldHold = inputs[position].duration == -1 ? INPUT_HOLD_MS : inputs[position].duration;
    }
    
    if (!isProcessing) return;
    
    Gamepad * gamepad = Storage::getInstance().GetGamepad();
    Input currentInput = inputs[position];

    gamepad->state = currentInput.state;
    
    if ((getMillis() - heldAt) >= shouldHold) {
        heldAt = getMillis(); position++;
        shouldHold = inputs[position].duration == -1 ? INPUT_HOLD_MS : inputs[position].duration;
    }
    
    if (isProcessing && position >= (sizeof(inputs) / sizeof(Input))) {
        position = 0;
        isProcessing = 0;
        bootselRaised = -1;
    }
}