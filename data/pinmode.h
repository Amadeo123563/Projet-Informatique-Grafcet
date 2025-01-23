#ifndef PINMODE_H
#define PINMODE_H

#include <string>

class PinMode
{
public:
    enum Mode {
        None = 0,
        Output = 1,
        Input_Pullup = 2,
        Input_Pulldown = 3,
        Input = 4
    };
    PinMode();
    PinMode(const Mode &value);

    Mode getMode() const;
    std::string toString() const;
    void setMode(const Mode &value);
    void forceSetMode(const Mode &value);

private:
    Mode mode;
};

#endif // PINMODE_H
