#include "addons/boardled.h"
#include "storagemanager.h"

bool BoardLedAddon::available() {
        // light_up(true);
    return true;
    auto options = Storage::getInstance().getBoardOptions();
	return options.boardLedMode != BoardLedMode::BOARD_LED_OFF;
}

void BoardLedAddon::setup() {
    gpio_init(BOARD_LED_PIN);
    auto options = Storage::getInstance().getBoardOptions();
    mode = INPUT_TEST;
}

void BoardLedAddon::process() {
    bool state = 0;
    switch (mode) {
        case INPUT_TEST: {
            Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();
            state = (gamepad->state.buttons != 0) || (gamepad->state.dpad != 0);
        }
        break;
        case MODE_INDICATOR:
            break;
        case BOARD_LED_OFF:
            return;
    }

    if (prevState != state) {
        board_led_on(state);
    }

    prevState = state;
}

void BoardLedAddon::board_led_on(bool state) {
    gpio_set_dir(BOARD_LED_PIN, GPIO_OUT);
    gpio_put(BOARD_LED_PIN, state ? 1 : 0);
}
