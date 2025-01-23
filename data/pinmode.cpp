#include "pinmode.h"

PinMode::PinMode()
{
    mode = None;
}

PinMode::PinMode(const PinMode::Mode &value)
{
    forceSetMode(value);
}

PinMode::Mode PinMode::getMode() const
{
    return mode;
}

std::string PinMode::toString() const
{
    switch (mode) {
    case None:
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
    if(mode > value) //Priorize Input
        return;
    mode = value;
}

void PinMode::forceSetMode(const PinMode::Mode &value)
{
    mode = value;
}
