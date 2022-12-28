#ifndef _InputMacro_H
#define _InputMacro_H

#include "gpaddon.h"

#include "GamepadEnums.h"

// Input Macro Module Name
#define InputMacroName "Input Macro"

enum MacroType {
    ON_RELEASE,
    ON_HOLD,
    ON_HOLD_REPEAT,
    ON_RELEASE_TOGGLE
};

struct Input {
    GamepadState state;
    int duration = -1;
};

class Macro {
    public:
        size_t sizeOf() {
            return   sizeof(int) + (size * sizeof(Input))
                   + name.size() + (sizeof(char) * name.size() + 1)
                   + (sizeof(type));
        }

        bool operator==(Macro& other) const {
            bool result = size == other.size
                       && type == other.type
                       && name == other.name;
            if (!result) return result;
            for (size_t i = 0; i < size; i++)
            {
                Input input = inputs[i]; GamepadState state = input.state;
                Input otherInput = other.inputs[i]; GamepadState otherState = otherInput.state;
                result = result && input.duration == otherInput.duration
                       && state.dpad == otherState.dpad
                       && state.buttons == otherState.buttons
                       && state.lt == otherState.lt
                       && state.rt == otherState.rt
                       && state.lx == otherState.lx
                       && state.ly == otherState.ly
                       && state.rx == otherState.rx
                       && state.ry == otherState.ry
                       && state.aux == otherState.aux;
                if (!result) return result;
            }
            
            return result;
        }

        bool operator!=(Macro& other) const {
            return !operator==(other);
        }
        
        Input* inputs;
        std::string name;
        MacroType type;
        int size;
};

Macro* createMacro(Input*, const char*, MacroType, int);

class InputMacro : public GPAddon {
public:
	virtual bool available();   // GPAddon available
	virtual void setup();       // Analog Setup
	virtual void process();     // Analog Process
    virtual std::string name() { return InputMacroName; }
private:
};

#endif  // _InputMacro_H_