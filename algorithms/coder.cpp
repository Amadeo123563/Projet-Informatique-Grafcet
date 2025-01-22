#include "coder.h"

Coder::Coder(const std::vector<Step>& steps, const std::vector<Action>& actions, const std::vector<Transition>& transitions)
{
    codeSteps.reserve(steps.size());

    //TODO
}

const std::vector<CodeStep>& Coder::getCodeSteps() const
{
    return codeSteps;
}

const std::unordered_map<int, PinMode>& Coder::getPinmodes() const
{
    return pinmodes;
}

const std::vector<std::string>& Coder::getGlobalVariables() const
{
    return globalVariables;
}
