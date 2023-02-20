#ifndef DISPLAY_BUTTON_LAYOUTS_H_
#define DISPLAY_BUTTON_LAYOUTS_H_

#include "enums.h"
#include <map>
#include<string>
#include <vector>
#include "OneBitDisplay.h"
#include "gamepad.h"

union ButtonLayoutType {
    ButtonLayout layout;
    ButtonLayoutRight layoutRight;
};

struct DisplayButtonLayoutParams {
    int startX;
	int startY;
	int buttonRadius;
	int buttonPadding;
};

class DisplayButtonLayout {
    public:
        DisplayButtonLayout(ButtonLayoutType buttonLayout, std::string name, DisplayButtonLayoutParams params): id(buttonLayout), name(name), defaultParams(params) {};
        virtual void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) = 0;
        ButtonLayoutType getId() { return id; }
        std::string getName() { return name; }
        DisplayButtonLayoutParams getDefaultParams() { return defaultParams; }
    private:
        ButtonLayoutType id;
        std::string name;
        DisplayButtonLayoutParams defaultParams;
    protected:
        bool pressedUp(Gamepad* gamepad, Gamepad* pGamepad)
        {
            switch (gamepad->options.dpadMode)
            {
                case DPAD_MODE_DIGITAL:      return pGamepad->pressedUp();
                case DPAD_MODE_LEFT_ANALOG:  return pGamepad->state.ly == GAMEPAD_JOYSTICK_MIN;
                case DPAD_MODE_RIGHT_ANALOG: return pGamepad->state.ry == GAMEPAD_JOYSTICK_MIN;
            }

            return false;
        }

        bool pressedDown(Gamepad* gamepad, Gamepad* pGamepad)
        {
            switch (gamepad->options.dpadMode)
            {
                case DPAD_MODE_DIGITAL:      return pGamepad->pressedDown();
                case DPAD_MODE_LEFT_ANALOG:  return pGamepad->state.ly == GAMEPAD_JOYSTICK_MAX;
                case DPAD_MODE_RIGHT_ANALOG: return pGamepad->state.ry == GAMEPAD_JOYSTICK_MAX;
            }

            return false;
        }

        bool pressedLeft(Gamepad* gamepad, Gamepad* pGamepad)
        {
            switch (gamepad->options.dpadMode)
            {
                case DPAD_MODE_DIGITAL:      return pGamepad->pressedLeft();
                case DPAD_MODE_LEFT_ANALOG:  return pGamepad->state.lx == GAMEPAD_JOYSTICK_MIN;
                case DPAD_MODE_RIGHT_ANALOG: return pGamepad->state.rx == GAMEPAD_JOYSTICK_MIN;
            }

            return false;
        }

        bool pressedRight(Gamepad* gamepad, Gamepad* pGamepad)
        {
            switch (gamepad->options.dpadMode)
            {
                case DPAD_MODE_DIGITAL:      return pGamepad->pressedRight();
                case DPAD_MODE_LEFT_ANALOG:  return pGamepad->state.lx == GAMEPAD_JOYSTICK_MAX;
                case DPAD_MODE_RIGHT_ANALOG: return pGamepad->state.rx == GAMEPAD_JOYSTICK_MAX;
            }

            return false;
        }
};


