#include "led_button_layouts.h"
#include "GamepadState.h"
#include "helper.h"
#include "hash.h"

std::vector<std::vector<LabelMaskPair>> LEDButtonLayoutButtons::generatedLEDButtons() {
	std::vector<std::vector<LabelMaskPair>> pixels =
	{
		{
			{ BUTTON_LABEL_B3, GAMEPAD_MASK_B3 },
			{ BUTTON_LABEL_B1, GAMEPAD_MASK_B1 },
		},
		{
			{ BUTTON_LABEL_B4, GAMEPAD_MASK_B4 },
			{ BUTTON_LABEL_B2, GAMEPAD_MASK_B2 },
		},
		{
			{ BUTTON_LABEL_R1, GAMEPAD_MASK_R1 },
			{ BUTTON_LABEL_R2, GAMEPAD_MASK_R2 },
		},
		{
			{ BUTTON_LABEL_L1, GAMEPAD_MASK_L1 },
			{ BUTTON_LABEL_L2, GAMEPAD_MASK_L2 },
		},
		{
			{ BUTTON_LABEL_LEFT, GAMEPAD_MASK_DL },
			{ BUTTON_LABEL_DOWN, GAMEPAD_MASK_DD },
			{ BUTTON_LABEL_RIGHT, GAMEPAD_MASK_DR },
			{ BUTTON_LABEL_UP, GAMEPAD_MASK_DU },
			{ BUTTON_LABEL_S1, GAMEPAD_MASK_S1 },
			{ BUTTON_LABEL_S2, GAMEPAD_MASK_S2 },
			{ BUTTON_LABEL_L3, GAMEPAD_MASK_L3 },
			{ BUTTON_LABEL_R3, GAMEPAD_MASK_R3 },
			{ BUTTON_LABEL_A1, GAMEPAD_MASK_A1 },
			{ BUTTON_LABEL_A2, GAMEPAD_MASK_A2 },
		},
	};

	return pixels;
}

std::vector<std::vector<LabelMaskPair>> LEDButtonLayoutStickless::generatedLEDButtons() {
	std::vector<std::vector<LabelMaskPair>> pixels =
	{
		{
			{ BUTTON_LABEL_LEFT, GAMEPAD_MASK_DL },
			PIXEL_OFF,
			PIXEL_OFF,
		},
		{
			{ BUTTON_LABEL_DOWN, GAMEPAD_MASK_DD },
			PIXEL_OFF,
			PIXEL_OFF,
		},
		{
			{ BUTTON_LABEL_RIGHT, GAMEPAD_MASK_DR },
			PIXEL_OFF,
			PIXEL_OFF,
		},
		{
			{ BUTTON_LABEL_UP, GAMEPAD_MASK_DU },
			PIXEL_OFF,
			PIXEL_OFF,
		},
		{
			{ BUTTON_LABEL_B3, GAMEPAD_MASK_B3 },
			{ BUTTON_LABEL_B1, GAMEPAD_MASK_B1 },
			PIXEL_OFF,
		},
		{
			{ BUTTON_LABEL_B4, GAMEPAD_MASK_B4 },
			{ BUTTON_LABEL_B2, GAMEPAD_MASK_B2 },
			PIXEL_OFF,
		},
		{
			{ BUTTON_LABEL_R1, GAMEPAD_MASK_R1 },
			{ BUTTON_LABEL_R2, GAMEPAD_MASK_R2 },
			PIXEL_OFF,
		},
		{
			{ BUTTON_LABEL_L1, GAMEPAD_MASK_L1 },
			{ BUTTON_LABEL_L2, GAMEPAD_MASK_L2 },
			PIXEL_OFF,
		},
		{
			{ BUTTON_LABEL_S1, GAMEPAD_MASK_S1 },
			{ BUTTON_LABEL_S2, GAMEPAD_MASK_S2 },
			{ BUTTON_LABEL_L3, GAMEPAD_MASK_L3 },
			{ BUTTON_LABEL_R3, GAMEPAD_MASK_R3 },
			{ BUTTON_LABEL_A1, GAMEPAD_MASK_A1 },
			{ BUTTON_LABEL_A2, GAMEPAD_MASK_A2 },
		},
	};

	return pixels;
}

std::vector<std::vector<LabelMaskPair>> LEDButtonLayoutWasd::generatedLEDButtons() {
	std::vector<std::vector<LabelMaskPair>> pixels =
	{
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
			{ BUTTON_LABEL_B3, GAMEPAD_MASK_B3 },
			{ BUTTON_LABEL_B1, GAMEPAD_MASK_B1 },
		},
		{
			{ BUTTON_LABEL_B4, GAMEPAD_MASK_B4 },
			{ BUTTON_LABEL_B2, GAMEPAD_MASK_B2 },
		},
		{
			{ BUTTON_LABEL_R1, GAMEPAD_MASK_R1 },
			{ BUTTON_LABEL_R2, GAMEPAD_MASK_R2 },
		},
		{
			{ BUTTON_LABEL_L1, GAMEPAD_MASK_L1 },
			{ BUTTON_LABEL_L2, GAMEPAD_MASK_L2 },
		},
		{
			{ BUTTON_LABEL_S1, GAMEPAD_MASK_S1 },
			{ BUTTON_LABEL_S2, GAMEPAD_MASK_S2 },
			{ BUTTON_LABEL_L3, GAMEPAD_MASK_L3 },
			{ BUTTON_LABEL_R3, GAMEPAD_MASK_R3 },
			{ BUTTON_LABEL_A1, GAMEPAD_MASK_A1 },
			{ BUTTON_LABEL_A2, GAMEPAD_MASK_A2 },
		},
	};

	return pixels;
}

std::map<size_t, LEDButtonLayout*> LED_BUTTON_LAYOUTS {
    {  hash(LEDButtonLayoutButtons::NAME), new LEDButtonLayoutButtons() },
    {  hash(LEDButtonLayoutStickless::NAME), new LEDButtonLayoutStickless() },
    {  hash(LEDButtonLayoutWasd::NAME), new LEDButtonLayoutWasd() },
};

std::map<size_t, LEDButtonLayout*> getLEDButtonLayouts() {
    return LED_BUTTON_LAYOUTS;
}

LEDButtonLayout* getLEDButtonLayout(size_t layoutId) {
    return LED_BUTTON_LAYOUTS[layoutId];
}

size_t getLEDButtonLayoutId(std::string name) {
    return hash(name);
}