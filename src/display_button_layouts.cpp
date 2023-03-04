#include <map>
#include "OneBitDisplay.h"
#include "display_button_layouts.h"
#include "hash.h"

bool DisplayButtonLayout::pressedUp(Gamepad* gamepad, Gamepad* pGamepad)
{
    switch (gamepad->options.dpadMode)
    {
        case DPAD_MODE_DIGITAL:      return pGamepad->pressedUp();
        case DPAD_MODE_LEFT_ANALOG:  return pGamepad->state.ly == GAMEPAD_JOYSTICK_MIN;
        case DPAD_MODE_RIGHT_ANALOG: return pGamepad->state.ry == GAMEPAD_JOYSTICK_MIN;
    }
    return false;
}

bool DisplayButtonLayout::pressedDown(Gamepad* gamepad, Gamepad* pGamepad)
{
    switch (gamepad->options.dpadMode)
    {
        case DPAD_MODE_DIGITAL:      return pGamepad->pressedDown();
        case DPAD_MODE_LEFT_ANALOG:  return pGamepad->state.ly == GAMEPAD_JOYSTICK_MAX;
        case DPAD_MODE_RIGHT_ANALOG: return pGamepad->state.ry == GAMEPAD_JOYSTICK_MAX;
    }
    return false;
}

bool DisplayButtonLayout::pressedLeft(Gamepad* gamepad, Gamepad* pGamepad)
{
    switch (gamepad->options.dpadMode)
    {
        case DPAD_MODE_DIGITAL:      return pGamepad->pressedLeft();
        case DPAD_MODE_LEFT_ANALOG:  return pGamepad->state.lx == GAMEPAD_JOYSTICK_MIN;
        case DPAD_MODE_RIGHT_ANALOG: return pGamepad->state.rx == GAMEPAD_JOYSTICK_MIN;
    }
    return false;
}

bool DisplayButtonLayout::pressedRight(Gamepad* gamepad, Gamepad* pGamepad)
{
    switch (gamepad->options.dpadMode)
    {
        case DPAD_MODE_DIGITAL:      return pGamepad->pressedRight();
        case DPAD_MODE_LEFT_ANALOG:  return pGamepad->state.lx == GAMEPAD_JOYSTICK_MAX;
        case DPAD_MODE_RIGHT_ANALOG: return pGamepad->state.rx == GAMEPAD_JOYSTICK_MAX;
    }
    return false;
}