class ButtonLayoutStick : public DisplayButtonLayout {
    public:
        ButtonLayoutStick(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_STICK}, "Stick", DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        ButtonLayoutStick(ButtonLayoutType buttonLayout, std::string name, DisplayButtonLayoutParams params): DisplayButtonLayout(buttonLayout, name, params) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutStickless : public DisplayButtonLayout {
    public:
        ButtonLayoutStickless(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_STICKLESS}, "Stickless", DisplayButtonLayoutParams {8, 20, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + (buttonRadius * 2);

            obdPreciseEllipse(&obd, startX, startY, buttonRadius, buttonRadius, 1, pressedLeft(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + buttonMargin, startY, buttonRadius, buttonRadius, 1, pressedDown(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + (buttonMargin * 1.875), startY + (buttonMargin / 2), buttonRadius, buttonRadius, 1, pressedRight(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + (buttonMargin * 2.25), startY + buttonMargin * 1.875, buttonRadius, buttonRadius, 1, pressedUp(gamepad, pGamepad));
        }
};

class ButtonLayoutButtonsAngled : public DisplayButtonLayout {
    public:
        ButtonLayoutButtonsAngled(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_BUTTONS_ANGLED}, "Buttons Angled", DisplayButtonLayoutParams {8, 28, 7, 3}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + (buttonRadius * 2);

            // WASD
            obdPreciseEllipse(&obd, startX, startY + buttonMargin * 0.5, buttonRadius, buttonRadius, 1, pressedLeft(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin * 0.875, buttonRadius, buttonRadius, 1, pressedDown(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + buttonMargin * 1.5, startY - buttonMargin * 0.125, buttonRadius, buttonRadius, 1, pressedUp(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + (buttonMargin * 2), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, pressedRight(gamepad, pGamepad));
        }
};

class ButtonLayoutButtonsBasic : public DisplayButtonLayout {
    public:
        ButtonLayoutButtonsBasic(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_BUTTONS_BASIC}, "Buttons Basic", DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + (buttonRadius * 2);

            // UDLR
            obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, pressedLeft(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, pressedUp(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, pressedDown(gamepad, pGamepad));
            obdPreciseEllipse(&obd, startX + (buttonMargin * 1.625), startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, pressedRight(gamepad, pGamepad));
        }
};

class ButtonLayoutKeyboardAngled : public DisplayButtonLayout {
    public:
        ButtonLayoutKeyboardAngled(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_KEYBOARD_ANGLED}, "Keyboard Angled", DisplayButtonLayoutParams {18, 28, 5,}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + (buttonRadius * 2);

            // MixBox
            drawDiamond(obd, startX, startY, buttonRadius, 1, pressedLeft(gamepad, pGamepad));
            drawDiamond(obd, startX + buttonMargin / 2, startY + buttonMargin / 2, buttonRadius, 1, pressedDown(gamepad, pGamepad));
            drawDiamond(obd, startX + buttonMargin, startY, buttonRadius, 1, pressedUp(gamepad, pGamepad));
            drawDiamond(obd, startX + buttonMargin, startY + buttonMargin, buttonRadius, 1, pressedRight(gamepad, pGamepad));
        }
    private:
        void drawDiamond(OBDISP& obd, int cx, int cy, int size, uint8_t colour, uint8_t filled)
        {
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
};

class ButtonLayoutKeyboardA: public DisplayButtonLayout {
    public:
        ButtonLayoutKeyboardA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_KEYBOARDA}, "Keyboard", DisplayButtonLayoutParams {8, 28, 10, 1}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + buttonRadius;

            // MAME
            obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonRadius, startY + buttonRadius + buttonMargin, 1, pressedLeft(gamepad, pGamepad));
            obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin, startX + buttonRadius + buttonMargin, startY + buttonRadius + buttonMargin, 1, pressedDown(gamepad, pGamepad));
            obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonRadius + buttonMargin, startY + buttonRadius, 1, pressedUp(gamepad, pGamepad));
            obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius + buttonMargin, 1, pressedRight(gamepad, pGamepad));
        }
};

class ButtonLayoutDancepadA: public DisplayButtonLayout {
    public:
        ButtonLayoutDancepadA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_DANCEPADA}, "Dancepad", DisplayButtonLayoutParams {39, 12, 15,}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + buttonRadius;

            obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonRadius, startY + buttonRadius + buttonMargin, 1, pressedLeft(gamepad, pGamepad));
            obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin * 2, startX + buttonRadius + buttonMargin, startY + buttonRadius + buttonMargin * 2, 1, pressedDown(gamepad, pGamepad));
            obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonRadius + buttonMargin, startY + buttonRadius, 1, pressedUp(gamepad, pGamepad));
            obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius + buttonMargin, 1, pressedRight(gamepad, pGamepad));
        }
};

class ButtonLayoutTwinstickA : public DisplayButtonLayout {
    public:
        ButtonLayoutTwinstickA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_TWINSTICKA}, "Twinstick", DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutBlankA : public DisplayButtonLayout {
    public:
        ButtonLayoutBlankA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_BLANKA}, "Blank", DisplayButtonLayoutParams {0, 0, 0, 0}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
        }
};

