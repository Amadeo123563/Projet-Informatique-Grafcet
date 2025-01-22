#include "pinmode.h"

PinMode::PinMode()
{

}

PinMode::PinMode(const PinMode::Mode &value)
{
    setMode(value);
}

PinMode::Mode PinMode::getMode() const
{
    return mode;
}

std::string PinMode::toString() const
{
    switch (mode) {
    case Input:
        return "INPUT";
    case Output:
        return "OUTPUT";
    case Input_Pullup:
        return "INPUT_PULLUP";
    case Input_Pulldown:
        return "INPUT_PULLDOWN";
    }
}

void PinMode::setMode(const Mode &value)
{
    mode = value;
}
