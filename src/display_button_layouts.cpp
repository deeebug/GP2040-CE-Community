#include <vector>
#include "OneBitDisplay.h"
#include "display_button_layouts.h"

std::vector<DisplayButtonLayout*> LAYOUTS {
        new ButtonLayoutStick(),
        new ButtonLayoutStickless(),
        new ButtonLayoutButtonsAngled(),
        new ButtonLayoutButtonsBasic(),
        new ButtonLayoutKeyboardAngled(),
        new ButtonLayoutKeyboardA(),
        new ButtonLayoutDancepadA(),
        new ButtonLayoutTwinstickA(),
        new ButtonLayoutBlankA(),
        new ButtonLayoutVLXA(),
        new ButtonLayoutFightboardStick(),
        new ButtonLayoutFightboardMirrored()
};

std::vector<DisplayButtonLayout*> getDisplayButtonLayouts() {
    return LAYOUTS;
}

std::vector<DisplayButtonLayout*> LAYOUTS_RIGHT {
        new ButtonLayoutArcade(),
        new ButtonLayoutSticklessb(),
        new ButtonLayoutButtonsAngledB(),
        new ButtonLayoutVewlix(),
        new ButtonLayoutVewlix7(),
        new ButtonLayoutCapcom(),
        new ButtonLayoutCapcom6(),
        new ButtonLayoutSega2p(),
        new ButtonLayoutNoir8(),
        new ButtonLayoutKeyboardB(),
        new ButtonLayoutDancepadB(),
        new ButtonLayoutTwinstickB(),
        new ButtonLayoutBlankB(),
        new ButtonLayoutVLXB(),
        new ButtonLayoutFightboardStickMirrored(),
        new ButtonLayoutFightboard()
};

std::vector<DisplayButtonLayout*> getDisplayButtonLayoutsRight() {
    return LAYOUTS_RIGHT;
}