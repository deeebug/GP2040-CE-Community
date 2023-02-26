#ifndef LED_BUTTON_LAYOUTS_H_
#define LED_BUTTON_LAYOUTS_H_

#include<string>
#include <map>
#include <vector>

struct LabelMaskPair {
    std::string label;
    uint32_t mask;
};

const LabelMaskPair PIXEL_OFF \
	{ .label={}, .mask=0 };

class LEDButtonLayout {
    public:
        LEDButtonLayout(std::string name): name(name) {};
        virtual std::vector<std::vector<LabelMaskPair>> generatedLEDButtons() = 0;
        std::string getName() { return name; }
    private:
        std::string name;
};

class LEDButtonLayoutButtons : public LEDButtonLayout {
    public:
        static constexpr const char* const NAME = "8-Button Layout";
        LEDButtonLayoutButtons(): LEDButtonLayout(NAME) {}
        std::vector<std::vector<LabelMaskPair>> generatedLEDButtons();
};

class LEDButtonLayoutStickless : public LEDButtonLayout {
    public:
        static constexpr const char* const NAME = "Stickless Layout";
        LEDButtonLayoutStickless(): LEDButtonLayout(NAME) {}
        std::vector<std::vector<LabelMaskPair>> generatedLEDButtons();
};

class LEDButtonLayoutWasd : public LEDButtonLayout {
    public:
        static constexpr const char* const NAME = "WASD Layout";
        LEDButtonLayoutWasd(): LEDButtonLayout(NAME) {}
        std::vector<std::vector<LabelMaskPair>> generatedLEDButtons();
};

std::map<size_t, LEDButtonLayout*> getLEDButtonLayouts();
LEDButtonLayout* getLEDButtonLayout(size_t layoutId);
size_t getLEDButtonLayoutId(std::string name);
void registerLEDButtonLayout(LEDButtonLayout* layout);

#endif
