#ifndef CODER_H
#define CODER_H

#include <vector>
#include <unordered_map>
#include "data/step.h"
#include "data/action.h"
#include "data/transition.h"
#include "data/codestep.h"
#include "data/pinmode.h"

class Coder
{
public:
    Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions);

    const std::vector<CodeStep>& getCodeSteps() const;

    const std::unordered_map<int, PinMode>& getPinmodes() const;

private:
    std::vector<CodeStep> codeSteps;
    std::unordered_map<int, PinMode> pinmodes;
};

#endif // CODER_H
