#include "BoardConfig.h"
#include "display_button_layouts.h"
#include "led_button_layouts.h"
#include "helper.h"

void ButtonLayoutFightboardMirrored::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);
    const int leftMargin = startX + buttonPadding + buttonRadius;

	obdPreciseEllipse(&obd, leftMargin, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL1());
	obdPreciseEllipse(&obd, leftMargin + buttonMargin, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
	obdPreciseEllipse(&obd, leftMargin + (buttonMargin*2), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
	obdPreciseEllipse(&obd, leftMargin + (buttonMargin*3), startY * 1.25, buttonRadius, buttonRadius, 1, pGamepad->pressedB3());

	obdPreciseEllipse(&obd, leftMargin, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL2());
	obdPreciseEllipse(&obd, leftMargin + buttonMargin, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
	obdPreciseEllipse(&obd, leftMargin + (buttonMargin*2), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
	obdPreciseEllipse(&obd, leftMargin + (buttonMargin*3), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, pGamepad->pressedB1());

    // Extra buttons
    obdPreciseEllipse(&obd, startX + buttonMargin * 0.5, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedL3());
    obdPreciseEllipse(&obd, startX + buttonMargin * 1.0625, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedS1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 1.625, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedA1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 2.125+0.0625, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedS2());
    obdPreciseEllipse(&obd, startX + buttonMargin * 2.75, startY + (buttonMargin * 1.5), 3, 3, 1, pGamepad->pressedR3());
}

std::vector<std::vector<LabelMaskPair>> LEDButtonLayoutWasdFBM::generatedLEDButtons() {
	std::vector<std::vector<LabelMaskPair>> pixels =
	{
		{
			{ BUTTON_LABEL_L1, GAMEPAD_MASK_L1 },
			{ BUTTON_LABEL_L2, GAMEPAD_MASK_L2 },
		},
		{
			{ BUTTON_LABEL_R1, GAMEPAD_MASK_R1 },
			{ BUTTON_LABEL_R2, GAMEPAD_MASK_R2 },
		},
		{
			{ BUTTON_LABEL_B4, GAMEPAD_MASK_B4 },
			{ BUTTON_LABEL_B2, GAMEPAD_MASK_B2 },
		},
		{
			{ BUTTON_LABEL_B3, GAMEPAD_MASK_B3 },
			{ BUTTON_LABEL_B1, GAMEPAD_MASK_B1 },
		},
		{
			PIXEL_OFF,
			{ BUTTON_LABEL_LEFT, GAMEPAD_MASK_DL },
		},
		{
			{ BUTTON_LABEL_UP, GAMEPAD_MASK_DU },
			{ BUTTON_LABEL_DOWN, GAMEPAD_MASK_DD },
		},
		{
			PIXEL_OFF,
			{ BUTTON_LABEL_RIGHT, GAMEPAD_MASK_DR },
		},
		{
			{ BUTTON_LABEL_S1, GAMEPAD_MASK_S1 },
			{ BUTTON_LABEL_S2, GAMEPAD_MASK_S2 },
			{ BUTTON_LABEL_L3, GAMEPAD_MASK_L3 },
			{ BUTTON_LABEL_R3, GAMEPAD_MASK_R3 },
			{ BUTTON_LABEL_A1, GAMEPAD_MASK_A1 },
			{ BUTTON_LABEL_A2, GAMEPAD_MASK_A2 },
		}
	};

	return pixels;
}

void initBoardConfig() {
    registerDisplayButtonLayout(new ButtonLayoutFightboardStickMirrored());
    registerDisplayButtonLayoutRight(new ButtonLayoutFightboardMirrored());
    registerLEDButtonLayout(new LEDButtonLayoutWasdFBM());
}