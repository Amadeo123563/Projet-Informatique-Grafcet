#ifndef PINMODE_H
#define PINMODE_H

#include <string>

class PinMode
{
public:
    enum Mode {
        Input,
        Output,
        Input_Pullup,
        Input_Pulldown
    };
    PinMode();
    PinMode(const Mode &value);

    Mode getMode() const;
    std::string toString() const;
    void setMode(const Mode &value);

private:
    Mode mode;
};

#endif // PINMODE_H