void ButtonLayoutStick::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);
    // Stick
    obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY + (buttonMargin / 2), buttonRadius * 1.25, buttonRadius * 1.25, 1, 0);
    
    if (pressedUp(gamepad, pGamepad)) {
        if (pressedLeft(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
        } else if (pressedRight(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
        } else {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY, buttonRadius, buttonRadius, 1, 1);
        }
    } else if (pressedDown(gamepad, pGamepad)) {
        if (pressedLeft(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
        } else if (pressedRight(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
        } else {
            obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin, buttonRadius, buttonRadius, 1, 1);
        }
    } else if (pressedLeft(gamepad, pGamepad)) {
        obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    } else if (pressedRight(gamepad, pGamepad)) {
        obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    } else {
        obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    }
}

void ButtonLayoutStickless::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    obdPreciseEllipse(&obd, startX, startY, buttonRadius, buttonRadius, 1, pressedLeft(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + buttonMargin, startY, buttonRadius, buttonRadius, 1, pressedDown(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + (buttonMargin * 1.875), startY + (buttonMargin / 2), buttonRadius, buttonRadius, 1, pressedRight(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.25), startY + buttonMargin * 1.875, buttonRadius, buttonRadius, 1, pressedUp(gamepad, pGamepad));
}

void ButtonLayoutButtonsAngled::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // WASD
    obdPreciseEllipse(&obd, startX, startY + buttonMargin * 0.5, buttonRadius, buttonRadius, 1, pressedLeft(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin * 0.875, buttonRadius, buttonRadius, 1, pressedDown(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + buttonMargin * 1.5, startY - buttonMargin * 0.125, buttonRadius, buttonRadius, 1, pressedUp(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + (buttonMargin * 2), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, pressedRight(gamepad, pGamepad));
}

void ButtonLayoutButtonsBasic::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // UDLR
    obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, pressedLeft(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pressedUp(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, pressedDown(gamepad, pGamepad));
    obdPreciseEllipse(&obd, startX + (buttonMargin * 1.625), startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, pressedRight(gamepad, pGamepad));
}

void ButtonLayoutKeyboardAngled::drawDiamond(OBDISP& obd, int cx, int cy, int size, uint8_t colour, uint8_t filled) {
    if (filled) {
        int i;
        for (i = 0; i < size; i++) {
        obdDrawLine(&obd, cx - i, cy - size + i, cx + i, cy - size + i, colour, 0);
        obdDrawLine(&obd, cx - i, cy + size - i, cx + i, cy + size - i, colour, 0);
        }
        obdDrawLine(&obd, cx - size, cy, cx + size, cy, colour, 0); // Fill in the middle
    }
    obdDrawLine(&obd, cx - size, cy, cx, cy - size, colour, 0);
    obdDrawLine(&obd, cx, cy - size, cx + size, cy, colour, 0);
    obdDrawLine(&obd, cx + size, cy, cx, cy + size, colour, 0);
    obdDrawLine(&obd, cx, cy + size, cx - size, cy, colour, 0);
}

void ButtonLayoutKeyboardAngled::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // MixBox
    drawDiamond(obd, startX, startY, buttonRadius, 1, pressedLeft(gamepad, pGamepad));
    drawDiamond(obd, startX + buttonMargin / 2, startY + buttonMargin / 2, buttonRadius, 1, pressedDown(gamepad, pGamepad));
    drawDiamond(obd, startX + buttonMargin, startY, buttonRadius, 1, pressedUp(gamepad, pGamepad));
    drawDiamond(obd, startX + buttonMargin, startY + buttonMargin, buttonRadius, 1, pressedRight(gamepad, pGamepad));
}

void ButtonLayoutKeyboardA::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + buttonRadius;
    // MAME
    obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonRadius, startY + buttonRadius + buttonMargin, 1, pressedLeft(gamepad, pGamepad));
    obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin, startX + buttonRadius + buttonMargin, startY + buttonRadius + buttonMargin, 1, pressedDown(gamepad, pGamepad));
    obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonRadius + buttonMargin, startY + buttonRadius, 1, pressedUp(gamepad, pGamepad));
    obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius + buttonMargin, 1, pressedRight(gamepad, pGamepad));
}

void ButtonLayoutDancepadA::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + buttonRadius;

    obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonRadius, startY + buttonRadius + buttonMargin, 1, pressedLeft(gamepad, pGamepad));
    obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin * 2, startX + buttonRadius + buttonMargin, startY + buttonRadius + buttonMargin * 2, 1, pressedDown(gamepad, pGamepad));
    obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonRadius + buttonMargin, startY + buttonRadius, 1, pressedUp(gamepad, pGamepad));
    obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius + buttonMargin, 1, pressedRight(gamepad, pGamepad));
}

void ButtonLayoutTwinstickA::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // Stick
    obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY + (buttonMargin / 2), buttonRadius * 1.25, buttonRadius * 1.25, 1, 0);
    
    if (pressedUp(gamepad, pGamepad)) {
        if (pressedLeft(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
        } else if (pressedRight(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
        } else {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY, buttonRadius, buttonRadius, 1, 1);
        }
    } else if (pressedDown(gamepad, pGamepad)) {
        if (pressedLeft(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
        } else if (pressedRight(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
        } else {
            obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin, buttonRadius, buttonRadius, 1, 1);
        }
    } else if (pressedLeft(gamepad, pGamepad)) {
        obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    } else if (pressedRight(gamepad, pGamepad)) {
        obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    } else {
        obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    }
}

void ButtonLayoutBlankA::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
}

void ButtonLayoutVLXA::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // Stick
    obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY + (buttonMargin / 2), buttonRadius * 1.25, buttonRadius * 1.25, 1, 0);
    
    if (pressedUp(gamepad, pGamepad)) {
        if (pressedLeft(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
        } else if (pressedRight(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
        } else {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY, buttonRadius, buttonRadius, 1, 1);
        }
    } else if (pressedDown(gamepad, pGamepad)) {
        if (pressedLeft(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
        } else if (pressedRight(gamepad, pGamepad)) {
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
        } else {
            obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin, buttonRadius, buttonRadius, 1, 1);
        }
    } else if (pressedLeft(gamepad, pGamepad)) {
        obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    } else if (pressedRight(gamepad, pGamepad)) {
        obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    } else {
        obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    }
}

