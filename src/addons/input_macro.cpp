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

enum MacroType {
    ON_RELEASE,
    ON_HOLD,
    ON_HOLD_REPEAT,
    ON_RELEASE_TOGGLE
};

struct Input {
    GamepadState state;
    int duration = -1;
};

struct Macro {
    Input *inputs;
    wchar_t *name;
    MacroType type;
    int size;
};

Input _inputsHadouken[] = {
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

Macro hadouken = {
    .inputs = _inputsHadouken,
    .name = L"Hadouken",
    .type = ON_HOLD_REPEAT,
    .size = 3
};

Input _inputsShoryuken[] = {
    {
        { .dpad = GAMEPAD_MASK_RIGHT }
    },
    {
        { .dpad = GAMEPAD_MASK_DOWN }
    },
    {
        { .dpad = GAMEPAD_MASK_DOWN | GAMEPAD_MASK_RIGHT,
          .buttons = GAMEPAD_MASK_B4  }
    }
};

Macro shoryuken = {
    .inputs = _inputsShoryuken,
    .name = L"Shoryuken",
    .type = ON_RELEASE,
    .size = 3
};

Input _inputsTatsu[] = {
    {
        { .dpad = GAMEPAD_MASK_DOWN }
    },
    {
        { .dpad = GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT }
    },
    {
        { .dpad = GAMEPAD_MASK_LEFT,
          .buttons = GAMEPAD_MASK_B2  }
    }
};

Macro tatsu = {
    .inputs = _inputsTatsu,
    .name = L"Tatsumaki Senpuukyaku",
    .type = ON_RELEASE,
    .size = 3
};

Input _inputsFf7[] = {
    {{ .dpad = 0 }, .duration = 200},  {{ .dpad = GAMEPAD_MASK_RIGHT }, .duration = 500},
    {{ .dpad = 0 }, .duration = 200},  {{ .dpad = GAMEPAD_MASK_LEFT }, .duration = 500},
    {{ .dpad = 0 }, .duration = 500},  {{ .dpad = 0, .buttons = GAMEPAD_MASK_B2 }, .duration = 500},
    {{ .dpad = 0 }, .duration = 500},  {{ .dpad = 0, .buttons = GAMEPAD_MASK_B2 }, .duration = 500},
    {{ .dpad = 0 }, .duration = 500},  {{ .dpad = 0, .buttons = GAMEPAD_MASK_B2 }, .duration = 500},
    {{ .dpad = 0 }, .duration = 500},  {{ .dpad = 0, .buttons = GAMEPAD_MASK_B2 }, .duration = 500}
};

Macro ff7 = {
    .inputs = _inputsFf7,
    .name = L"Ff7",
    .type = ON_RELEASE_TOGGLE,
    .size = 12
};

Macro macroList[4] = { ff7, hadouken, shoryuken, tatsu };

int macroPosition = 0;
int position = 0;
bool isProcessing = 0;
int bootselPressed = 0;
int64_t heldAt = 0;
int shouldHold = INPUT_HOLD_MS;
bool trigger = false;
int prevBootselPressed = 0;

void InputMacro::setup() {
    position = 0;
    isProcessing = 0;
    bootselPressed = 0;
}

void InputMacro::process()
{
    if (getMillis() < 1000) return;
    macroPosition = 0;
    auto macro = macroList[macroPosition];
    
    bootselPressed = get_bootsel_button();
    if (!isProcessing) {
        switch (macro.type) {
            case ON_RELEASE_TOGGLE:
            case ON_RELEASE:
                trigger = prevBootselPressed && !bootselPressed;
                break;
            case ON_HOLD:
                trigger = !prevBootselPressed && bootselPressed;
                break;
            case ON_HOLD_REPEAT:
                trigger = bootselPressed;
                break;
            default:
                break;
        }
    } else {
        switch (macro.type) {
            case ON_RELEASE_TOGGLE:
                if (prevBootselPressed && !bootselPressed)
                    trigger = false;
                break;
        }
    }

    prevBootselPressed = bootselPressed;

    auto inputs = macro.inputs;

    if (!isProcessing && trigger) {
        isProcessing = 1;
        heldAt = getMillis();
        shouldHold = inputs[position].duration == -1 ? INPUT_HOLD_MS : inputs[position].duration;
    }
    
    if (!isProcessing) return;
    
    Gamepad * gamepad = Storage::getInstance().GetGamepad();
    gamepad->state = inputs[position].state;
    
    if ((getMillis() - heldAt) >= shouldHold) {
        heldAt = getMillis(); position++;
        shouldHold = inputs[position].duration == -1 ? INPUT_HOLD_MS : inputs[position].duration;
    }
    
    if (isProcessing && position >= ((sizeof(Input) * macro.size) / sizeof(Input))) {
        position = 0;
        isProcessing = (macro.type == ON_RELEASE_TOGGLE && trigger) ? 1 : 0;
        macroPosition = (macro.type == ON_RELEASE_TOGGLE && trigger) ? macroPosition : (++macroPosition) % (sizeof(macroList) / sizeof(Macro));
        trigger = isProcessing;
    }
}