class ButtonLayoutVLXA : public DisplayButtonLayout {
    public:
        ButtonLayoutVLXA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_VLXA}, "VLX", DisplayButtonLayoutParams {7, 28, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutFightboardStick : public ButtonLayoutStick {
    public:
        ButtonLayoutFightboardStick(): ButtonLayoutStick({.layout = BUTTON_LAYOUT_FIGHTBOARD_STICK}, "Fightboard", DisplayButtonLayoutParams {18, 22, 8, 2}) {}
};

class ButtonLayoutFightboardMirrored : public DisplayButtonLayout {
    public:
        ButtonLayoutFightboardMirrored(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_FIGHTBOARD_MIRRORED}, "Fightboard Mirrored", DisplayButtonLayoutParams {0, 22, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

/**
 * RIGHT layouts
*/

class ButtonLayoutArcade : public DisplayButtonLayout {
    public:
        ButtonLayoutArcade(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_ARCADE}, "Arcade", DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutSticklessb : public DisplayButtonLayout {
    public:
        ButtonLayoutSticklessb(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_STICKLESSB}, "Stickless", DisplayButtonLayoutParams {8, 20, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutButtonsAngledB : public DisplayButtonLayout {
    public:
        ButtonLayoutButtonsAngledB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_BUTTONS_ANGLEDB}, "Buttons Angled", DisplayButtonLayoutParams {8, 28, 7, 3}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutVewlix : public DisplayButtonLayout {
    public:
        ButtonLayoutVewlix(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_VEWLIX}, "Viewlix", DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutVewlix7 : public DisplayButtonLayout {
    public:
        ButtonLayoutVewlix7(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_VEWLIX7}, "Viewlix 7", DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutCapcom : public DisplayButtonLayout {
    public:
        ButtonLayoutCapcom(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_CAPCOM}, "Capcom", DisplayButtonLayoutParams {6, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutCapcom6 : public DisplayButtonLayout {
    public:
        ButtonLayoutCapcom6(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_CAPCOM6}, "Capcom 6", DisplayButtonLayoutParams {16, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + (buttonRadius * 2);

            // 6-button Capcom
            obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB3());
            obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedB4());
            obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY, buttonRadius, buttonRadius, 1, pGamepad->pressedR1());

            obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB1());
            obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedB2());
            obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, pGamepad->pressedR2());
        }
};

class ButtonLayoutSega2p : public DisplayButtonLayout {
    public:
        ButtonLayoutSega2p(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_SEGA2P}, "Sega 2P", DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutNoir8 : public DisplayButtonLayout {
    public:
        ButtonLayoutNoir8(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_NOIR8}, "Noir 8", DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutKeyboardB : public DisplayButtonLayout {
    public:
        ButtonLayoutKeyboardB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_KEYBOARDB}, "Keyboard", DisplayButtonLayoutParams {68, 28, 10, 1}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + buttonRadius;

            // 6-button MAME Style
            obdRectangle(&obd, startX, startY, startX + buttonRadius, startY + buttonRadius, 1, pGamepad->pressedB3());
            obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonRadius + buttonMargin, startY + buttonRadius, 1, pGamepad->pressedB4());
            obdRectangle(&obd, startX + buttonMargin * 2, startY, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius, 1, pGamepad->pressedR1());

            obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonRadius, startY + buttonMargin + buttonRadius, 1, pGamepad->pressedB1());
            obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin, startX + buttonRadius + buttonMargin, startY + buttonMargin + buttonRadius, 1, pGamepad->pressedB2());
            obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonRadius + buttonMargin * 2, startY + buttonMargin + buttonRadius, 1, pGamepad->pressedR2());
        }
};

class ButtonLayoutDancepadB : public DisplayButtonLayout {
    public:
        ButtonLayoutDancepadB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_DANCEPADB}, "Dancepad", DisplayButtonLayoutParams {39, 12, 15, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            const int buttonMargin = buttonPadding + buttonRadius;
            
            obdRectangle(&obd, startX, startY, startX + buttonRadius, startY + buttonRadius, 1, pGamepad->pressedB2()); // Up/Left
            obdRectangle(&obd, startX, startY + buttonMargin * 2, startX + buttonRadius, startY + buttonRadius + buttonMargin * 2, 1, pGamepad->pressedB4()); // Down/Left
            obdRectangle(&obd, startX + buttonMargin * 2, startY, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius, 1, pGamepad->pressedB1()); // Up/Right
            obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin * 2, startX + buttonRadius + buttonMargin * 2, startY + buttonRadius + buttonMargin * 2, 1, pGamepad->pressedB3()); // Down/Right
        }
};

class ButtonLayoutTwinstickB : public DisplayButtonLayout {
    public:
        ButtonLayoutTwinstickB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_TWINSTICKB}, "Twinstick", DisplayButtonLayoutParams {100, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutBlankB : public DisplayButtonLayout {
    public:
        ButtonLayoutBlankB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_BLANKB}, "Blank", DisplayButtonLayoutParams {0, 0, 0, 0}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
            // drawSticklessButtons(obd, gamepad, Gamepad* pGamepad, startX, startY, buttonRadius, buttonPadding);
        }
};

class ButtonLayoutVLXB : public DisplayButtonLayout {
    public:
        ButtonLayoutVLXB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_VLXB}, "VLX", DisplayButtonLayoutParams {6, 28, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

class ButtonLayoutFightboardStickMirrored : public ButtonLayoutStick {
    public:
        ButtonLayoutFightboardStickMirrored(): ButtonLayoutStick({.layoutRight = BUTTON_LAYOUT_FIGHTBOARD_STICK_MIRRORED}, "Fightboard Mirrored", DisplayButtonLayoutParams {8, 22, 7, 3}) {}
};

class ButtonLayoutFightboard : public DisplayButtonLayout {
    public:
        ButtonLayoutFightboard(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_FIGHTBOARD}, "Fightboard", DisplayButtonLayoutParams {90, 22, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) {
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
};

std::vector<DisplayButtonLayout*> getDisplayButtonLayouts();
std::vector<DisplayButtonLayout*> getDisplayButtonLayoutsRight();

#endif