/**
 * RIGHT layouts
*/

void ButtonLayoutArcade::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 8-button
    obdPreciseEllipse(&obd, startX + buttonMargin * 3.125, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + buttonMargin * 4.125, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + buttonMargin * 5.125, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 6.125, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + buttonMargin * 2.875, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 3.875, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + buttonMargin * 4.875, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    obdPreciseEllipse(&obd, startX + buttonMargin * 5.875, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedL2());
}

void ButtonLayoutSticklessb::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 8-button
    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY, buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedL2());

}

void ButtonLayoutButtonsAngledB::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 8-button
    obdPreciseEllipse(&obd, startX + buttonMargin * 3.625, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + buttonMargin * 4.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + buttonMargin * 5.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 6.625, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    obdPreciseEllipse(&obd, startX + buttonMargin * 6.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedL2());

}

void ButtonLayoutVewlix::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 8-button Vewlix
    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + (buttonMargin * 0.2), buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75) - (buttonMargin / 3), startY + buttonMargin + (buttonMargin * 0.2), buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL2());
}

void ButtonLayoutVewlix7::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 8-button Vewlix
    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + (buttonMargin * 0.2), buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75) - (buttonMargin / 3), startY + buttonMargin + (buttonMargin * 0.2), buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    //obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pGamepad->pressedL2());

}

void ButtonLayoutCapcom::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 8-button Capcom
    obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 6.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    obdPreciseEllipse(&obd, startX + buttonMargin * 6.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedL2());
}

void ButtonLayoutCapcom6::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 6-button Capcom
    obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedR1());

    obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
}

void ButtonLayoutSega2p::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 8-button Sega2P
    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + (buttonMargin / 3), buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY, buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + buttonMargin + (buttonMargin / 3), buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedL2());
}

void ButtonLayoutNoir8::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 8-button Noir8
    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + (buttonMargin / 3.5), buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY, buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + buttonMargin + (buttonMargin / 3.5), buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedL2());
}

void ButtonLayoutKeyboardB::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + buttonRadius;

    // 6-button MAME Style
    obdRectangle(&obd, startX, startY, startX + buttonRadius, startY + buttonRadius, 1, pGamepad->pressedB3());
    obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonRadius + buttonMargin, startY + buttonRadius, 1, pGamepad->pressedB4());
    obdRectangle(&obd, startX + buttonMargin * 2, startY, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius, 1, pGamepad->pressedR1());

    obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonRadius, startY + buttonMargin + buttonRadius, 1, pGamepad->pressedB1());
    obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin, startX + buttonRadius + buttonMargin, startY + buttonMargin + buttonRadius, 1, pGamepad->pressedB2());
    obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonRadius + buttonMargin * 2, startY + buttonMargin + buttonRadius, 1, pGamepad->pressedR2());
}

void ButtonLayoutDancepadB::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + buttonRadius;
    
    obdRectangle(&obd, startX, startY, startX + buttonRadius, startY + buttonRadius, 1, pGamepad->pressedB2()); // Up/Left
    obdRectangle(&obd, startX, startY + buttonMargin * 2, startX + buttonRadius, startY + buttonRadius + buttonMargin * 2, 1, pGamepad->pressedB4()); // Down/Left
    obdRectangle(&obd, startX + buttonMargin * 2, startY, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius, 1, pGamepad->pressedB1()); // Up/Right
    obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin * 2, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius + buttonMargin * 2, 1, pGamepad->pressedB3()); // Down/Right
}

void ButtonLayoutTwinstickB::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // Stick
    obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY + (buttonMargin / 2), buttonRadius * 1.25, buttonRadius * 1.25, 1, 0);
    
    if (pGamepad->pressedB4()) {
        if (pGamepad->pressedB3()) {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
        } else if (pGamepad->pressedB2()) {
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
        } else {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY, buttonRadius, buttonRadius, 1, 1);
        }
    } else if (pGamepad->pressedB1()) {
        if (pGamepad->pressedB3()) {
            obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
        } else if (pGamepad->pressedB2()) {
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
        } else {
            obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin, buttonRadius, buttonRadius, 1, 1);
        }
    } else if (pGamepad->pressedB3()) {
        obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    } else if (pGamepad->pressedB2()) {
        obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    } else {
        obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
    }
}

