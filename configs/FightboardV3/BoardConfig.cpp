#include "BoardConfig.h"
#include "display_button_layouts.h"
#include "led_button_layouts.h"
#include "helper.h"

void ButtonLayoutFightboard::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

	obdPreciseEllipse(&obd, (startX + buttonMargin * 3.625), startY * 1.25, buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
	obdPreciseEllipse(&obd, (startX + buttonMargin * 4.625), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
	obdPreciseEllipse(&obd, (startX + buttonMargin * 5.625), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
	obdPreciseEllipse(&obd, (startX + buttonMargin * 6.625), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

	obdPreciseEllipse(&obd, (startX + buttonMargin * 3.625), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
	obdPreciseEllipse(&obd, (startX + buttonMargin * 4.625), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
	obdPreciseEllipse(&obd, (startX + buttonMargin * 5.625), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
	obdPreciseEllipse(&obd, (startX + buttonMargin * 6.625), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL2());

    // Extra buttons
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.5, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedL3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.0625, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedS1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.625, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedA1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.125+0.0625, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedS2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.75, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedR3());
}

void initBoardConfig() {
    registerDisplayButtonLayout(new ButtonLayoutFightboardStick());
    registerDisplayButtonLayoutRight(new ButtonLayoutFightboard());
    registerLEDButtonLayout(new LEDButtonLayoutFightboard());
}