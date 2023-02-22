#ifndef DISPLAY_BUTTON_LAYOUTS_H_
#define DISPLAY_BUTTON_LAYOUTS_H_

#include "enums.h"
#include <map>
#include<string>
#include <map>
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
        bool pressedUp(Gamepad* gamepad, Gamepad* pGamepad);
        bool pressedDown(Gamepad* gamepad, Gamepad* pGamepad);
        bool pressedLeft(Gamepad* gamepad, Gamepad* pGamepad);
        bool pressedRight(Gamepad* gamepad, Gamepad* pGamepad);
};

class ButtonLayoutStick : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Stick";
        ButtonLayoutStick(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_STICK}, NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        ButtonLayoutStick(ButtonLayoutType buttonLayout, std::string name, DisplayButtonLayoutParams params): DisplayButtonLayout(buttonLayout, name, params) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutStickless : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Stickless";
        ButtonLayoutStickless(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_STICKLESS}, NAME, DisplayButtonLayoutParams {8, 20, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutButtonsAngled : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Buttons Angled";
		ButtonLayoutButtonsAngled(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_BUTTONS_ANGLED}, NAME, DisplayButtonLayoutParams {8, 28, 7, 3}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutButtonsBasic : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Buttons Basic";
		ButtonLayoutButtonsBasic(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_BUTTONS_BASIC}, NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutKeyboardAngled : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Keyboard Angled";
        ButtonLayoutKeyboardAngled(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_KEYBOARD_ANGLED}, "Keyboard Angled", DisplayButtonLayoutParams {18, 28, 5,}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
        void drawDiamond(OBDISP& obd, int cx, int cy, int size, uint8_t colour, uint8_t filled);
};

class ButtonLayoutKeyboardA: public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Keyboard";
		ButtonLayoutKeyboardA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_KEYBOARDA}, NAME, DisplayButtonLayoutParams {8, 28, 10, 1}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutDancepadA: public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Dancepad";
		ButtonLayoutDancepadA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_DANCEPADA}, NAME, DisplayButtonLayoutParams {39, 12, 15,}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutTwinstickA : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Twinstick";
		ButtonLayoutTwinstickA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_TWINSTICKA}, NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutBlankA : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Blank";
		ButtonLayoutBlankA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_BLANKA}, NAME, DisplayButtonLayoutParams {0, 0, 0, 0}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutVLXA : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "VLX";
		ButtonLayoutVLXA(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_VLXA}, NAME, DisplayButtonLayoutParams {7, 28, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutFightboardStick : public ButtonLayoutStick {
    public:
        ButtonLayoutFightboardStick(): ButtonLayoutStick({.layout = BUTTON_LAYOUT_FIGHTBOARD_STICK}, "Fightboard", DisplayButtonLayoutParams {18, 22, 8, 2}) {}
};

class ButtonLayoutFightboardMirrored : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Fightboard Mirrored";
		ButtonLayoutFightboardMirrored(): DisplayButtonLayout({.layout = BUTTON_LAYOUT_FIGHTBOARD_MIRRORED}, NAME, DisplayButtonLayoutParams {0, 22, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

/**
 * RIGHT layouts
*/

class ButtonLayoutArcade : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Arcade";
        ButtonLayoutArcade(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_ARCADE}, NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutSticklessb : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Stickless";
        ButtonLayoutSticklessb(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_STICKLESSB}, NAME, DisplayButtonLayoutParams {8, 20, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutButtonsAngledB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Buttons Angled";
        ButtonLayoutButtonsAngledB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_BUTTONS_ANGLEDB}, NAME, DisplayButtonLayoutParams {8, 28, 7, 3}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutVewlix : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Viewlix";
        ButtonLayoutVewlix(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_VEWLIX}, NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutVewlix7 : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Viewlix 7";
        ButtonLayoutVewlix7(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_VEWLIX7}, NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutCapcom : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Capcom";
        ButtonLayoutCapcom(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_CAPCOM}, NAME, DisplayButtonLayoutParams {6, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutCapcom6 : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Capcom 6";
        ButtonLayoutCapcom6(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_CAPCOM6}, NAME, DisplayButtonLayoutParams {16, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutSega2p : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Sega 2P";
        ButtonLayoutSega2p(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_SEGA2P}, NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutNoir8 : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Noir 8";
        ButtonLayoutNoir8(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_NOIR8}, NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutKeyboardB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Keyboard";
        ButtonLayoutKeyboardB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_KEYBOARDB}, NAME, DisplayButtonLayoutParams {68, 28, 10, 1}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutDancepadB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Dancepad";
        ButtonLayoutDancepadB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_DANCEPADB}, NAME, DisplayButtonLayoutParams {39, 12, 15, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutTwinstickB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Twinstick";
        ButtonLayoutTwinstickB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_TWINSTICKB}, NAME, DisplayButtonLayoutParams {100, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutBlankB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Blank";
        ButtonLayoutBlankB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_BLANKB}, NAME, DisplayButtonLayoutParams {0, 0, 0, 0}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutVLXB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "VLX";
        ButtonLayoutVLXB(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_VLXB}, NAME, DisplayButtonLayoutParams {6, 28, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutFightboardStickMirrored : public ButtonLayoutStick {
    public:
        ButtonLayoutFightboardStickMirrored(): ButtonLayoutStick({.layoutRight = BUTTON_LAYOUT_FIGHTBOARD_STICK_MIRRORED}, "Fightboard Mirrored", DisplayButtonLayoutParams {8, 22, 7, 3}) {}
};

class ButtonLayoutFightboard : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Fightboard";
        ButtonLayoutFightboard(): DisplayButtonLayout({.layoutRight = BUTTON_LAYOUT_FIGHTBOARD}, NAME, DisplayButtonLayoutParams {90, 22, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

std::map<size_t, DisplayButtonLayout*> getDisplayButtonLayouts();
std::map<size_t, DisplayButtonLayout*> getDisplayButtonLayoutsRight();

#endif