void ButtonLayoutBlankB::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    // drawSticklessButtons(obd, gamepad, Gamepad* pGamepad, startX, startY, buttonRadius, buttonPadding);
}

void ButtonLayoutVLXB::draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
    const int buttonMargin = buttonPadding + (buttonRadius * 2);

    // 9-button Hori VLX
    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + (buttonMargin * 0.2), buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL1());

    obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75) - (buttonMargin / 3), startY + buttonMargin + (buttonMargin * 0.2), buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
    obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pGamepad->pressedL2());

    obdPreciseEllipse(&obd, startX + (buttonMargin * 7.4) - (buttonMargin / 3.5), startY + buttonMargin - (buttonMargin / 1.5), buttonRadius *.8, buttonRadius * .8, 1, pGamepad->pressedS2());
}

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

std::map<size_t, DisplayButtonLayout*> LAYOUTS {
    {  hash(ButtonLayoutStick::NAME), new ButtonLayoutStick() },
    {  hash(ButtonLayoutStickless::NAME), new ButtonLayoutStickless() },
    {  hash(ButtonLayoutButtonsAngled::NAME), new ButtonLayoutButtonsAngled() },
    {  hash(ButtonLayoutButtonsBasic::NAME), new ButtonLayoutButtonsBasic() },
    {  hash(ButtonLayoutKeyboardAngled::NAME), new ButtonLayoutKeyboardAngled() },
    {  hash(ButtonLayoutKeyboardA::NAME), new ButtonLayoutKeyboardA() },
    {  hash(ButtonLayoutDancepadA::NAME), new ButtonLayoutDancepadA() },
    {  hash(ButtonLayoutTwinstickA::NAME), new ButtonLayoutTwinstickA() },
    {  hash(ButtonLayoutBlankA::NAME), new ButtonLayoutBlankA() },
    {  hash(ButtonLayoutVLXA::NAME), new ButtonLayoutVLXA() },
    {  hash(ButtonLayoutFightboardStick::NAME), new ButtonLayoutFightboardStick() },
    {  hash(ButtonLayoutFightboardStickMirrored::NAME), new ButtonLayoutFightboardStickMirrored() },
};

std::map<size_t, DisplayButtonLayout*> getDisplayButtonLayouts() {
    return LAYOUTS;
}

std::map<size_t, DisplayButtonLayout*> LAYOUTS_RIGHT {
    {  hash(ButtonLayoutArcade::NAME), new ButtonLayoutArcade() },
    {  hash(ButtonLayoutSticklessb::NAME), new ButtonLayoutSticklessb() },
    {  hash(ButtonLayoutButtonsAngledB::NAME), new ButtonLayoutButtonsAngledB() },
    {  hash(ButtonLayoutVewlix::NAME), new ButtonLayoutVewlix() },
    {  hash(ButtonLayoutVewlix7::NAME), new ButtonLayoutVewlix7() },
    {  hash(ButtonLayoutCapcom::NAME), new ButtonLayoutCapcom() },
    {  hash(ButtonLayoutCapcom6::NAME), new ButtonLayoutCapcom6() },
    {  hash(ButtonLayoutSega2p::NAME), new ButtonLayoutSega2p() },
    {  hash(ButtonLayoutNoir8::NAME), new ButtonLayoutNoir8() },
    {  hash(ButtonLayoutKeyboardB::NAME), new ButtonLayoutKeyboardB() },
    {  hash(ButtonLayoutDancepadB::NAME), new ButtonLayoutDancepadB() },
    {  hash(ButtonLayoutTwinstickB::NAME), new ButtonLayoutTwinstickB() },
    {  hash(ButtonLayoutBlankB::NAME), new ButtonLayoutBlankB() },
    {  hash(ButtonLayoutVLXB::NAME), new ButtonLayoutVLXB() },
    {  hash(ButtonLayoutFightboard::NAME), new ButtonLayoutFightboard() },
    {  hash(ButtonLayoutFightboardMirrored::NAME), new ButtonLayoutFightboardMirrored() },
};

std::map<size_t, DisplayButtonLayout*> getDisplayButtonLayoutsRight() {
    return LAYOUTS_RIGHT;
}