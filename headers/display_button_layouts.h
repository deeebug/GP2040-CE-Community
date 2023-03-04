#ifndef DISPLAY_BUTTON_LAYOUTS_H_
#define DISPLAY_BUTTON_LAYOUTS_H_

#include "enums.h"
#include <map>
#include<string>
#include <map>
#include "OneBitDisplay.h"
#include "gamepad.h"

struct DisplayButtonLayoutParams {
    int startX;
	int startY;
	int buttonRadius;
	int buttonPadding;
};

class DisplayButtonLayout {
    public:
        DisplayButtonLayout(std::string name, DisplayButtonLayoutParams params): name(name), defaultParams(params) {};
        virtual void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding) = 0;
        std::string getName() { return name; }
        DisplayButtonLayoutParams getDefaultParams() { return defaultParams; }
    private:
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
        ButtonLayoutStick(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        ButtonLayoutStick(std::string name, DisplayButtonLayoutParams params): DisplayButtonLayout(name, params) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutStickless : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Stickless";
        ButtonLayoutStickless(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 20, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutButtonsAngled : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Buttons Angled";
		ButtonLayoutButtonsAngled(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 7, 3}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutButtonsBasic : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Buttons Basic";
		ButtonLayoutButtonsBasic(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutKeyboardAngled : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Keyboard Angled";
        ButtonLayoutKeyboardAngled(): DisplayButtonLayout("Keyboard Angled", DisplayButtonLayoutParams {18, 28, 5,}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
        void drawDiamond(OBDISP& obd, int cx, int cy, int size, uint8_t colour, uint8_t filled);
};

class ButtonLayoutKeyboardA: public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Keyboard";
		ButtonLayoutKeyboardA(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 10, 1}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutDancepadA: public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Dancepad";
		ButtonLayoutDancepadA(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {39, 12, 15,}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutTwinstickA : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Twinstick";
		ButtonLayoutTwinstickA(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutBlankA : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Blank";
		ButtonLayoutBlankA(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {0, 0, 0, 0}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutVLXA : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "VLX";
		ButtonLayoutVLXA(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {7, 28, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutFightboardStick : public ButtonLayoutStick {
    public:
        static constexpr const char* const NAME = "Fightboard";
        ButtonLayoutFightboardStick(): ButtonLayoutStick(NAME, DisplayButtonLayoutParams {18, 22, 8, 2}) {}
};

class ButtonLayoutFightboardStickMirrored : public ButtonLayoutStick {
    public:
        static constexpr const char* const NAME = "Fightboard Mirrored";
        ButtonLayoutFightboardStickMirrored(): ButtonLayoutStick(NAME, DisplayButtonLayoutParams {90, 22, 8, 2}) {}
};

/**
 * RIGHT layouts
*/

class ButtonLayoutArcade : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Arcade";
        ButtonLayoutArcade(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutSticklessb : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Stickless";
        ButtonLayoutSticklessb(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 20, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutButtonsAngledB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Buttons Angled";
        ButtonLayoutButtonsAngledB(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 7, 3}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutVewlix : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Viewlix";
        ButtonLayoutVewlix(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutVewlix7 : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Viewlix 7";
        ButtonLayoutVewlix7(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutCapcom : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Capcom";
        ButtonLayoutCapcom(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {6, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutCapcom6 : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Capcom 6";
        ButtonLayoutCapcom6(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {16, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutSega2p : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Sega 2P";
        ButtonLayoutSega2p(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutNoir8 : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Noir 8";
        ButtonLayoutNoir8(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutKeyboardB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Keyboard";
        ButtonLayoutKeyboardB(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {68, 28, 10, 1}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutDancepadB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Dancepad";
        ButtonLayoutDancepadB(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {39, 12, 15, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutTwinstickB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Twinstick";
        ButtonLayoutTwinstickB(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {100, 28, 8, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutBlankB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Blank";
        ButtonLayoutBlankB(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {0, 0, 0, 0}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutVLXB : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "VLX";
        ButtonLayoutVLXB(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {6, 28, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutFightboard : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Fightboard";
        ButtonLayoutFightboard(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {8, 22, 7, 3}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

class ButtonLayoutFightboardMirrored : public DisplayButtonLayout {
    public:
        static constexpr const char* const NAME = "Fightboard Mirrored";
        ButtonLayoutFightboardMirrored(): DisplayButtonLayout(NAME, DisplayButtonLayoutParams {0, 22, 7, 2}) {}
        void draw(OBDISP& obd, Gamepad* gamepad, Gamepad* pGamepad, int startX, int startY, int buttonRadius, int buttonPadding);
};

std::map<size_t, DisplayButtonLayout*> getDisplayButtonLayouts();
std::map<size_t, DisplayButtonLayout*> getDisplayButtonLayoutsRight();

#endif
